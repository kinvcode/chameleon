﻿#pragma once


// PAGE3 对话框

class PAGE3 : public CDialogEx
{
	DECLARE_DYNAMIC(PAGE3)

public:
	PAGE3(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PAGE3();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAGE3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CEdit _attack_speed;
	CEdit _casting_speed;
	CEdit _move_speed;
	CEdit _cool_down;
	CEdit _damage_value;
};
