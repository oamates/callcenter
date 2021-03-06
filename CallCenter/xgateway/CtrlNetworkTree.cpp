//
// CtrlNetworkTree.cpp
//

#include "stdafx.h"
#include "Xgateway.h"
#include "CoolInterface.h"

#include "WndNetwork.h"
#include "CtrlNetworkTree.h"
#include "CtrlNetworkVideo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CNetworkTree, CTreeCtrl)
BEGIN_MESSAGE_MAP(CNetworkTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CNetworkTree)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCPAINT()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// CNetworkTree construction

CNetworkTree::CNetworkTree()
{
}

CNetworkTree::~CNetworkTree()
{
}

void CNetworkTree::Clear()
{
	HTREEITEM hItem = GetNextItem( NULL, TVGN_CHILD );

	for ( ; hItem != NULL ; hItem = GetNextItem( NULL, TVGN_CHILD ) )
	{
		this->RemoveItem( hItem );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkTree message handlers

BOOL CNetworkTree::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
	dwStyle |= WS_CHILD|/*TVS_PRIVATEIMAGELISTS|TVS_HASLINES|*/TVS_LINESATROOT;
	dwStyle |= /*TVS_HASBUTTONS|*/TVS_SHOWSELALWAYS;
	return CTreeCtrl::Create( dwStyle, rect, pParentWnd, IDC_NETWORK_TREE );
}

int CNetworkTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if ( CTreeCtrl::OnCreate( lpCreateStruct ) == -1 )return -1;
	
	SetImageList( &CoolInterface.m_pImages, TVSIL_NORMAL );
	
	return 0;
}

void CNetworkTree::OnDestroy() 
{
	Clear();
	
	CTreeCtrl::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkTree custom message handlers

void CNetworkTree::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVCUSTOMDRAW* pDraw = (NMTVCUSTOMDRAW*)pNMHDR;

	if ( pDraw->nmcd.dwDrawStage == CDDS_PREPAINT )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( pDraw->nmcd.dwDrawStage == (CDDS_ITEM|CDDS_PREPAINT) )
	{
		BOOL bSelected		= GetItemState( (HTREEITEM)pDraw->nmcd.dwItemSpec, TVIS_SELECTED ) & TVIS_SELECTED ? TRUE : FALSE;
		pDraw->clrText		= GetSysColor( bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT );
		pDraw->clrTextBk	= GetSysColor( bSelected ? COLOR_HIGHLIGHT : COLOR_WINDOW );
		*pResult = CDRF_DODEFAULT;
	}
}

void CNetworkTree::OnNcPaint()
{
	CTreeCtrl::OnNcPaint();

	if ( GetStyle() & WS_BORDER )
	{
		CWindowDC dc( this );
		CRect rc;

		COLORREF crBorder = GetSysColor( COLOR_ACTIVECAPTION );

		GetWindowRect( &rc );
		rc.OffsetRect( -rc.left, -rc.top );
		dc.Draw3dRect( &rc, crBorder, crBorder );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkTree operations

BOOL CNetworkTree::RemoveItem(HTREEITEM hItem)
{
	HTREEITEM hItem2 = GetNextItem( hItem, TVGN_NEXT );
	if ( ! hItem2 ) hItem2 = GetNextItem( hItem, TVGN_PREVIOUS );
	
	CNetworkChild* pChild = (CNetworkChild*)GetItemData(hItem);
	CTreeCtrl::DeleteItem( hItem );
	
	if ( hItem2 == NULL )
	{
		((CNetworkWnd*)GetParent()) ->SetActivePage( NULL );
	}
	else
	{
		SelectItem( hItem2 );
	}
	
	if ( pChild )
	{
		pChild->DestroyWindow();
		delete pChild;
	}
	
	return TRUE;
}

BOOL CNetworkTree::RemoveItem(CNetworkChild* pChild)
{
	HTREEITEM hItem = FindItem( pChild );
	return RemoveItem( hItem );
}

CNetworkChild* CNetworkTree::AddItem(LPCTSTR xUserGrp, LPCTSTR xGroupId)
{
	CNetworkChild* pChild = new CNetworkChild( this, xGroupId );
	pChild->Create( WS_CHILD, CRect(0,0,0,0), GetParent() );
	
	InsertItem(
		TVIF_PARAM|TVIF_TEXT|TVIF_STATE|TVIF_IMAGE|TVIF_SELECTEDIMAGE,
		(LPCTSTR)xUserGrp, 9, 9, TVIS_EXPANDED,
		TVIS_EXPANDED, (LPARAM)pChild, TVI_ROOT, TVI_LAST );
	
	return pChild;
}

int CNetworkTree::AddUser(LPCTSTR xUserId, CNetworkVideo*& pxVideo)
{
	CString xUserGrp, xGroupId;
	try
	{
		CString strQuery;
		
		strQuery.Format( "select * from xenterprise"
			" where xentercode in (select xentercode from xoperator where xcode = '%s')"
			" or xentercode = '%s'", xUserId, GUEST_CODE );
		
		TRACE( strQuery + "\n" );
		theApp.Message( MSG_DEBUG, strQuery );
		
		_RecordsetPtr xRecordset;
		xRecordset = xExecute(theApp.m_pConnection, (_bstr_t)strQuery, adCmdText);
		if ( xRecordset->adoEOF ) return EBase_NoUser;
		
		int nResult = OnReadGroup( xRecordset, xUserGrp, xGroupId );
		if ( nResult != 0 ) return nResult;
		
		CNetworkChild* pChild = FindItem( xGroupId );
		if ( ! pChild ) pChild = AddItem( xUserGrp, xGroupId );
		
		pxVideo = new CNetworkVideo( pChild, xUserId );
		pChild->AddBox( pxVideo );
		
		_variant_t TheValue = xRecordset->GetCollect( "xphone" );
		if ( VT_NULL != TheValue.vt ) pxVideo->SetLinkId( (LPCTSTR)(_bstr_t)TheValue );
	}
	catch (_com_error e)
	{
		return EBase_Unknown;
	}
	
	return 0;
}

int CNetworkTree::OnReadGroup(_RecordsetPtr xRecordset, CString& xUserGrp, CString& xGroupId)
{
	_variant_t TheValue = xRecordset->GetCollect( "xentercode" );
	ASSERT( VT_NULL != TheValue.vt ); xGroupId = (LPCTSTR)(_bstr_t)TheValue;
	
	TheValue = xRecordset->GetCollect( "xentername" );
	if ( VT_NULL == TheValue.vt ) xUserGrp = _T("<NULL>");
	else	xUserGrp = (LPCTSTR)(_bstr_t)TheValue;
	
	TheValue = xRecordset->GetCollect( "xexpiredate" );
	if ( VT_NULL != TheValue.vt )
	{
		COleDateTime tt = TheValue; tt.SetTime( 0, 0, 0 );
		
		COleDateTimeSpan tSpan = COleDateTime::GetCurrentTime() - tt;
		if ( tSpan.GetTotalDays() > 0 ) return EBase_NoPay;
	}
	
	return 0;
}

CNetworkChild* CNetworkTree::FindItem(LPCTSTR xGroupId) const
{
	HTREEITEM hItem = GetNextItem( NULL, TVGN_CHILD );
	
	for ( ; hItem != NULL ; hItem = GetNextItem( hItem, TVGN_NEXT ) )
	{
		CNetworkChild* pChild = (CNetworkChild*)GetItemData( hItem );
		if ( ! pChild || ! pChild->IsKindOf( RUNTIME_CLASS(CNetworkChild) ) ) return NULL;
		
		if ( pChild->IsGroupCompatible(xGroupId) ) return pChild;
	}
	
	return NULL;
}

HTREEITEM CNetworkTree::FindItem(CNetworkChild* pChild) const
{
	HTREEITEM hItem = GetNextItem( NULL, TVGN_CHILD );
	
	for ( ; hItem != NULL ; hItem = GetNextItem( hItem, TVGN_NEXT ) )
	{
		CNetworkChild* pChild2 = (CNetworkChild*)GetItemData( hItem );
		if ( ! pChild2 || ! pChild2->IsKindOf( RUNTIME_CLASS(CNetworkChild) ) ) return NULL;
		
		if ( pChild2 == pChild ) return hItem;
	}
	
	return NULL;
}
