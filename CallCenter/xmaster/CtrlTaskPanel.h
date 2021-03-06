//
// CtrlTaskPanel.h
//

#if !defined(AFX_CTRLTASKPANEL_H__509DBC3E_C56B_4A11_993A_3F227C6D9621__INCLUDED_)
#define AFX_CTRLTASKPANEL_H__509DBC3E_C56B_4A11_993A_3F227C6D9621__INCLUDED_

#pragma once

class CTaskBox;


class CTaskPanel : public CWnd
{
// Construction
public:
	CTaskPanel();
	virtual ~CTaskPanel();

	DECLARE_DYNAMIC(CTaskPanel)

// Attributes
protected:
	CPtrList	m_pBoxes;
	CTaskBox*	m_pStretch;
	int			m_nMargin;
	int			m_nCurve;
	CBitmap		m_bmWatermark;
	CBitmap		m_bmFooter;
	BOOL		m_bLayout;
	
// Operations
public:
	CTaskBox*	AddBox(CTaskBox* pBox, POSITION posBefore = NULL);
	POSITION	GetBoxIterator() const;
	CTaskBox*	GetNextBox(POSITION& pos) const;
	int			GetBoxCount() const;
	void		RemoveBox(CTaskBox* pBox);
	void		ClearBoxes(BOOL bDelete);
public:
	void		SetStretchBox(CTaskBox* pBox);
	void		SetMargin(int nMargin, int nCurve = 2);
	void		SetWatermark(HBITMAP hBitmap);
	void		SetFooter(HBITMAP hBitmap, BOOL bDefault = TRUE);
	void		OnChanged();
protected:
	void		Layout(CRect& rcClient);
	
// Overrides
public:
	//{{AFX_VIRTUAL(CTaskPanel)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CTaskPanel)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CTaskBox;
};

class CXMLElement;

class CTaskBox : public CWnd
{
// Construction
public:
	CTaskBox();
	virtual ~CTaskBox();

	DECLARE_DYNAMIC(CTaskBox)

// Attributes
private:
	CTaskPanel*	m_pPanel;
	int			m_nHeight;
	BOOL		m_bVisible;
	BOOL		m_bOpen;
	BOOL		m_bHover;
	BOOL		m_bPrimary;
	HICON		m_hIcon;
	BOOL		m_bIconDel;
protected:
	CBitmap		m_bmWatermark;
	CBitmap		m_bmCaptionmark;
	BOOL		m_bCaptionCurve;
	
// Operations
public:
	CTaskPanel*	GetPanel() const;
	void		SetCaption(LPCTSTR pszCaption);
	void		SetIcon(HICON hIcon, BOOL bDelete = FALSE);
	void		SetSize(int nHeight);
	void		SetPrimary(BOOL bPrimary = TRUE);
	void		SetWatermark(HBITMAP hBitmap);
	void		SetCaptionmark(HBITMAP hBitmap, BOOL bDefault = TRUE);
	void		Expand(BOOL bOpen = TRUE);
protected:
	int			GetOuterHeight() const;
	void		PaintBorders();
	void		InvalidateNonclient();

	virtual void OnExpanded(BOOL bOpen);
	
// Inlines
public:
	virtual BOOL LoadReport(CXMLElement* pXML) {return TRUE;}
	virtual BOOL LoadReport(CXMLElement* pXML, BOOL bCreate) {return TRUE;}

// Overrides
public:
	//{{AFX_VIRTUAL(CTaskBox)
	public:
	virtual BOOL Create(CTaskPanel* pPanel, int nHeight = 0, LPCTSTR pszCaption = NULL, UINT nIDIcon = 0);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CTaskBox)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CTaskPanel;
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CTRLTASKPANEL_H__509DBC3E_C56B_4A11_993A_3F227C6D9621__INCLUDED_)
