//
// Xmaster.h
//

#if !defined(AFXXMASTER_H__0E9E774A_836F_409F_B01B_8CA215E1F411__INCLUDED_)
#define AFXXMASTER_H__0E9E774A_836F_409F_B01B_8CA215E1F411__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"       // main symbols

class CMainWnd;

/////////////////////////////////////////////////////////////////////////////
// CXmasterApp:

class CXmasterApp : public CWinApp
{
// Construction
public:
	CXmasterApp();

// Attributes
public:
	CMutex				m_pMutex;
	CMutex				m_pSection;
	WORD				m_nVersion[4];

	_ConnectionPtr		m_pConnection;
	BOOL				m_bInitialized;
protected:
	CCriticalSection	m_csMessage;
	static TCHAR		szMessageBuffer[16384];

// Operations
public:
	void		Message(int nType, UINT nID, ...);
	void		Message(int nType, LPCTSTR pszFormat, ...);
	CString		GetErrorString();
protected:
	BOOL		SetFirstInstance();
	void		GetVersionNumber();
protected:
	BOOL		OpenDatabase();
	BOOL		CloseDatabase();
protected:
	void		PrintMessage(int nType, LPCTSTR pszLog);
	void		LogMessage(LPCTSTR pszLog, BOOL bShowTime = TRUE);

// Overrides
public:
	//{{AFX_VIRTUAL(CXmasterApp)
	public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();
	virtual void	WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation
public:
	//{{AFX_MSG(CXmasterApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CXmasterApp theApp;

//
// Registry
//

extern const TCHAR	gcszCopyright[];
extern const TCHAR	gcszWindowName[];
extern const TCHAR	gcszWindowClass[];

//
// Utility Functions
//

BOOL LoadString(CString& str, UINT nID);
LPCTSTR _tcsistr(LPCTSTR pszString, LPCTSTR pszPattern);
LPCTSTR _tcsnistr(LPCTSTR pszString, LPCTSTR pszPattern, DWORD plen);
void Replace(CString& strBuffer, LPCTSTR pszFind, LPCTSTR pszReplace);
CString ToHex(BYTE* pBuffer, DWORD nLength);
CString ToASCII(BYTE* pBuffer, DWORD nLength);

//
// User Messages
//

#ifdef _DEBUG
#define MLOG(x) theApp.Message( MSG_DEBUG, x )
#else
#define MLOG(x)
#endif

#define MSG_DEFAULT		0
#define MSG_SYSTEM		1
#define MSG_ERROR		2
#define MSG_DEBUG		3
#define MSG_TEMP		4

#define WM_LOG			(WM_USER+109)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFXXMASTER_H__0E9E774A_836F_409F_B01B_8CA215E1F411__INCLUDED_)
