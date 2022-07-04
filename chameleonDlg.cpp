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
#include "constant.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)

// CchameleonDlg 对话框

CchameleonDlg* MainDlg;

CchameleonDlg::CchameleonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAMELEON_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchameleonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, initButton);
	DDX_Control(pDX, IDC_EDIT1, Console);
	DDX_Control(pDX, IDC_BUTTON2, changeNameBtn);
	DDX_Control(pDX, IDC_EDIT7, _skill_size);
}

BEGIN_MESSAGE_MAP(CchameleonDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//	ON_BN_CLICKED(IDC_BUTTON1, &CchameleonDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON1, &CchameleonDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CchameleonDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON3, &CchameleonDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CchameleonDlg::OnBnClickedButton4)
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
	}
}


void CchameleonDlg::OnBnClickedButton2()
{
	Log(L"修改角色名");
	_DNF->changeUserName();
}

void CchameleonDlg::Log(wchar_t* msg)
{
	Console.SetSel(-1);
	Console.ReplaceSel(L"\r\n");
	Console.ReplaceSel(msg);
}


void CchameleonDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Log(L"清理残留数据中...");

	BOOL uninstallResult;
	wchar_t szFileName[MAX_PATH] = L"C:\\Randw.sys";
	// 停止并删除服务
	uninstallResult = SystemServiceOperate(szFileName, 2);
	if (FALSE == uninstallResult)
	{
		Log(L"停止驱动失败");
	}
	else {
		Log(L"停止驱动成功");
	}
	uninstallResult = SystemServiceOperate(szFileName, 3);
	if (FALSE == uninstallResult)
	{
		Log(L"删除驱动失败");
	}
	else {
		Log(L"删除驱动成功");
	}

	CDialogEx::OnClose();
}


void CchameleonDlg::OnBnClickedButton3()
{
	Log(L"修改三速");
	_DNF->threeSpeed(true);
}


void CchameleonDlg::OnBnClickedButton4()
{
	// 测试call
	//_DNF->summonMonster(56031);
	//_DNF->summonFigure(5308);
	//_DNF->hiddenUser();
	//_DNF->skillCall(_DNF->readLong(C_USER), 51200, 999999, 500, 500, 0, 0);
	
	// 获取技能代码
	CString skillCodeText;
	_skill_size.GetWindowText(skillCodeText);
	int skillCode = _ttoi(skillCodeText);
	_DNF->skillCall(_DNF->readLong(C_USER), skillCode, 1, 300, 300, 0, 0);
}
