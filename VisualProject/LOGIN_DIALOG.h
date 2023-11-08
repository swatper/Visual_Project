#pragma once


// LOGIN_DIALOG 대화 상자

class LOGIN_DIALOG : public CDialog
{
	DECLARE_DYNAMIC(LOGIN_DIALOG)

public:
	LOGIN_DIALOG(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~LOGIN_DIALOG();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIA_Login };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	BOOL Login_Success = FALSE;
	afx_msg void OnBnClickedExit();
	CString Set_Name;
	int User_Type;
};
