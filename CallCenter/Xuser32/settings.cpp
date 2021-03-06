//
// Settings.cpp
//

#include "stdafx.h"
#include "Settings.h"
#include "Registry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSettings Settings;

//////////////////////////////////////////////////////////////////////
// CSettings setup

void CSettings::Setup()
{
	Add( _T(".Debug"), &General.Debug, FALSE );
	Add( _T(".DebugLog"), &General.DebugLog, FALSE );
	Add( _T(".Reinstall"), &General.Reinstall, TRUE );
	
	Add( _T("Database.Host"), &Database.Host, "." );
	Add( _T("Database.Database"), &Database.Database, "CallCenter" );
	
	Add( _T("Network.Host"), &Network.Host, "127.0.0.1" );
	Add( _T("Network.Port"), &Network.Port, 9960 );
	
	Add( _T("Audio.Input"), &Audio.Input, NULL );
	Add( _T("Audio.Render"), &Audio.Render, NULL );
	
	Add( _T("Video.Input"), &Video.Input, NULL );
}

//////////////////////////////////////////////////////////////////////
// CSettings construction

CSettings::CSettings()
{
	TCHAR szPath[128];
	GetModuleFileName( NULL, szPath, 128 );
	
	General.Path = szPath;
	if ( General.Path.ReverseFind( '\\' ) >= 0 )
		General.Path = General.Path.Left( General.Path.ReverseFind( '\\' ) );
	
	Setup();
}

CSettings::~CSettings()
{
	for ( POSITION pos = m_pItems.GetHeadPosition() ; pos ; )
	{
		delete (Item*)m_pItems.GetNext( pos );
	}
}

//////////////////////////////////////////////////////////////////////
// CSettings add items

void CSettings::Add(LPCTSTR pszName, DWORD* pDword, DWORD nDefault)
{
	m_pItems.AddTail( new Item( pszName, pDword, NULL, NULL ) );
	*pDword = nDefault;
}

void CSettings::Add(LPCTSTR pszName, int* pDword, DWORD nDefault)
{
	m_pItems.AddTail( new Item( pszName, (DWORD*)pDword, NULL, NULL ) );
	*pDword = nDefault;
}

void CSettings::Add(LPCTSTR pszName, DOUBLE* pFloat, DOUBLE nDefault)
{
	m_pItems.AddTail( new Item( pszName, NULL, pFloat, NULL ) );
	*pFloat = nDefault;
}

void CSettings::Add(LPCTSTR pszName, CString* pString, LPCTSTR pszDefault)
{
	m_pItems.AddTail( new Item( pszName, NULL, NULL, pString ) );
	if ( pszDefault ) *pString = pszDefault;
}

//////////////////////////////////////////////////////////////////////
// CSettings load

void CSettings::Load()
{
	CRegistry pRegistry;
	
	for ( POSITION pos = m_pItems.GetHeadPosition() ; pos ; )
	{
		Item* pItem = (Item*)m_pItems.GetNext( pos );
		pItem->Load();
	}
	
	if ( General.Reinstall )
	{
		InstallCapture(CLSID_AudioInputDeviceCategory, Audio.Input);
		InstallCapture(CLSID_AudioRendererCategory, Audio.Render);
		
		General.Reinstall = FALSE;
	}
}

void CSettings::Save()
{
	for ( POSITION pos = m_pItems.GetHeadPosition() ; pos ; )
	{
		Item* pItem = (Item*)m_pItems.GetNext( pos );
		if ( pItem->m_sName != _T(".Path") ) pItem->Save();
	}
}

HRESULT CSettings::InstallCapture(REFCLSID clsidDevClass, CString& pSetting)
{
	HRESULT hr = S_OK;
	
	CComPtr<ICreateDevEnum> icDevEnum;
	hr = CoCreateInstance( CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&icDevEnum );
	if ( hr != S_OK ) return hr;
	
	CComPtr<IEnumMoniker> ieMoniker;
	hr = icDevEnum->CreateClassEnumerator( clsidDevClass, &ieMoniker, 0 );
	if ( hr != S_OK || ieMoniker == NULL ) return hr;
	
	IMoniker* pMoniker;
	hr = ieMoniker->Next( 1, &pMoniker, NULL );
	if ( hr != S_OK || pMoniker == NULL ) return hr;
	
	LPOLESTR pDisplayName = NULL;
	pMoniker->GetDisplayName( NULL, NULL, &pDisplayName );
	
	pSetting = pDisplayName;
	CoTaskMemFree( pDisplayName );
	
	return S_FALSE;
}

//////////////////////////////////////////////////////////////////////
// CSettings settings lookup

CSettings::Item* CSettings::GetSetting(LPCTSTR pszName) const
{
	for ( POSITION pos = m_pItems.GetHeadPosition() ; pos ; )
	{
		Item* pItem = (Item*)m_pItems.GetNext( pos );
		if ( pItem->m_sName.CompareNoCase( pszName ) == 0 ) return pItem;
	}
	
	return NULL;
}

CSettings::Item* CSettings::GetSetting(LPVOID pValue) const
{
	for ( POSITION pos = m_pItems.GetHeadPosition() ; pos ; )
	{
		Item* pItem = (Item*)m_pItems.GetNext( pos );
		if ( pItem->m_pDword == pValue ||
			 pItem->m_pFloat == pValue ||
			 pItem->m_pString == pValue ) return pItem;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CSettings::Item construction and operations

CSettings::Item::Item(LPCTSTR pszName, DWORD* pDword, DOUBLE* pFloat, CString* pString)
{
	m_sName		= pszName;
	m_pDword	= pDword;
	m_pFloat	= pFloat;
	m_pString	= pString;
}

void CSettings::Item::Load()
{
	CRegistry pRegistry;
	
	int nPos = m_sName.Find( '.' );
	if ( nPos < 0 ) return;

	if ( m_pDword )
	{
		*m_pDword = pRegistry.GetDword( m_sName.Left( nPos ), m_sName.Mid( nPos + 1 ), *m_pDword );
	}
	else if ( m_pFloat )
	{
		*m_pFloat = pRegistry.GetFloat( m_sName.Left( nPos ), m_sName.Mid( nPos + 1 ), *m_pFloat );
	}
	else
	{
		*m_pString = pRegistry.GetString( m_sName.Left( nPos ), m_sName.Mid( nPos + 1 ), *m_pString );
	}

}

void CSettings::Item::Save()
{
	CRegistry pRegistry;
	
	int nPos = m_sName.Find( '.' );
	if ( nPos < 0 ) return;

	if ( m_pDword )
	{
		pRegistry.SetInt( m_sName.Left( nPos ), m_sName.Mid( nPos + 1 ), *m_pDword );
	}
	else if ( m_pFloat )
	{
		CString str;
		str.Format( _T("%e"), *m_pFloat );
		pRegistry.SetString( m_sName.Left( nPos ), m_sName.Mid( nPos + 1 ), str );
	}
	else
	{
		pRegistry.SetString( m_sName.Left( nPos ), m_sName.Mid( nPos + 1 ), *m_pString );
	}
}