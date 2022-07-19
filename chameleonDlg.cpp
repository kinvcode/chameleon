// chameleonDlg.cpp: 实现文件
//
#include "pch.h"
#include "framework.h"
#include "chameleon.h"
#include "chameleonDlg.h"
#include "afxdialogex.h"
#include "ReadWrite.h"
#include "dnf.h"
#include "loadDriver.h"
#include "msdk.h"
#include "UsbHidKeyCode.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)

// CchameleonDlg 对话框

CchameleonDlg* MainDlg;

CchameleonDlg::CchameleonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAMELEON_DIALOG, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDIGNORE);
}

void CchameleonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, initButton);
	DDX_Control(pDX, IDC_EDIT1, Console);
	DDX_Control(pDX, IDC_BUTTON2, changeNameBtn);
	DDX_Control(pDX, IDC_EDIT7, _skill_size);
	DDX_Control(pDX, IDC_EDIT2, _user_name);
	DDX_Control(pDX, IDC_EDIT4, _attack_speed);
	DDX_Control(pDX, IDC_EDIT8, _skill_code);
	DDX_Control(pDX, IDC_EDIT6, _skill_damage);
	DDX_Control(pDX, IDC_EDIT3, _casting_speed);
	DDX_Control(pDX, IDC_EDIT5, _move_speed);
	DDX_Control(pDX, IDC_CHECK1, _switch_three_speed);
	DDX_Control(pDX, IDC_CHECK2, _switch_skill_call);
	DDX_Control(pDX, IDC_CHECK4, _switch_hidden_user);
	DDX_Control(pDX, IDC_CHECK3, _switch_score);
	DDX_Control(pDX, IDC_CHECK5, _switch_gather_items);
	DDX_Control(pDX, IDC_EDIT9, _cool_down);
	DDX_Control(pDX, IDC_CHECK6, _switch_cool_down);
	DDX_Control(pDX, IDC_EDIT10, _damage_value);
	DDX_Control(pDX, IDC_CHECK7, _switch_hook_damage);
}

BEGIN_MESSAGE_MAP(CchameleonDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CchameleonDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CchameleonDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON3, &CchameleonDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CchameleonDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CchameleonDlg::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_EDIT9, &CchameleonDlg::OnEnChangeEdit9)
	ON_BN_CLICKED(IDC_BUTTON6, &CchameleonDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CchameleonDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_CHECK7, &CchameleonDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK1, &CchameleonDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK6, &CchameleonDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_BUTTON8, &CchameleonDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CchameleonDlg 消息处理程序

BOOL CchameleonDlg::OnInitDialog()
{
	MainDlg = this;
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	SetWindowText(L"变色龙");

	// 初始化数值
	_attack_speed.SetWindowText(L"1000");
	_casting_speed.SetWindowText(L"2000");
	_move_speed.SetWindowText(L"1000");

	_cool_down.SetWindowText(L"80");

	_damage_value.SetWindowText(L"196688");

	_switch_score.SetCheck(BST_CHECKED);
	_switch_three_speed.SetCheck(BST_CHECKED);
	_switch_gather_items.SetCheck(BST_CHECKED);
	_switch_cool_down.SetCheck(BST_CHECKED);
	_switch_hook_damage.SetCheck(BST_CHECKED);


	_user_name.SetWindowText(L"旭旭宝宝");

	// 初始化模拟按键
	msdk_handle = M_Open(1);
	if (msdk_handle == INVALID_HANDLE_VALUE) {
		Log(L"模拟按键开启失败");
	}

	Log(L"请启动游戏然后点击初始化");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CchameleonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CchameleonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CchameleonDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	// 获取DNF进程ID
	int PID = ReadWrite::TakePid(L"DNF.exe");
	if (PID == 0) {
		Log(L"请先运行游戏");
		//AfxMessageBox(L"");
		return;
	}

	// 初始化DNF辅助对象
	_DNF = new DNF(PID);

	// 根据读取数值判断，是否有读写权限

	// 打印int数值例子
	//int res = _DNF->readInt(0x140000000);
	//CString message;
	//message.Format(_T("%d"), res);
	//Console.SetWindowText(message);

	int res = _DNF->readInt(0x140000000);
	if (_DNF->readInt(0x140000000) != 0x905A4D) {
		Log(L"无读写权限，请获取图标");
		return;
	}
	else {
		Log(L"初始化成功!");

		// 启动读取人物基址线程
		_DNF->userPonterUpdate();
		// 启动手动线程
		//_DNF->manualThreadControl();
		_DNF->autoThreadControl();
	}
}


void CchameleonDlg::OnBnClickedButton2()
{
	// 获取角色名
	CString name;
	CStringA userName;
	_user_name.GetWindowText(name);
	if (name.IsEmpty()) {
		Log(L"角色名为空，不进行修改");
		return;
	}
	userName = name;
	_DNF->changeUserName(userName);
}

void CchameleonDlg::Log(wchar_t* msg)
{
	Console.SetSel(-1);
	Console.ReplaceSel(msg);
	Console.SetSel(-1);
	Console.ReplaceSel(L"\r\n");
}

void CchameleonDlg::Log(CString msg)
{
	Console.SetSel(-1);
	Console.ReplaceSel(msg);
	Console.SetSel(-1);
	Console.ReplaceSel(L"\r\n");
}


void CchameleonDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Log(L"清理残留数据中...");

	// 关闭按键
	if (msdk_handle != INVALID_HANDLE_VALUE) {
		M_Close(msdk_handle);
		msdk_handle = INVALID_HANDLE_VALUE;
	}

	// 停止并删除读写驱动
	wchar_t sys_path[MAX_PATH];
	GetCurrentDirectory(sizeof(sys_path), sys_path);
	wcscat_s(sys_path, L"\\Randw.sys");

	BOOL uninstallResult;
	uninstallResult = SystemServiceOperate(sys_path, 2);
	if (FALSE == uninstallResult)
	{
		Log(L"停止驱动失败");
	}
	else {
		Log(L"停止驱动成功");
	}
	uninstallResult = SystemServiceOperate(sys_path, 3);
	if (FALSE == uninstallResult)
	{
		Log(L"删除驱动失败");
	}
	else {
		Log(L"删除驱动成功");
	}
	Sleep(1500);
	CDialogEx::OnClose();
}


void CchameleonDlg::OnBnClickedButton3()
{
	Log(L"修改三速");
	CString attack, casting, move;
	_attack_speed.GetWindowText(attack);
	_casting_speed.GetWindowText(casting);
	_move_speed.GetWindowText(move);

	_DNF->threeSpeed(_ttoi(attack), _ttoi(casting), _ttoi(move));
	MainDlg->Log(L"三速已开启");
}


void CchameleonDlg::OnBnClickedButton4()
{
	// 测试call
	//_DNF->summonMonster(56031);
	//_DNF->summonFigure(5308);
	//_DNF->hiddenUser();
	//_DNF->skillCall(_DNF->readLong(C_USER), 51200, 999999, 500, 500, 0, 0);

	// 获取技能代码
	CString skillCode, skillDamage, skillSize;
	_skill_code.GetWindowText(skillCode);
	_skill_damage.GetWindowText(skillDamage);
	_skill_size.GetWindowText(skillSize);


	_DNF->skillCall(_DNF->readLong(_DNF->C_USER), _ttoi(skillCode), _ttoi(skillDamage), 500, 250, 0, 0);
}


void CchameleonDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	// 测试模拟键鼠
	//if (msdk_handle == INVALID_HANDLE_VALUE) {
	//	Log(L"模拟按键未启动！");
	//	return;
	//}
	//unsigned int RetSw;
	//for (int i = 0; i < 10;i++) 
	//{
	//	unsigned int RetSw;
	//	RetSw = M_KeyPress(msdk_handle, Keyboard_a, 1);
	//}

	// 召唤仓库
	_DNF->skillCall(_DNF->readLong(_DNF->C_USER), 51200, 999999, 100, 100, 100, 1);
}


void CchameleonDlg::OnEnChangeEdit9()
{
	CString number;
	_cool_down.GetWindowText(number);

	int num = _ttoi(number);

	if (num < 0 || num > 80)
	{
		_cool_down.SetWindowText(L"50");
		Log(L"技能缩减范围：1~80");
	}

}


void CchameleonDlg::OnBnClickedButton6()
{
	CString number;
	_cool_down.GetWindowText(number);

	_DNF->encrypt(_DNF->readLong(_DNF->C_USER) + _DNF->C_FLOAT_COOL_DOWN2, _ttoi(number));
	Log(L"技能冷却已开启");
}


void CchameleonDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	// 获取人物坐标
	//COORDINATE user_coor;
	//__int64 coor;
	//user_coor = _DNF->readCoordinate(_DNF->readLong(_DNF->C_USER));

	//__int64 pass_room_data = _DNF->passRoomData(0);
	//coor = pass_room_data;
	//int start_x = _DNF->readInt(coor + 0);
	//int start_y = _DNF->readInt(coor + 4);
	//int end_x = _DNF->readInt(coor + 8);
	//int end_y = _DNF->readInt(coor + 12);

	//M_ReleaseAllKey(msdk_handle);

	Sleep(1000);
	_DNF->runToDestination(600, 150,false);
}


void CchameleonDlg::OnBnClickedCheck7()
{
	if (_switch_hook_damage.GetCheck() == BST_CHECKED)
	{
		_DNF->hookDamage(true);
		Log(L"HOOK伤害：开！");
	}
	else {
		_DNF->hookDamage(true);
		Log(L"HOOK伤害：关！");
	}
}


void CchameleonDlg::OnBnClickedCheck1()
{
	if (_switch_three_speed.GetCheck() == BST_CHECKED)
	{
		CString attack_speed, move_speed, casting_speed;
		_attack_speed.GetWindowText(attack_speed);
		_move_speed.GetWindowText(move_speed);
		_casting_speed.GetWindowText(casting_speed);
		_DNF->threeSpeed(_ttoi(attack_speed), _ttoi(casting_speed), _ttoi(move_speed));
		Log(L"三速：开！");
	}
	else {
		_DNF->threeSpeed(0, 0, 0);
		Log(L"三速：关！");
	}
}


void CchameleonDlg::OnBnClickedCheck6()
{
	if (_switch_cool_down.GetCheck() == BST_CHECKED)
	{
		CString num;
		_cool_down.GetWindowText(num);
		float number = (float)_ttof(num);
		_DNF->skillCoolDown(number);
		Log(L"技能冷却缩减：开！");
	}
	else {
		_DNF->skillCoolDown(0);
		Log(L"技能冷却缩减：关！");
	}
}


void CchameleonDlg::OnBnClickedButton8()
{
	Console.SetSel(0, -1);
	Console.ReplaceSel(_T(""));
	//Console.Clear();
	//UpdateData(FALSE);
	M_ReleaseAllKey(msdk_handle);
}
