//
// Transfer.cpp
//

#include "stdafx.h"
#include "Xmaster.h"
#include "Transfer.h"
#include "Settings.h"
#include "Network.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CTransfer construction

CTransfer::CTransfer(SOCKET hSocket, SOCKADDR_IN* pHost) : CConnection( hSocket, pHost )
{
	m_bConnected	= FALSE;
	
	Network.Add( this );
}

CTransfer::CTransfer()
{
	Network.Add( this );
}

CTransfer::~CTransfer()
{
	ASSERT( m_hSocket == INVALID_SOCKET );
	
	Network.Remove( this );
}

//////////////////////////////////////////////////////////////////////
// CTransfer connect

BOOL CTransfer::ConnectTo(LPCTSTR pAddress, int nPort)
{
	DWORD nAddress = inet_addr( pAddress );
	CConnection::ConnectTo( (IN_ADDR*)&nAddress, nPort );
	
	WSAEventSelect( m_hSocket, Network.m_pWakeup, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE );
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTransfer disconnect

void CTransfer::Disconnect()
{
	if ( IsConnected() ) SendPacket( new CByePacket );
	
	CConnection::Close();
}

//////////////////////////////////////////////////////////////////////
// CTransfer run function

BOOL CTransfer::OnRun()
{
	if ( ! CConnection::OnRun() ) return FALSE;
	
	DWORD tNow = GetTickCount();
	
	if ( ! IsConnected() && tNow - m_tConnected >= 15* 1000 )
	{
		OnDropped( WSA_WAIT_TIMEOUT ); return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTransfer socket event handlers

void CTransfer::OnDropped(int nError)
{
	CString strHost = inet_ntoa( m_pHost.sin_addr );
	if ( nError ) theApp.Message( MSG_ERROR, _T("WARNING: Dropped Connection From %s, Error:=%i"), strHost, nError );
	
	Disconnect();
}

BOOL CTransfer::OnHandshake()
{
	m_bConnected	= TRUE;
	m_tConnected	= GetTickCount();
	
	return TRUE;
}

BOOL CTransfer::OnRead()
{
	if ( ! CConnection::OnRead() ) return FALSE;
	
	CPacket* pPacket = ReadBuffer( m_pInput );
	if ( pPacket == NULL ) return TRUE;
	
	BOOL bResult = OnPacket( pPacket );
	pPacket->Release();
	
	return bResult;
}

//////////////////////////////////////////////////////////////////////
// CTransfer packet unserialize

void CTransfer::DoPrint(BYTE* pBuffer, DWORD nLength, LPCTSTR lpszHeader)
{
	if ( ! Settings.General.Debug ) return;
	
	CString strLine = lpszHeader;
	strLine += ToASCII( (BYTE*)pBuffer, nLength );
	
	theApp.Message( MSG_DEBUG, (LPCTSTR)strLine );
}

//////////////////////////////////////////////////////////////////////
// CTransfer packet event handlers

BOOL CTransfer::OnPacket(CPacket* pPacket)
{
	if ( pPacket->IsCommandId( "OK" ) )
	{
		return OnOkPacket( (COkPacket*)pPacket );
	}
	else
	if ( pPacket->IsCommandId( "ERR" ) )
	{
		return OnErrPacket( (CErrPacket*)pPacket );
	}
	else
	if ( pPacket->IsCommandId( "BYE" ) )
	{
		return OnByePacket( (CByePacket*)pPacket );
	}
	
	return TRUE;
}

BOOL CTransfer::OnOkPacket(COkPacket* pPacket)
{
	return TRUE;
}

BOOL CTransfer::OnErrPacket(CErrPacket* pPacket)
{
	Disconnect(); return FALSE;
}

BOOL CTransfer::OnByePacket(CByePacket* pPacket)
{
	Disconnect(); return FALSE;
}

//////////////////////////////////////////////////////////////////////
// CTransfer send packet

BOOL CTransfer::SendPacket(const void* pData, DWORD nLength)
{
	m_pOutput->Add( pData, nLength );
	CConnection::OnWrite();

	return TRUE;
}

BOOL CTransfer::SendPacket(CPacket* pPacket)
{
	pPacket->EncodeBody( m_pOutput );
	CConnection::OnWrite();
	pPacket->Release();

	return TRUE;
}

void CTransfer::SendFailPacket(int nErrCode)
{
	SendPacket( new CErrPacket( nErrCode ) );
}
