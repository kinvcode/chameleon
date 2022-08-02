// PAGE2.cpp: 实现文件
//

#include "pch.h"
#include "chameleon.h"
#include "PAGE2.h"
#include "afxdialogex.h"
#include "constant.h"

// PAGE2 对话框

IMPLEMENT_DYNAMIC(PAGE2, CDialogEx)

PAGE2::PAGE2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAGE2, pParent)
{

}

PAGE2::~PAGE2()
{
}

void PAGE2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, _ctl_list);
}


BEGIN_MESSAGE_MAP(PAGE2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &PAGE2::OnBnClickedButton1)
END_MESSAGE_MAP()


// PAGE2 消息处理程序

BOOL PAGE2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_ctl_list.InsertColumn(0, TEXT("怪物阵营"), LVCFMT_CENTER, 100);
	_ctl_list.InsertColumn(1, TEXT("怪物类型"), LVCFMT_CENTER, 100);
	_ctl_list.InsertColumn(2, TEXT("怪物代码"), LVCFMT_CENTER, 100);
	_ctl_list.InsertColumn(3, TEXT("怪物名称"), LVCFMT_CENTER, 200);
	_ctl_list.InsertColumn(4, TEXT("怪物血量"), LVCFMT_CENTER, 200);
	_ctl_list.InsertColumn(5, TEXT("怪物坐标"), LVCFMT_CENTER, 200);



	return TRUE;
}

void PAGE2::OnBnClickedButton1()
{
	_ctl_list.DeleteAllItems();
	// 遍历怪物
	if (_DNF->judgeGameStatus() != 3)
	{
		return;
	}

	__int64 head_address = _DNF->readLong(_DNF->readLong(_DNF->readLong(_DNF->readLong(_DNF->C_USER) + C_MAP_OFFSET) + 16) + C_MAP_HEAD);
	__int64 end_address = _DNF->readLong(_DNF->readLong(_DNF->readLong(_DNF->readLong(_DNF->C_USER) + C_MAP_OFFSET) + 16) + C_MAP_END);
	int item_quantity = 0;
	int row = 0;

	int monster_quantity = (int)(end_address - head_address) / 24;
	for (__int64 i = 1; i <= monster_quantity; i++)
	{
		__int64 monster_address = _DNF->readLong(_DNF->readLong(head_address + i * 24) + 16) - 32;
		int monster_camp = _DNF->readInt(monster_address + C_CAMP_OFFSET);
		int monster_type = _DNF->readInt(monster_address + C_TYPE_OFFSET);
		int monster_code = _DNF->readInt(monster_address + C_CODE_OFFSET);
		int monster_blood = _DNF->readInt(monster_address + C_MONSTER_BLOOD);
		CStringA monster_name = _DNF->Unicode2Ansi(_DNF->readByteArray(_DNF->readLong(monster_address + C_NAME_OFFSET), 50));

		if (monster_name.IsEmpty() && monster_code == 0) {
			continue;
		}

		COORDINATE monster_coor = _DNF->readCoordinate(monster_address);

		CString str_camp, str_type, str_code, str_blood, str_name, str_coor;
		str_camp.Format(_T("%d"), monster_camp);
		str_type.Format(_T("%d"), monster_type);
		str_code.Format(_T("%d"), monster_code);
		str_blood.Format(_T("%d"), monster_blood);
		str_coor.Format(_T("%d,%d,%d"), monster_coor.x, monster_coor.y, monster_coor.z);
		_ctl_list.InsertItem(row, str_camp);
		_ctl_list.SetItemText(row, 1, str_type);
		_ctl_list.SetItemText(row, 2, str_code);
		_ctl_list.SetItemText(row, 3, (CString)monster_name);
		_ctl_list.SetItemText(row, 4, str_blood);
		_ctl_list.SetItemText(row, 5, str_coor);
		row++;


		_DNF->handleEvents();
	}
}
