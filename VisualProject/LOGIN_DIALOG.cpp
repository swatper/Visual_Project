// LOGIN_DIALOG.cpp: 구현 파일
//

#include "pch.h"
#include "VisualProject.h"
#include "LOGIN_DIALOG.h"
#include "afxdialogex.h"


// LOGIN_DIALOG 대화 상자

IMPLEMENT_DYNAMIC(LOGIN_DIALOG, CDialog)

LOGIN_DIALOG::LOGIN_DIALOG(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIA_Login, pParent)
	, Set_Name(_T("Guest"))
	, User_Type(0)
{

}

LOGIN_DIALOG::~LOGIN_DIALOG()
{
}

void LOGIN_DIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SET_NAME, Set_Name);
	DDX_Radio(pDX, IDC_MAKE_ROOM, User_Type);
}


BEGIN_MESSAGE_MAP(LOGIN_DIALOG, CDialog)
	ON_BN_CLICKED(IDCANCEL, &LOGIN_DIALOG::OnBnClickedCancel)
	ON_BN_CLICKED(ID_EXIT, &LOGIN_DIALOG::OnBnClickedExit)
END_MESSAGE_MAP()


// LOGIN_DIALOG 메시지 처리기


void LOGIN_DIALOG::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
	UpdateData(TRUE);
	UpdateData(FALSE);
	Login_Success = TRUE;
	OnOK();
}


void LOGIN_DIALOG::OnBnClickedExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Login_Success = FALSE;
	OnOK();
}
