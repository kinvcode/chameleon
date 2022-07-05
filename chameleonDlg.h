
// chameleonDlg.h: 头文件
//

#pragma once
#include "dnf.h"


// CchameleonDlg 对话框
class CchameleonDlg : public CDialogEx
{
// 构造
public:
	CchameleonDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAMELEON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// 初始化按钮
	CButton initButton;
	
	// 初始化按钮点击事件
	afx_msg void OnBnClickedButton1();
	
	// 控制台控件
	CEdit Console;


	// 游戏
public:
	//PID
	DNF* _DNF;
	CButton changeNameBtn;
	afx_msg void OnBnClickedButton2();
	void Log(wchar_t* msg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CEdit _skill_size;
	CEdit _user_name;
	CEdit _attack_speed;
	CEdit _skill_code;
	CEdit _skill_damage;
	CEdit _casting_speed;
	CEdit _move_speed;
};
