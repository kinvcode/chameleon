// PAGE3.cpp: 实现文件
//

#include "pch.h"
#include "chameleon.h"
#include "PAGE3.h"
#include "afxdialogex.h"


// PAGE3 对话框

IMPLEMENT_DYNAMIC(PAGE3, CDialogEx)

PAGE3::PAGE3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAGE3, pParent)
{

}

PAGE3::~PAGE3()
{
}

void PAGE3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _attack_speed);
	DDX_Control(pDX, IDC_EDIT9, _casting_speed);
	DDX_Control(pDX, IDC_EDIT10, _move_speed);
	DDX_Control(pDX, IDC_EDIT11, _cool_down);
	DDX_Control(pDX, IDC_EDIT12, _damage_value);
}


BEGIN_MESSAGE_MAP(PAGE3, CDialogEx)
END_MESSAGE_MAP()


// PAGE3 消息处理程序
BOOL PAGE3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_attack_speed.SetWindowText(L"1000");
	_casting_speed.SetWindowText(L"2000");
	_move_speed.SetWindowText(L"1000");

	_cool_down.SetWindowText(L"70");
	_damage_value.SetWindowText(L"196688");

	return TRUE;
}