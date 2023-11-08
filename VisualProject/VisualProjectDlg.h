
// VisualProjectDlg.h: 헤더 파일
//

#pragma once
#include "CMySocket.h"
#include "LOGIN_DIALOG.h"
#include "Blocks.h"
#include "Player.h"
#include <stdlib.h>
#include <time.h>

#define BOARDSIZE 47

// CVisualProjectDlg 대화 상자
class CVisualProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	CVisualProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VISUALPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int User_Type = -1;					  //유저 유형: 방장 | 유저
	CString Get_Name;					  //유저 이름 (로그인 Dialong에서 가져옴)
	CMySocket Serv_Socket, Client_Socket; //소켓 생성
	void OnAccept();
	void OnConnect();
	void OnClose();
	void OnReceive();
	void OnSend();
	CString Serv_Address;
	afx_msg void OnBnClickedOpenRoom();
	afx_msg void OnBnClickedConnectRoom();
	CListBox Player_List;
	afx_msg void OnBnClickedDiceRoll();
	Blocks* board = new Blocks[BOARDSIZE];
	Player player1;
	Player player2;
	bool playerTurn;	//true = 플레이어 1, false = 플레이어 2 차례

	afx_msg void OnBnClickedGameStart();
};
