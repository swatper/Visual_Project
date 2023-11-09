
// VisualProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "VisualProject.h"
#include "VisualProjectDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVisualProjectDlg 대화 상자



CVisualProjectDlg::CVisualProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VISUALPROJECT_DIALOG, pParent)
	, Get_Name(_T(""))
	, Serv_Address(_T("localhost"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVisualProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Get_Name);
	DDX_Text(pDX, IDC_SET_ADDRESS, Serv_Address);
	DDX_Control(pDX, IDC_PLAYER_LIST, Player_List);
}

BEGIN_MESSAGE_MAP(CVisualProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_ROOM, &CVisualProjectDlg::OnBnClickedOpenRoom)
	ON_BN_CLICKED(IDC_CONNECT_ROOM, &CVisualProjectDlg::OnBnClickedConnectRoom)
	ON_BN_CLICKED(IDC_DICE_ROLL, &CVisualProjectDlg::OnBnClickedDiceRoll)
	ON_BN_CLICKED(IDC_GAME_START, &CVisualProjectDlg::OnBnClickedGameStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVisualProjectDlg 메시지 처리기

BOOL CVisualProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	srand((unsigned)time(NULL));
	//로그인 Dialong 실행
	LOGIN_DIALOG LogIn;
	LogIn.DoModal();
	UpdateData(TRUE);
	Get_Name = LogIn.Set_Name;		//닉네임 가져오기
	UpdateData(FALSE);
	//로그인 성공(접속 버튼)시에만 실행
	if (LogIn.Login_Success == TRUE) {
		if (LogIn.User_Type == 0) {	//방장으로 접속
			GetDlgItem(IDC_CONNECT_ROOM)->EnableWindow(FALSE);
			GetDlgItem(IDC_DICE_ROLL)->EnableWindow(FALSE);
			User_Type = 0;
			Serv_Socket.SetParent(this);
			Client_Socket.SetParent(this);
		}
		else {				//유저(?)로 접속
			GetDlgItem(IDC_OPEN_ROOM)->EnableWindow(FALSE);
			GetDlgItem(IDC_GAME_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_DICE_ROLL)->EnableWindow(FALSE);
			User_Type = 1;
			Client_Socket.SetParent(this);
			Client_Socket.Create();
		}
		MessageBox(Get_Name + _T("님 환영합니다. "));
	}
	else {
		OnOK();
	}

	//맵 그리기
	int ver = 1;
	int hor = 1;
	int v = 1;
	for (int i = 0; i < BOARDSIZE; i++) {
		if (ver > 11 || ver <= 0) {
			v *= -1;
			ver += v;
			hor++;
			board[i].setCord(50 * (ver), 50 * hor);
			hor++;
		}
		else {
			board[i].setCord(50 * (ver), 50 * hor);
			ver += v;
		}
	}

	//특수 블록 생성
	for (int i = 0; i < 2; i++) {
		int specialBlock = rand() % (BOARDSIZE - 5) + 2;
		if (board[specialBlock].getBlockType() == 0) {
			board[specialBlock].setBlockType(1);
		}
	}


	//플레이어 초기 설정
	player1.SetI(0);
	player2.SetI(0);
	playerTurn = true; //1번 플레이어 먼저

	//주사위
	DICE_NUN = 0;
	animation_count = 0;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CVisualProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CVisualProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CClientDC dc(this);

		CBrush brush, * oldBrush;
		
		int r = 10;
		int pedding = 25;
		for (int i = 0; i < BOARDSIZE; i++) {
			int xPos = board[i].getX();
			int yPos = board[i].getY();
			if (i == 0) {
				brush.CreateSolidBrush(RGB(255, 0, 0));
				oldBrush = dc.SelectObject(&brush);
			}
			else if (i == BOARDSIZE - 1) {
				brush.CreateSolidBrush(RGB(0, 255, 0));
				oldBrush = dc.SelectObject(&brush);
			}
			else if (i == BOARDSIZE - 2) {
				brush.CreateSolidBrush(RGB(200, 0, 0));
				oldBrush = dc.SelectObject(&brush);
			}
			else if(board[i].getBlockType() == 1){
				brush.CreateSolidBrush(RGB(255, 255, 0));
				oldBrush = dc.SelectObject(&brush);
			}
			else {
				brush.CreateSolidBrush(RGB(204, 201, 231));
				oldBrush = dc.SelectObject(&brush);
			}
			
			dc.Rectangle(xPos + pedding, yPos + pedding, xPos - pedding, yPos - pedding);
			if (player1.getI() == i) {
				brush.CreateSolidBrush(RGB(255, 204, 0));
				dc.SelectObject(&brush);
				dc.Rectangle(xPos + r+5, yPos + r+5, xPos - (r+5), yPos - (r+5));
				dc.SelectObject(oldBrush);
			}
			if (player2.getI() == i) {
				brush.CreateSolidBrush(RGB(51, 255, 255));
				dc.SelectObject(&brush);
				dc.Ellipse(xPos + r, yPos + r, xPos - r, yPos - r);
				dc.SelectObject(oldBrush);
			}
		}
		dc.SelectObject(oldBrush);
		brush.DeleteObject();

		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap bitmap;
		bitmap.LoadBitmap(DICE_NUN); //BITMAP ID
		CBitmap* oldbitmap = MemDC.SelectObject(&bitmap);
		//출력 좌표x, y, 폭, 넓이, 넣을 BITMAP DC, 저장한 것이 어디서 시작하는지 좌표
		dc.BitBlt(730, 500, 48, 48, &MemDC, 0, 0, SRCCOPY);
		dc.SelectObject(oldbitmap);
		bitmap.DeleteObject();
		
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CVisualProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-----------------------------------------------소켓 관련 코드
void CVisualProjectDlg::OnAccept() {
	Serv_Socket.Accept(Client_Socket);
	//리스트박스에 플레이어 이름 추가
	char* pBuf = new char[1025];
	int iBufsize = 1024;
	Client_Socket.Receive((void*)pBuf, iBufsize);
	Player_List.AddString((LPCTSTR)pBuf);
}
//Connect 시도후에 OnConnect 호출
void CVisualProjectDlg::OnConnect() {

}
//소켓이 닫히면 실행
void CVisualProjectDlg::OnClose() {

}
//데이터를 받으면 실행
void CVisualProjectDlg::OnReceive() {

}

void CVisualProjectDlg::OnSend() {

}
//방 만들기
void CVisualProjectDlg::OnBnClickedOpenRoom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Serv_Socket.Create(4000); //포트 생성
	Serv_Socket.Listen();
}
//방 들어가기
void CVisualProjectDlg::OnBnClickedConnectRoom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);
	//서버(?)열기
	Client_Socket.Connect(Serv_Address, 4000);
	//닉네임 보내기
	int Name_length = Get_Name.GetLength() + 8;
	char* SendName = new char[Name_length];
	memcpy(SendName, (char*)(LPCTSTR)Get_Name, Name_length);
	Client_Socket.Send((void*)SendName, Name_length);
	
}

//주사위 굴리기
void CVisualProjectDlg::OnBnClickedDiceRoll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int dice_num = rand() % 6 + 1;
	DICE_NUN = IDB_DICE1 + dice_num - 1;
	Invalidate();
	if (playerTurn) {
		player1.SetI(player1.getI() + dice_num);
		if (player1.getI() >= BOARDSIZE - 1) {
			player1.SetI(BOARDSIZE - 1);
		}
		else if (player1.getI() == BOARDSIZE - 2) {
			MessageBox(_T("이런!!"));
			player1.SetI(0);
		}
		else if (board[player1.getI()].getBlockType() == 1) {
			MessageBox(_T("특수 블럭! 랜덤 위치"));
			player1.SetI(rand() % (BOARDSIZE - 5));
		}
	}
	else {
		player2.SetI(player2.getI() + dice_num);
		if (player2.getI() >= BOARDSIZE - 1) {
			player2.SetI(BOARDSIZE - 1);
		}
		else if (player2.getI() == BOARDSIZE - 2) {
			MessageBox(_T("이런!!"));
			player2.SetI(0);
		}
		else if (board[player2.getI()].getBlockType() == 1) {
			MessageBox(_T("특수 블럭! 랜덤 위치"));
			player2.SetI(rand() % (BOARDSIZE - 5));
		}
	}
	Invalidate();
	playerTurn = !playerTurn;
}


void CVisualProjectDlg::OnBnClickedGameStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_DICE_ROLL)->EnableWindow(TRUE);
	GetDlgItem(IDC_GAME_START)->EnableWindow(FALSE);
}


void CVisualProjectDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
}
