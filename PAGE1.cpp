// PAGE1.cpp: 实现文件
//

#include "pch.h"
#include "chameleon.h"
#include "PAGE1.h"
#include "afxdialogex.h"


// PAGE1 对话框

IMPLEMENT_DYNAMIC(PAGE1, CDialogEx)

PAGE1::PAGE1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAGE1, pParent)
{

}

PAGE1::~PAGE1()
{
}

void PAGE1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, _switch_three_speed);
	DDX_Control(pDX, IDC_CHECK2, _switch_score);
	DDX_Control(pDX, IDC_CHECK4, _switch_gather_items);
	DDX_Control(pDX, IDC_CHECK3, _switch_hidden_user);
	DDX_Control(pDX, IDC_CHECK5, _switch_cool_down);
	DDX_Control(pDX, IDC_CHECK6, _switch_hook_damage);
}


BEGIN_MESSAGE_MAP(PAGE1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &PAGE1::OnBnClickedButton2)
END_MESSAGE_MAP()


// PAGE1 消息处理程序
BOOL PAGE1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_switch_score.SetCheck(BST_CHECKED);
	_switch_three_speed.SetCheck(BST_CHECKED);
	_switch_gather_items.SetCheck(BST_CHECKED);
	_switch_cool_down.SetCheck(BST_CHECKED);
	_switch_hook_damage.SetCheck(BST_CHECKED);
	_switch_hidden_user.SetCheck(BST_CHECKED);


	return TRUE;
}

void PAGE1::OnBnClickedButton2()
{
	if (penetrate) {
		_DNF->penetrate(false);
		penetrate = false;
	}
	else {
		_DNF->penetrate(true);
		penetrate = true;
	}
	// TODO: 在此添加控件通知处理程序代码
}
