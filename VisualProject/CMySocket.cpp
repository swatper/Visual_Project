// CMySocket.cpp: 구현 파일
//

#include "pch.h"
#include "VisualProject.h"
#include "CMySocket.h"
#include "VisualProjectDlg.h"

// CMySocket

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}


// CMySocket 멤버 함수
void CMySocket::SetParent(CDialog* pWnd)
{
	m_pWnd = pWnd;
}

void CMySocket::OnAccept(int nErrorCode)
{
	if (nErrorCode == 0)
		((CVisualProjectDlg*)m_pWnd)->OnAccept();
}

void CMySocket::OnConnect(int nErrorCode)
{
	if (nErrorCode == 0)
		((CVisualProjectDlg*)m_pWnd)->OnConnect();
}

void CMySocket::OnReceive(int nErrorCode)
{
	if (nErrorCode == 0)
		((CVisualProjectDlg*)m_pWnd)->OnReceive();
}

void CMySocket::OnClose(int nErrorCode)
{
	if (nErrorCode == 0)
		((CVisualProjectDlg*)m_pWnd)->OnClose();
}

void CMySocket::OnSend(int nErrorCode)
{
	if (nErrorCode == 0)
		((CVisualProjectDlg*)m_pWnd)->OnSend();
}
