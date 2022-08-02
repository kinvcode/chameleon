
// chameleonDlg.h: 头文件
//

#pragma once
#include "dnf.h"
#include "PAGE1.h"
#include "PAGE2.h"
#include "PAGE3.h"


// CchameleonDlg 对话框
class CchameleonDlg : public CDialogEx
{
// 构造
public:
	CchameleonDlg(CWnd* pParent = nullptr);	// 标准构造函数

	HANDLE msdk_handle; // 按键句柄

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAMELEON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CDialog* _tab_box[3];
	int _cur_tab_index;


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void initTabCtl();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	PAGE1 page1;
	PAGE2 page2;
	PAGE3 page3;
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
	//CButton changeNameBtn;
	//afx_msg void OnBnClickedButton2();
	void Log(wchar_t* msg);
	void Log(CString msg);
	afx_msg void OnClose();
	//afx_msg void OnBnClickedButton3();
	//afx_msg void OnBnClickedButton4();
	//CEdit _skill_size;
	//CEdit _user_name;
	//CEdit _attack_speed;
	//CEdit _skill_code;
	//CEdit _skill_damage;
	//CEdit _casting_speed;
	//CEdit _move_speed;
	//afx_msg void OnBnClickedButton5();
	//CButton _switch_three_speed;
	//CButton _switch_skill_call;
	//CButton _switch_hidden_user;
	//CButton _switch_score;
	//CButton _switch_gather_items;
	//CEdit _cool_down;
	//CButton _switch_cool_down;
	//afx_msg void OnEnChangeEdit9();
	//afx_msg void OnBnClickedButton6();
	//afx_msg void OnBnClickedButton7();
	//CEdit _damage_value;
	//CButton _switch_hook_damage;
	//afx_msg void OnBnClickedCheck7();
	//afx_msg void OnBnClickedCheck1();
	//afx_msg void OnBnClickedCheck6();
	//afx_msg void OnBnClickedButton8();
	CTabCtrl _ctl_tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};
