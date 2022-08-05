#include "pch.h"
#include "dnf.h"
#include "chameleonDlg.h"
#include "overloading.h"
#include "msdk.h"
#include "UsbHidKeyCode.h"
#include "constant.h"
#include <time.h>

extern CchameleonDlg* MainDlg;


DNF::DNF(DWORD PID)
{
	this->PID = PID;

	this->C_USER = C_USER_ADDRESS;
	this->C_USER_POINTER = 0;
}

DNF::~DNF()
{

}

std::vector<byte> DNF::Ansi2Unicode(CStringA str)
{
	std::vector<byte>Ret;
	DWORD dwNum = MultiByteToWideChar(936, 0, str, -1, NULL, 0);
	byte* pwText;
	pwText = new  byte[dwNum * 2];
	MultiByteToWideChar(936, 0, str, -1, (LPWSTR)pwText, dwNum * 2);

	for (size_t i = 0; i < dwNum * 2; i++)
	{
		Ret.push_back(pwText[i]);
	}
	Ret.push_back(0);
	Ret.push_back(0);
	return Ret;
}

CStringA DNF::Unicode2Ansi(std::vector<byte> vby)
{
	int length = (int)vby.size();
	byte* str = new byte[vby.size()]();
	for (int i = 0; i < length; ++i)
	{
		str[i] = vby[i];
	}
	length = length / 2;
	length = ::WideCharToMultiByte(CP_ACP, NULL, (LPCWCH)str, length, NULL, 0, NULL, NULL);
	char* resultCha = new char[length + 1]();
	::WideCharToMultiByte(CP_ACP, NULL, (LPCWCH)str, length, resultCha, length, NULL, NULL);
	CStringA result(resultCha);
	return result;
}

std::vector<byte> DNF::makeByteArray(std::vector<byte> data)
{
	return data;
}

std::vector<byte> DNF::makeEmptyByteArray(int length)
{
	std::vector<byte> data;
	for (int i = 0; i < length; i++)
	{
		data.push_back(NULL);
	}

	return data;
}


std::vector<byte> DNF::intToBytes(__int64 length) {
	__int64  adr = (__int64)&length;
	std::vector<byte>c;
	for (size_t i = 0; i < 8; i++) {
		c.push_back(*(byte*)adr++);
	}
	return c;
}

std::vector<byte> DNF::intToBytes2(int length) {
	__int64  adr = (__int64)&length;
	std::vector<byte>c;
	for (size_t i = 0; i < 4; i++) {
		c.push_back(*(byte*)adr++);
	}
	return c;
}

void DNF::handleEvents()
{
	CWinThread* pWinThread = AfxGetThread();
	if (pWinThread != NULL)
	{
		MSG msg;
		while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) && msg.message != WM_QUIT)
		{
			AfxGetThread()->PumpMessage();
		}
	}
}

bool DNF::programDelay(int time = 0, int delayUnit = 0)
{
	INT_PTR Interval = time, Unit = 0;
	if (!Interval && !Unit)
	{
		Interval = 5000 * 60 * 60;
		Unit = 5000 * 60 * 60;
	}
	else {
		switch (delayUnit)
		{
		case 0:
			Unit = 1;
			break;
		case 1:
			Unit = 1000;
			break;
		case 2:
			Unit = 1000 * 60;
			break;
		case 3:
			Unit = 1000 * 60 * 60;
			break;
		default:
			break;
		}
	}

	HANDLE handle[1];
	handle[0] = CreateWaitableTimerW(NULL, false, NULL);
	LARGE_INTEGER lpDueTime;
	lpDueTime.QuadPart = -10 * Interval * 1000 * Unit;
	SetWaitableTimer(handle[0], &lpDueTime, 0, NULL, NULL, false);
	DWORD nCount = sizeof(handle) / sizeof(HANDLE);
	while (MsgWaitForMultipleObjects(nCount, handle, false, -1, (QS_KEY | QS_MOUSEMOVE | QS_MOUSEBUTTON | QS_POSTMESSAGE | QS_TIMER | QS_PAINT | QS_SENDMESSAGE | QS_HOTKEY)) != WAIT_OBJECT_0)
	{
		// 系统处理事件
		MSG msg;
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CloseHandle(handle[0]);
	return true;
}


int DNF::readInt(__int64 address)
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	int value;
	bool res = ReadProcessMemory(handle, (void*)address, &value, 4, NULL);
	if (res)
	{
		return value;
	}
	CloseHandle(handle);
	return 0;
}

// 总是异常
__int64 DNF::readLong(__int64 address)
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	__int64 value;
	bool res = ReadProcessMemory(handle, (void*)address, &value, 8, NULL);
	if (ReadProcessMemory(handle, (void*)address, &value, 8, NULL))
	{
		return value;
	}
	CloseHandle(handle);
	return 0;
}

float DNF::readFloat(__int64 address)
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	float value;
	bool res = ReadProcessMemory(handle, (void*)address, &value, 4, NULL);
	if (res)
	{
		return value;
	}
	CloseHandle(handle);
	return 0;
}

std::vector<byte> DNF::readByteArray(__int64 address, int length)
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	byte* tempResult;
	tempResult = new byte[length];
	memset(tempResult, 0, length);
	ReadProcessMemory(handle, (LPCVOID)address, tempResult, length, NULL);
	std::vector<byte> result;
	result.resize(length);
	for (int i = 0; i < length; i++)
	{
		result[i] = tempResult[i];
	}

	CloseHandle(handle);
	return result;
}

bool DNF::writeByteArray(__int64 address, std::vector<byte> Data)
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	int length;
	length = (int)Data.size();
	byte* val = new byte[length];
	for (int i = 0; i < length; i++)
	{
		val[i] = Data[i];
	}
	bool res = WriteProcessMemory(handle, (LPVOID)address, (LPCVOID)val, length, NULL);
	CloseHandle(handle);
	return res;
}

bool DNF::writeInt(__int64 address, __int64 value)
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	bool res = WriteProcessMemory(handle, (void*)address, &value, 4, NULL);
	CloseHandle(handle);
	return res;
}

bool DNF::writeFloat(__int64 address, float value)
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	bool res = WriteProcessMemory(handle, (void*)address, &value, 4, NULL);
	CloseHandle(handle);
	return res;
}

bool DNF::writeLong(__int64 address, __int64 value)
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	bool res = WriteProcessMemory(handle, (void*)address, &value, 8, NULL);
	CloseHandle(handle);
	return res;
}

void DNF::encrypt(__int64 address, int value)
{
	__int64 data = value + 4;
	data = 0x1F2A025C ^ data;
	// 写长整数
	writeLong(address, data);
}

__int64 DNF::decrypt(__int64 address)
{
	__int64 data = readLong(address) ^ 0x1F2A025C;
	data -= 4;
	return data;
}

__int64 DNF::decrypt2(__int64 address)
{
	__int64 value = readInt(address);
	value = value ^ 0x1F2A025C & 0xffffffff;
	value -= 4;
	return value;
}

void DNF::memoryAssambly(std::vector<byte>asm_code)
{
	// HOOK数据和原始数据
	std::vector<byte>HOOKdata, HOOKraw;

	//异步执行
	bool asyncExecute = false;

	//汇编中转,空白地址,判断地址
	__int64 asmTransfer, emptyAddress, logicAddress, HOOKasm, HOOKret;

	asmTransfer = C_EMPTY_ADDRESS + 300;
	emptyAddress = C_EMPTY_ADDRESS + 500;
	logicAddress = emptyAddress - 100;

	if (asyncExecute) {
		return;
	}
	asyncExecute = true;

	HOOKasm = C_ASM_CALL;
	HOOKasm += 144;
	HOOKret = HOOKasm + 19;

	HOOKdata = readByteArray(HOOKasm, 19);
	HOOKraw = HOOKdata;

	HOOKdata = HOOKdata + makeByteArray({ 72,184 }) + intToBytes(logicAddress);
	HOOKdata = HOOKdata + makeByteArray({ 131, 56, 1, 117, 42, 72, 129, 236, 0, 3, 0, 0 });
	HOOKdata = HOOKdata + makeByteArray({ 72,187 }) + intToBytes(emptyAddress);
	HOOKdata = HOOKdata + makeByteArray({ 255, 211 });
	HOOKdata = HOOKdata + makeByteArray({ 72,184 }) + intToBytes(logicAddress);
	HOOKdata = HOOKdata + makeByteArray({ 199, 0, 3, 0, 0, 0 });
	HOOKdata = HOOKdata + makeByteArray({ 72, 129, 196, 0, 3, 0, 0 });
	HOOKdata = HOOKdata + makeByteArray({ 255, 37, 0, 0, 0, 0 }) + intToBytes(HOOKret);

	if (readInt(asmTransfer) == 0)
	{
		writeByteArray(asmTransfer, HOOKdata);
	}

	writeByteArray(emptyAddress, asm_code + makeByteArray({ 195 }));
	writeByteArray(HOOKasm, makeByteArray({ 255, 37, 0, 0, 0, 0 }) + intToBytes(asmTransfer) + makeByteArray({ 144, 144, 144, 144, 144 }));
	writeLong(logicAddress, 1);

	while (readLong(logicAddress) == 1)
	{
		Sleep(5);
		handleEvents();
	}

	writeByteArray(HOOKasm, HOOKraw);
	writeByteArray(emptyAddress, makeEmptyByteArray((int)asm_code.size() + 16));

	asyncExecute = false;
}

void DNF::changeUserName(CStringA name)
{
	__int64 userNameAddress = readLong(readLong(C_USER) + C_NAME_OFFSET);
	if (userNameAddress)
	{
		if (writeByteArray(userNameAddress, Ansi2Unicode(name))) {
			MainDlg->Log(L"修改名字成功");
		}
	}
}

void DNF::threeSpeed(int attack, int casting, int move)
{
	long long shoePointer = readLong(C_USER) + C_SHOE_OFFSET; // 鞋子指针

	encrypt(readLong(shoePointer) + C_ATTACK_SPEED, attack);
	encrypt(readLong(shoePointer) + C_MOVE_SPEED, move);
	encrypt(readLong(shoePointer) + C_CASTING_SPEED, casting);

}

void DNF::summonFigure(int code)
{
	std::vector<byte>asm_code;

	__int64 emptyAddress = C_EMPTY_ADDRESS + 2900;

	writeLong(emptyAddress, code);
	writeLong(emptyAddress + 4, 135);
	writeLong(emptyAddress + 8, 135);
	writeLong(emptyAddress + 12, 600000);
	writeLong(emptyAddress + 16, 1);

	asm_code = asm_code + makeByteArray({ 72, 131, 236, 32 });
	asm_code = asm_code + makeByteArray({ 73, 184 }) + intToBytes(emptyAddress);
	asm_code = asm_code + makeByteArray({ 72, 186 }) + intToBytes(C_USER);
	asm_code = asm_code + makeByteArray({ 72, 139, 18, 72, 185, 0, 0, 0, 0, 0, 0, 0, 0, 72, 184 }) + intToBytes(C_SUMMON_FIGURE);
	asm_code = asm_code + makeByteArray({ 255, 208, 72, 131, 196, 32 });

	memoryAssambly(asm_code);
}

void DNF::summonMonster(int code)
{
	std::vector<byte>asm_code;

	__int64 emptyAddress = C_EMPTY_ADDRESS + 2800;

	writeLong(emptyAddress, code);
	writeLong(emptyAddress + 4, 135);
	writeLong(emptyAddress + 8, 135);
	writeLong(emptyAddress + 12, 600000);
	writeLong(emptyAddress + 16, 1);

	asm_code = asm_code + makeByteArray({ 72, 131, 236, 32 });
	asm_code = asm_code + makeByteArray({ 73, 184 }) + intToBytes(emptyAddress);
	asm_code = asm_code + makeByteArray({ 72, 186 }) + intToBytes(C_USER);
	asm_code = asm_code + makeByteArray({ 72, 139, 18, 72, 185, 0, 0, 0, 0, 0, 0, 0, 0, 72, 184 }) + intToBytes(C_SUMMON_MONSTER);
	asm_code = asm_code + makeByteArray({ 255, 208, 72, 131, 196, 32 });

	memoryAssambly(asm_code);
}

void DNF::hiddenUser()
{
	__int64 target = readLong(C_USER);

	std::vector<byte>asm_code;
	asm_code = makeByteArray({ 72, 129, 236, 0, 2, 0, 0 });
	asm_code = asm_code + makeByteArray({ 65, 191, 255, 255, 255, 255 });
	asm_code = asm_code + makeByteArray({ 199, 68, 36, 32, 255, 255, 0, 0 });
	asm_code = asm_code + makeByteArray({ 65, 185, 1, 0, 0, 0 });
	asm_code = asm_code + makeByteArray({ 73, 184, 1, 0, 0, 0, 0, 0, 0, 0 });
	asm_code = asm_code + makeByteArray({ 186, 1, 0, 0, 0 });
	asm_code = asm_code + makeByteArray({ 72, 185 }) + intToBytes(target);
	asm_code = asm_code + makeByteArray({ 72, 184 }) + intToBytes(C_HIDDEN_CALL);
	asm_code = asm_code + makeByteArray({ 255, 208, 72, 129, 196, 0, 2, 0, 0 });

	memoryAssambly(asm_code);
}

void DNF::skillCall(__int64 pointer, int code, __int64 damage, int x, int y, int z, int skillSize)
{
	__int64 emptyAddress = C_EMPTY_ADDRESS + 1200;
	std::vector<byte>asm_code;

	float size = (float)skillSize;

	writeLong(emptyAddress, pointer);
	writeInt(emptyAddress + 16, code);
	writeLong(emptyAddress + 20, damage);
	writeInt(emptyAddress + 32, x);
	writeInt(emptyAddress + 36, y);
	writeInt(emptyAddress + 40, z);
	writeFloat(emptyAddress + 140, size);
	writeInt(emptyAddress + 144, 65535);
	writeInt(emptyAddress + 148, 65535);

	asm_code = asm_code + makeByteArray({ 72, 129, 236, 0, 2, 0, 0 });
	asm_code = asm_code + makeByteArray({ 72, 185 }) + intToBytes(emptyAddress);
	asm_code = asm_code + makeByteArray({ 72, 184 }) + intToBytes(C_SKILL_CALL);
	asm_code = asm_code + makeByteArray({ 255, 208, 72, 129, 196, 0, 2, 0, 0 });

	memoryAssambly(asm_code);
}

COORDINATE DNF::judgeBossRoom()
{
	COORDINATE coor;
	__int64 roomData = readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET);
	coor.x = (int)decrypt(roomData + C_BOSS_ROOM_X);
	coor.y = (int)decrypt(roomData + C_BOSS_ROOM_Y);
	return coor;
}

COORDINATE DNF::judgeCurrentRoom()
{
	COORDINATE coor;

	__int64 roomData = readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET);
	coor.x = readInt(roomData + C_CURRENT_ROOM_X);
	coor.y = readInt(roomData + C_CURRENT_ROOM_Y);
	return coor;
}

bool DNF::judgeHaveMonster()
{
	if (judgeGameStatus() != 3)
	{
		return false;
	}

	__int64 head_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_HEAD);
	__int64 end_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_END);

	int monster_quantity = (int)(end_address - head_address) / 16;
	for (__int64 i = 1; i <= monster_quantity; i++)
	{
		__int64 monster_address = readLong(readLong(head_address + i * 16) + 16) - 32;
		int monster_type = readInt(monster_address + C_TYPE_OFFSET);
		int monster_camp = readInt(monster_address + C_CAMP_OFFSET);
		int monster_code = readInt(monster_address + C_CODE_OFFSET);
		int monster_blood = readInt(monster_address + C_MONSTER_BLOOD);

		if (monster_type == 0x111 || monster_type == 0x211)
		{
			if (monster_camp != 0 && monster_blood != 0)
			{
				return true;
			}
		}

		handleEvents();
	}
	return false;
}

bool DNF::judgeHaveItem()
{
	if (judgeGameStatus() != 3)
	{
		return false;
	}

	__int64 head_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_HEAD);
	__int64 end_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_END);

	int monster_quantity = (int)(end_address - head_address) / 16;
	for (__int64 i = 1; i <= monster_quantity; i++)
	{
		__int64 monster_address = readLong(readLong(head_address + i * 16) + 16) - 32;
		int monster_type = readInt(monster_address + C_TYPE_OFFSET);
		int monster_camp = readInt(monster_address + C_CAMP_OFFSET);
		int monster_code = readInt(monster_address + C_CODE_OFFSET);
		int monster_blood = readInt(monster_address + C_MONSTER_BLOOD);

		if (monster_type == 0x121)
		{
			return true;
		}

		handleEvents();
	}
	return false;
}

int DNF::judgeGameStatus()
{
	return readInt(C_GAME_STATUS);
}

bool DNF::judgeClearance()
{
	__int64 roomData = readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET);
	__int64 result = readInt(roomData + C_BONFIRE_JUDGE);
	if (result == 2 || result == 0)
	{
		return true;
	}
	else {
		return false;
	}
}

bool DNF::judgeDoorOpen()
{
	if (decrypt2(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_DOOR_OFFSET) == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool DNF::judgeIsBossRoom()
{
	COORDINATE cur, boss;
	cur = judgeCurrentRoom();
	boss = judgeBossRoom();
	if (cur.x == boss.x && cur.y == boss.y)
	{
		return true;
	}
	else {
		return false;
	}
}

// 判断技能冷却
int DNF::judgeCoolDown()
{
	//__int64 address;
	//address = readLong(C_USER);
	//address = readLong(address + 技能栏);
	//address = readLong(address + 技能栏偏移);
	//
	//__int64 res = readLong(address + 技能位置);
	//__int64 call = 0x143E31BC0;
	//if (res != 0) {
	//	
	//}
	//else {
	//	return Keyboard_x;
	//}

	return Keyboard_x;
}

bool DNF::judgeGabriel() 
{
	//__int64 type = readLong(通关商店类型);
	//// 1000 大妈；1003 加百利； 1007 ?
	//if (type == 1003) {
	//	return true;
	//}
	return false;
}

UINT manualThread(LPVOID pParam)
{
	// 手动逻辑处理
	DNF* dnf = (DNF*)pParam;

	bool first_room = false;
	bool clearance_judge = false;

	while (true)
	{
		if (dnf->readInt(0x140000000) != 0x905A4D) {
			// 游戏结束
		}

		// 如果在图内
		if (dnf->readInt(C_GAME_STATUS) == 3)
		{
			// 开启拾取
			if (MainDlg->page1._switch_gather_items.GetCheck() == BST_CHECKED)
			{
				//全屏吸物
				//dnf->gatherItems();
				//dnf->gatherAll();
			}

			// 如果是第一个房间
			if (first_room == false && dnf->judgeClearance() == false)
			{
				first_room = true;
				dnf->firstRoomFunctions();
			}

			// 如果已经通关
			if (clearance_judge == false && dnf->judgeClearance() == true)
			{
				first_room = false;
				clearance_judge = true;

				// 关闭图内功能
				dnf->closeDungeonFunctions();
			}
		}

		// 如果不在图内
		if (dnf->readInt(C_GAME_STATUS) <= 2)
		{
			first_room = false;
			clearance_judge = false;
		}

		dnf->programDelay(100);
		dnf->handleEvents();
	}

	dnf->hiddenUser();

	return 0;   // thread completed successfully
}

UINT autoThread(LPVOID pParam)
{
	// 自动逻辑处理
	DNF* dnf = (DNF*)pParam;

	bool change_user = false;

	bool first_room = false;
	bool clearance = false;
	bool handle_next_room = false;
	int Gabriel = 0;
	int handle_room_times = 0;
	int gather_item_times = 0;

	//
	while (true)
	{
		if (dnf->judgeGameStatus() == 3)
		{
			// 遍历怪物与物品
			dnf->getDungeonAllObj();

			// 如果没开门
			if (dnf->judgeDoorOpen() == false)
			{
				MainDlg->Log(L"当前未开门");

				//dnf->penetrate(false);
				handle_next_room = false;
				clearance = false;
				handle_room_times = 0;

				// 首图功能
				if (first_room == false)
				{
					MainDlg->Log(L"开启首图功能");
					first_room = true;
					dnf->penetrate(true);
					dnf->firstRoomFunctions();
				}

				// 处理首个怪物
				if (dnf->_monster_list.size() > 0) {
					// 跟踪怪物
					COORDINATE monster_coor = dnf->_monster_list[0].coor;
					MainDlg->Log(L"跑到怪物位置");
					bool arrive_target = dnf->runToDestination(monster_coor.x, monster_coor.y, false, 50);
					if (arrive_target) {
						// 判断技能冷却列表并释放随机技能
						int key = dnf->getCoolDownKey();
						if (key == Keyboard_x) {
							MainDlg->Log(L"进行普通攻击");
							M_KeyPress(MainDlg->msdk_handle, Keyboard_x, 3);
						}
						else {
							MainDlg->Log(L"进行技能攻击");
							M_KeyPress(MainDlg->msdk_handle, key, 1);
						}
					}
					else {
						MainDlg->Log(L"到达目标失败,不采取动作");
					}
				}
			}
			else {
				MainDlg->Log(L"当前已开门");

				if (dnf->_item_list.size() > 0) {
					gather_item_times++;
					MainDlg->Log(L"存在物品，关闭穿透，进行聚物");
					dnf->penetrate(false);
					dnf->gatherItemsByItems(dnf->_item_list);
				}
				else {
					MainDlg->Log(L"没有物品，开启穿透");
					dnf->penetrate(true);
				}

				// 如果当前是BOSS
				if (dnf->judgeIsBossRoom())
				{
					MainDlg->Log(L"当前BOSS房间");

					// 通关->通关处理
					if (dnf->judgeClearance())
					{
						MainDlg->Log(L"当前已通关");

						first_room = false;
						if (clearance)
						{
							// 关闭图内功能
							MainDlg->Log(L"关闭图内功能");
							dnf->closeDungeonFunctions();

							Gabriel += 1;
							if (Gabriel >= 15)
							{
								//再次挑战();
								MainDlg->Log(L"再次挑战");
								if (Gabriel >= 30) {
									M_KeyPress(MainDlg->msdk_handle, Keyboard_ESCAPE, 2);
									M_KeyPress(MainDlg->msdk_handle, Keyboard_RightControl, 1);
								}
								else {
									M_KeyPress(MainDlg->msdk_handle, Keyboard_RightControl, 1);
								}
							}
							if (Gabriel == 30)
							{
								//返回城镇();
							}
						}
						else {
							Gabriel = 0;
							clearance = true;
							// 组包翻牌();
							handle_room_times += 1;
							// 分解装备();
							// 更换角色();
						}
					}
				}
				else {
					MainDlg->Log(L"当前普通房间");

					// 没有物品->进行过图处理
					if (dnf->_item_list.size() == 0 || gather_item_times > 3)
					{
						// 过图处理
						handle_room_times += 1;
						if (handle_room_times >= 3)
						{
							MainDlg->Log(L"即将进入下个房间");
							dnf->autoNextRoom();
							handle_room_times = 0;
							gather_item_times = 0;
						}
					}
				}
			}

			// 清空物品容器
			dnf->_item_list.clear();
			// 清空怪物容器
			dnf->_monster_list.clear();
		}

		if (dnf->judgeGameStatus() == 1 && change_user == false)
		{
			first_room = false;

			handle_room_times += 1;
			if (handle_room_times > 12)
			{
				//二次进图(); // 继续挑战
				handle_room_times = 0;
			}
		}

		dnf->programDelay(300);
	}
	return 0;
}

UINT userPointerThread(LPVOID pParam)
{
	DNF* dnf = (DNF*)pParam;

	bool statusChange = false;
	__int64 emptyAddress;

	while (true)
	{
		// 如果切换到后台
		if (!dnf->windowIsTop()) {
			//切换到后台，停止跑图;
			M_ReleaseAllKey(MainDlg->msdk_handle);
		}

		if (dnf->readInt(0x140000000) != 0x905A4D) {
			// 游戏结束
			statusChange = false;
			break;
		}

		if (dnf->judgeGameStatus() >= 1 && statusChange == false)
		{
			emptyAddress = C_EMPTY_ADDRESS + 4000;
			dnf->C_USER_POINTER = dnf->getUserPointer(emptyAddress);
			dnf->C_USER = emptyAddress;
			if (dnf->C_USER_POINTER == 0)
			{
				// 重新读取人物指针
				continue;
			}
			statusChange = true;
			// 人物指针已改变
			MainDlg->Log(L"人物指针已改变");
		}
		else if (dnf->judgeGameStatus() == 0)
		{
			// 选择角色
			statusChange = false;
		}
		dnf->programDelay(300);
	}
	return 0;
}

void DNF::manualThreadControl()
{
	// 开启线程逻辑
	AfxBeginThread(manualThread, this);
}

void DNF::autoThreadControl()
{
	AfxBeginThread(autoThread, this);
}

void DNF::userPonterUpdate()
{
	AfxBeginThread(userPointerThread, this);
}

void DNF::superScore()
{
	writeLong(readLong(C_SCORE_ADDRESS) + C_CE_SCORE, 999999);
}

void DNF::gatherItems()
{
	if (judgeGameStatus() != 3)
	{
		return;
	}

	__int64 head_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_HEAD);
	__int64 end_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_END);
	int item_quantity = 0;
	COORDINATE monster_coordinate;
	COORDINATE user_coordinate;

	int monster_quantity = (int)(end_address - head_address) / 24;
	for (__int64 i = 1; i <= monster_quantity; i++)
	{
		__int64 monster_address = readLong(readLong(head_address + i * 24) + 16) - 32;
		int monster_type = readInt(monster_address + C_TYPE_OFFSET);
		int monster_camp = readInt(monster_address + C_CAMP_OFFSET);
		int monster_code = readInt(monster_address + C_CODE_OFFSET);
		int monster_blood = readInt(monster_address + C_MONSTER_BLOOD);

		if (monster_type == 0x121)
		{
			item_quantity += 1;
			monster_coordinate = readCoordinate(monster_address);
			user_coordinate = readCoordinate(readLong(C_USER));
			if (monster_coordinate.z == 0)
			{
				if (abs(monster_coordinate.x - user_coordinate.x) < 20 && abs(monster_coordinate.y - user_coordinate.y) < 20)
				{
					continue;
				}
				writeFloat(readLong(monster_address + C_OBJECT_COORDINATE) + 32, (float)user_coordinate.x);
				writeFloat(readLong(monster_address + C_OBJECT_COORDINATE) + 36, (float)user_coordinate.y);
			}
			handleEvents();
		}
	}

	if (item_quantity > 0)
	{
		writeByteArray(C_AUTO_PICKUP, makeByteArray({ 117,21 }));
		programDelay(150);
		writeByteArray(C_AUTO_PICKUP, makeByteArray({ 116,21 }));
	}
}

void DNF::gatherItemsByItems(std::vector<DUNGEONOBJ> items) 
{
	if (judgeGameStatus() != 3)
	{
		return;
	}

	COORDINATE user_coordinate = readCoordinate(readLong(C_USER));

	size_t length = items.size();

	for (size_t i = 0; i < length; i++) 
	{
		if (abs(items[i].coor.x - user_coordinate.x) < 20 && abs(items[i].coor.y - user_coordinate.y) < 20)
		{
			continue;
		}
		writeFloat(readLong(items[i].p + C_OBJECT_COORDINATE) + 32, (float)user_coordinate.x);
		writeFloat(readLong(items[i].p + C_OBJECT_COORDINATE) + 36, (float)user_coordinate.y);
		handleEvents();
	}

	if (length > 0)
	{
		writeByteArray(C_AUTO_PICKUP, makeByteArray({ 117,21 }));
		programDelay(150);
		writeByteArray(C_AUTO_PICKUP, makeByteArray({ 116,21 }));
	}
}

void DNF::gatherAll()
{
	if (judgeGameStatus() != 3)
	{
		return;
	}

	__int64 head_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_HEAD);
	__int64 end_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_END);
	int item_quantity = 0;
	COORDINATE monster_coordinate;
	COORDINATE user_coordinate;

	int monster_quantity = (int)(end_address - head_address) / 24;
	for (__int64 i = 1; i <= monster_quantity; i++)
	{
		__int64 monster_address = readLong(readLong(head_address + i * 24) + 16) - 32;
		int monster_type = readInt(monster_address + C_TYPE_OFFSET);
		int monster_camp = readInt(monster_address + C_CAMP_OFFSET);
		int monster_code = readInt(monster_address + C_CODE_OFFSET);
		int monster_blood = readInt(monster_address + C_MONSTER_BLOOD);

		if (monster_type == 0x211 || monster_type == 0x121 || monster_type == 0x111)
		{
			// 0x211 领主
			// 0x121 物品
			// 0x111 ??
			if (monster_type == 0x121) {
				item_quantity += 1;
			}
			monster_coordinate = readCoordinate(monster_address);
			user_coordinate = readCoordinate(readLong(C_USER));
			if (abs(monster_coordinate.x - user_coordinate.x) < 20 && abs(monster_coordinate.y - user_coordinate.y) < 20)
			{
				continue;
			}

			// 移动对象
			//writeFloat(readLong(monster_address + C_OBJECT_COORDINATE) + 32, (float)user_coordinate.x);
			//writeFloat(readLong(monster_address + C_OBJECT_COORDINATE) + 36, (float)user_coordinate.y);

			// 移动到对象
			coorCall(monster_coordinate.x, monster_coordinate.y, monster_coordinate.z);
			handleEvents();
			break;
		}
	}

	if (item_quantity > 0)
	{
		writeByteArray(C_AUTO_PICKUP, makeByteArray({ 117,21 }));
		programDelay(150);
		writeByteArray(C_AUTO_PICKUP, makeByteArray({ 116,21 }));
	}
}

COORDINATE DNF::readCoordinate(__int64 address)
{
	__int64 pointer;
	COORDINATE coor;

	int type = readInt(address + C_TYPE_OFFSET);

	if (type == 0x111)
	{
		pointer = readLong(address + C_READ_COORDINATE);
		coor.x = (int)readFloat(pointer + 0);
		coor.y = (int)readFloat(pointer + 4);
		coor.z = (int)readFloat(pointer + 8);
		return coor;
	}
	else
	{
		pointer = readLong(address + C_OBJECT_COORDINATE);
		coor.x = (int)readFloat(pointer + 32);
		coor.y = (int)readFloat(pointer + 36);
		coor.z = (int)readFloat(pointer + 40);
		return coor;
	}
}

void DNF::firstRoomFunctions()
{
	// 上上空格
	M_KeyPress(MainDlg->msdk_handle, Keyboard_UpArrow, 1);
	M_KeyPress(MainDlg->msdk_handle, Keyboard_UpArrow, 1);
	M_KeyPress(MainDlg->msdk_handle, Keyboard_KongGe, 1);

	M_DelayRandom(200,350);

	//// 右右空格
	M_KeyPress(MainDlg->msdk_handle, Keyboard_RightArrow, 1);
	M_KeyPress(MainDlg->msdk_handle, Keyboard_RightArrow, 1);
	M_KeyPress(MainDlg->msdk_handle, Keyboard_KongGe, 1);

	M_DelayRandom(200, 350);

	if (MainDlg->page1._switch_score.GetCheck() == BST_CHECKED)
	{
		superScore();
	}

	if (MainDlg->page1._switch_cool_down.GetCheck() == BST_CHECKED)
	{
		CString num;
		MainDlg->page3._cool_down.GetWindowText(num);
		float number = (float)_ttof(num);
		skillCoolDown(number);
	}

	if (MainDlg->page1._switch_hook_damage.GetCheck() == BST_CHECKED)
	{
		hookDamage(true);
	}

	if (MainDlg->page1._switch_three_speed.GetCheck() == BST_CHECKED)
	{
		CString attack_speed, move_speed, casting_speed;
		MainDlg->page3._attack_speed.GetWindowText(attack_speed);
		MainDlg->page3._move_speed.GetWindowText(move_speed);
		MainDlg->page3._casting_speed.GetWindowText(casting_speed);
		threeSpeed(_ttoi(attack_speed), _ttoi(casting_speed), _ttoi(move_speed));
	}

	if (MainDlg->page1._switch_hidden_user.GetCheck() == BST_CHECKED)
	{
		hiddenUser();
	}
}

void DNF::clearanceEvent()
{

}

void DNF::skillCoolDown(float num)
{
	encrypt(readLong(C_USER) + C_FLOAT_COOL_DOWN2, (int)num);
}

__int64 DNF::getUserPointer(__int64 emptyAddress)
{
	std::vector<byte>asm_code;

	asm_code = makeByteArray({ 72, 131, 236,100 });
	asm_code = asm_code + makeByteArray({ 72,184 }) + intToBytes(C_USER_CALL);
	asm_code = asm_code + makeByteArray({ 255,208 });
	asm_code = asm_code + makeByteArray({ 72,163 }) + intToBytes(emptyAddress);
	asm_code = asm_code + makeByteArray({ 72,131,196,100 });
	memoryAssambly(asm_code);

	return readLong(emptyAddress);
}

void DNF::hookDamage(bool on)
{
	CString value;
	MainDlg->page3._damage_value.GetWindowText(value);

	std::vector<byte>damge_data;

	__int64 damage_address = C_GLOBAL_ADDRESS;
	__int64 damage_value = _ttoi(value);

	if (on) {
		damge_data = readByteArray(damage_address, 10);
		writeByteArray(damage_address, (makeByteArray({ 72,190 }) + intToBytes(damage_value)));
	}
	else {
		writeByteArray(damage_address, damge_data);
	}
}

__int64 DNF::passRoomData(int direction)
{
	__int64 empty_address = C_EMPTY_ADDRESS + 1450;
	__int64 room_data = readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_PASS_ROOM_OFFSET);

	std::vector<byte>asm_code;
	asm_code = makeByteArray({ 72,129,236,0,1,0,0 });
	asm_code = asm_code + makeByteArray({ 72,185 }) + intToBytes(room_data);
	asm_code = asm_code + makeByteArray({ 186 }) + intToBytes2(direction);
	asm_code = asm_code + makeByteArray({ 72,184 }) + intToBytes(C_COORDINATE_PASS_ROOM);
	asm_code = asm_code + makeByteArray({ 255,208 });
	asm_code = asm_code + makeByteArray({ 72,163 }) + intToBytes(empty_address);
	asm_code = asm_code + makeByteArray({ 72,129,196,0,1,0,0 });
	memoryAssambly(asm_code);
	return readLong(empty_address);
}

void DNF::closeDungeonFunctions()
{
	skillCoolDown(0);
	threeSpeed(0, 0, 0);
	hookDamage(false);
}

// 跑到目标
BOOL DNF::runToDestination(int x, int y, bool is_room = false, int target_range = 10)
{
	if (x < 1 || y < 1) {
		MainDlg->Log(L"位置太近，放弃跑图");
		return false;
	}

	CString coor;
	coor.Format(L"目标坐标 X:%d Y:%d", x, y);
	MainDlg->Log(coor);

	__int64 a, b;
	if (is_room) {
		a = readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_X);
		b = readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_Y);
	}

	COORDINATE user_coor = readCoordinate(readLong(C_USER));

	bool x_arrived = false, y_arrived = false, isFirst = true, arrive_next = false;

	int direction_x;
	int direction_y;

	if (x > user_coor.x) {
		direction_x = Keyboard_RightArrow;
	}
	else {
		direction_x = Keyboard_LeftArrow;
	}

	if (y > user_coor.y) {
		direction_y = Keyboard_DownArrow;
	}
	else {
		direction_y = Keyboard_UpArrow;
	}

	// 记录当前时间戳
	time_t cur_time = time(NULL);

	while (true) {

		// 如果跑图时间超过5秒。则直接退出跑图
		if (time(NULL) - cur_time > 5) {
			MainDlg->Log(L"跑图超时，退出跑图");
			return false;
		}

		user_coor = readCoordinate(readLong(C_USER));

		if (isFirst) {
			MainDlg->Log(L"重新跑图");
			M_KeyPress(MainDlg->msdk_handle, direction_x, 1);
			M_DelayRandom(50,100);
			M_KeyDown(MainDlg->msdk_handle, direction_x);
			M_DelayRandom(100, 150);
			M_KeyDown(MainDlg->msdk_handle, direction_y);
		}

		if (judgeGameStatus() != 3) {
			//不在图内，停止跑图;
			MainDlg->Log(L"人物已离开图内，停止跑图");
			M_ReleaseAllKey(MainDlg->msdk_handle);
			break;
		}

		// 判断人物动作
		//if (decrypt(readLong(C_USER) + C_MOVEMENT_ID) != 14) {
		//	MainDlg->Log(L"当前不是跑步动作，停止跑图");
		//	// 弹起移动按键
		//	keyboardUp(direction_x);
		//	keyboardUp(direction_y);
		//	break;
		//}

		// 判断房间：如果目标是下一个房间
		if (is_room) {
			if (readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_X) != a ||
				readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_Y) != b) {
				arrive_next = true;
				MainDlg->Log(L"房间发生变化，停止按键");
				// 弹起移动按键
				M_ReleaseAllKey(MainDlg->msdk_handle);
				break;
			}
		}

		// 优先到达Y坐标
		if (y_arrived == false) {
			if (direction_y == Keyboard_UpArrow) {
				if (y - user_coor.y > 10) {
					MainDlg->Log(L"向上超出Y范围，停止");
					M_ReleaseAllKey(MainDlg->msdk_handle);
					break;
				}
			}
			else {
				if (user_coor.y - y > 10) {
					MainDlg->Log(L"向下超出Y范围，停止");
					M_ReleaseAllKey(MainDlg->msdk_handle);
					break;
				}
			}

			if (abs(user_coor.y - y) > 10) {
				isFirst = false;
				continue;
			}
			else {
				keyboardUp(direction_y);
				y_arrived = true;
			}
		}

		if (x_arrived == false) {
			if (direction_x == Keyboard_RightArrow) {
				if (user_coor.x - x > target_range) {
					MainDlg->Log(L"向右超出X范围，停止");
					M_ReleaseAllKey(MainDlg->msdk_handle);
					break;
				}
			}
			else {
				if (x - user_coor.x > target_range) {
					MainDlg->Log(L"向左超出X范围，停止");
					M_ReleaseAllKey(MainDlg->msdk_handle);
					break;
				}
			}
			if (abs(user_coor.x - x) > target_range) {
				isFirst = false;
				continue;
			}
			else {
				keyboardUp(direction_x);
				x_arrived = true;
			}
		}

		if (x_arrived && y_arrived) {
			MainDlg->Log(L"到达目标位置，停止按键");

			M_ReleaseAllKey(MainDlg->msdk_handle);
			return true;
			break;
		}

		//programDelay(100);
	}

	//handleEvents();

	MainDlg->Log(L"跑图结束，停止按键");
	M_ReleaseAllKey(MainDlg->msdk_handle);

	return false;
}

// 走到目标
void DNF::walkToDestination()
{

}

void DNF::keyboardUp(int key)
{
	if (M_KeyState(MainDlg->msdk_handle, key) == 1) {
		M_KeyUp(MainDlg->msdk_handle, key);
	}
}

bool DNF::windowIsTop()
{
	// 判断DNF是否是置顶窗口
	HWND cur_top_window = GetForegroundWindow();
	HWND dnf_window = FindWindowA(NULL, "地下城与勇士");
	return cur_top_window == dnf_window;
}

void DNF::penetrate(bool on)
{
	if (on) {
		writeInt(readLong(C_USER) + C_PENETRATE_MAP, -255);
		writeInt(readLong(C_USER) + C_PENETRATE_BUILDING, -255);
	}
	else {
		writeInt(readLong(C_USER) + C_PENETRATE_MAP, 10);
		writeInt(readLong(C_USER) + C_PENETRATE_BUILDING, 40);
	}
}

void DNF::autoNextRoom()
{
	if (judgeGameStatus() != 3) {
		return;
	}

	COORDINATE room_coor = judgeCurrentRoom();
	COORDINATE boss_coor = judgeBossRoom();

	if (room_coor.x == boss_coor.x && room_coor.y == boss_coor.y) {
		return;
	}

	int next_direction = judgeNextRoomDiretion(room_coor, boss_coor);

	if (next_direction == 1)
	{
		MainDlg->Log(L"向右奔跑");
	}
	if (next_direction == 2)
	{
		MainDlg->Log(L"向左奔跑");
	}
	if (next_direction == 3)
	{
		MainDlg->Log(L"向下奔跑");
	}
	if (next_direction == 4)
	{
		MainDlg->Log(L"向上奔跑");
	}

	runToNextRoom(next_direction);

}

int DNF::judgeNextRoomDiretion(COORDINATE current, COORDINATE boss)
{
	int x, y;

	DUNGEONMAP map_data = mapData();
	if (map_data.fatigue < 1) {
		return 0;
	}
	x = map_data.way[0].x - map_data.way[1].x;
	y = map_data.way[0].y - map_data.way[1].y;


	if (x == 0)
	{
		if (y == 1)
		{
			return 2;// 上
		}
		else
		{
			return 3;// 下
		}
	}
	else if (y == 0)
	{
		if (x == 1)
		{
			return 0;// 左
		}
		else
		{
			return 1;// 右
		}
	}

	return 0;
}

DUNGEONMAP DNF::mapData()
{
	DUNGEONMAP map_data;

	__int64 room_data = readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET);
	__int64 room_index = decrypt2(room_data + C_MAP_CODE);

	map_data.width = readInt(readLong(room_data + C_WH_OFFSET) + room_index * 8 + 0);
	map_data.height = readInt(readLong(room_data + C_WH_OFFSET) + room_index * 8 + 4);
	map_data.tmp = readLong(readLong(room_data + C_AISLE_OFFSET) + 32 * room_index + 8);

	map_data.aisle_num = map_data.width * map_data.height;

	for (__int64 i = 0; i < map_data.aisle_num; i++)
	{
		map_data.aisle.insert(map_data.aisle.begin() + i, readInt(map_data.tmp + i * 4));
	}

	map_data.begin.x = readInt(readInt(readInt(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_BEGIN_ROOM_X) + 1;
	map_data.begin.y = readInt(readInt(readInt(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_BEGIN_ROOM_Y) + 1;
	map_data.end.x = (int)decrypt2(room_data + C_BOSS_ROOM_X) + 1;
	map_data.end.y = (int)decrypt2(room_data + C_BOSS_ROOM_Y) + 1;

	if (map_data.begin.x == map_data.end.x && map_data.begin.y == map_data.end.y) {
		return map_data;
	}

	map_data.fatigue = getWay(map_data.aisle, map_data.width, map_data.height, map_data.begin, map_data.end, map_data.way);
	return map_data;
}

int DNF::getWay(std::vector<int> aisle, int width, int height, COORDINATE begin, COORDINATE end, std::vector<COORDINATE>& way)
{
	COORDINATE begin_coor, end_coor;
	std::vector<std::vector<AISLEDATA>> map_tag, map_array;
	std::vector<COORDINATE> cross_way;

	if (begin.x == end.x && begin.y == end.y)
	{
		way.clear();
		way.resize(0);
		return 0;
	}

	createMap(width, height, aisle, map_array);
	showMap(map_array, width, height, map_tag);

	begin_coor.x = begin.x * 3 - 2;
	begin_coor.y = begin.y * 3 - 2;
	end_coor.x = end.x * 3 - 2;
	end_coor.y = end.y * 3 - 2;

	// 路径算法
	pathCalc(map_tag, begin_coor, end_coor, width * 3, height * 3, cross_way);

	// 整理坐标
	return arrangeCoor(cross_way, way);
}

void DNF::createMap(int width, int height, std::vector<int> aisle, std::vector<std::vector<AISLEDATA>>& map)
{
	map.clear();
	map.resize(width);
	for (int x = 0; x < width; x++)
	{
		map[x].resize(height);
	}

	int i = 0;
	int x, y;
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			map[x][y].coor.x = x;
			map[x][y].coor.y = y;
			map[x][y].aisle = aisle[i];
			map[x][y].left = judgeDirection(aisle[i], 0);
			map[x][y].right = judgeDirection(aisle[i], 1);
			map[x][y].top = judgeDirection(aisle[i], 2);
			map[x][y].bottom = judgeDirection(aisle[i], 3);
			map[x][y].bg = 0xFFFFFF;
			i++;
			if (map[x][y].aisle == 0)
			{
				map[x][y].bg = 0x000000;
			}
		}
	}
}

void DNF::showMap(std::vector<std::vector<AISLEDATA>> map, int width, int height, std::vector<std::vector<AISLEDATA>>& tag)
{
	tag.clear();
	tag.resize(width * 3);

	for (int x = 0; x < width * 3; x++)
	{
		tag[x].resize(height * 3);
	}

	int x, y;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			tag[(x + 1) * 3 - 2][(y + 1) * 3 - 2].bg = 0xFFFFFF;
			if (map[x][y].left == true)
			{
				tag[(x + 1) * 3 - 3][(y + 1) * 3 - 2].bg = 0xFFFFFF;
			}
			if (map[x][y].right == true)
			{
				tag[(x + 1) * 3 - 1][(y + 1) * 3 - 2].bg = 0xFFFFFF;
			}
			if (map[x][y].top == true)
			{
				tag[(x + 1) * 3 - 2][(y + 1) * 3 - 3].bg = 0xFFFFFF;
			}
			if (map[x][y].bottom == true)
			{
				tag[(x + 1) * 3 - 2][(y + 1) * 3 - 1].bg = 0xFFFFFF;
			}
		}
	}

}

bool DNF::judgeDirection(int aisle, int direction)
{
	unsigned char direction_arr[4];
	unsigned char direction_set[16][4] =
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 1, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 1, 0, 1, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 1 },
		{ 0, 1, 0, 1 },
		{ 0, 0, 1, 1 },
		{ 0, 1, 1, 1 },
		{ 1, 0, 0, 1 },
		{ 1, 1, 0, 1 },
		{ 1, 0, 1, 1 },
		{ 1, 1, 1, 1 }
	};
	std::vector<byte> arr;
	if (aisle >= 0 && aisle <= 15)
	{
		for (int i = 0; i < 4; i++)
		{
			direction_arr[i] = direction_set[aisle][i];
		}
	}
	else {
		for (int i = 0; i < 4; i++)
		{
			direction_arr[i] = 0;
		}
	}
	if (direction_arr[direction] == 1) {
		return true;
	}
	return false;
}

void DNF::pathCalc(std::vector<std::vector<AISLEDATA>> map_tag, COORDINATE begin, COORDINATE end, int width, int height, std::vector<COORDINATE>& cross_way)
{
	bool exists_open_list, exists_close_list;
	COORDINATE not_check_coor;
	MAPNODE not_check_node, tmp_node;
	std::vector<MAPNODE> open_list, close_list;
	int min_number = 0;
	DWORD min_f;
	int estimate_g;
	int x, y;

	tmp_node.current.x = begin.x;
	tmp_node.current.y = begin.y;
	map_tag[begin.x][begin.y].bg = 0x00FF00;
	map_tag[end.x][end.y].bg = 0x0000FF;
	open_list.insert(open_list.begin(), tmp_node);

	do
	{
		min_f = 0;
		for (y = 0; y < open_list.size(); y++)
		{
			if (min_f == 0)
			{
				min_f = open_list[0].f;
				min_number = y;
			}
			if (open_list[y].f < min_f)
			{
				min_f = open_list[y].f;
				min_number = y;
			}
		}
		tmp_node = open_list[min_number];
		open_list.erase(open_list.begin() + min_number);
		close_list.insert(close_list.begin(), tmp_node);

		if (tmp_node.current.x != begin.x || tmp_node.current.y != begin.y)
		{
			if (tmp_node.current.x != end.x || tmp_node.current.y != end.y)
			{
				map_tag[tmp_node.current.x][tmp_node.current.y].bg = 0x0080FF;
			}
		}

		for (y = 0; y < close_list.size(); y++)
		{
			if (close_list[y].current.x == end.x && close_list[y].current.y == end.y)
			{
				not_check_node = close_list[y];
				do {
					for (unsigned int x = 0; x < close_list.size(); x++)
					{
						if (close_list[x].current.x == not_check_node.final.x && close_list[x].current.y == not_check_node.final.y)
						{
							not_check_node = close_list[x];
							break;
						}
					}
					if (not_check_node.current.x != begin.x || not_check_node.current.y != begin.y)
					{
						map_tag[not_check_node.current.x][not_check_node.current.y].bg = 0x00D8D8;
						cross_way.insert(cross_way.begin(), not_check_node.current);
					}

				} while (not_check_node.current.x != begin.x || not_check_node.current.y != begin.y);
				cross_way.insert(cross_way.begin(), begin);
				cross_way.insert(cross_way.end(), end);
				return;
			}
		}

		for (y = 0; y < 4; y++)
		{
			if (y == 0)
			{
				not_check_coor.x = tmp_node.current.x;
				not_check_coor.y = tmp_node.current.y - 1;
			}
			else if (y == 1) {
				not_check_coor.x = tmp_node.current.x - 1;
				not_check_coor.y = tmp_node.current.y;
			}
			else if (y == 2) {
				not_check_coor.x = tmp_node.current.x + 1;
				not_check_coor.y = tmp_node.current.y;
			}
			else {
				not_check_coor.x = tmp_node.current.x;
				not_check_coor.y = tmp_node.current.y + 1;
			}

			if (not_check_coor.x < 0 || not_check_coor.x>(width - 1) || not_check_coor.y<0 || not_check_coor.y>(height - 1))
			{
				continue;
			}
			if (map_tag[not_check_coor.x][not_check_coor.y].bg == 0x000000)
			{
				continue;
			}
			exists_close_list = false;

			for (x = 0; x < close_list.size(); x++)
			{
				if (close_list[x].current.x == not_check_coor.x && close_list[x].current.y == not_check_coor.y)
				{
					exists_close_list = true;
					break;
				}
			}
			if (exists_close_list) {
				continue;
			}
			exists_open_list = false;

			for (x = 0; x < open_list.size(); x++)
			{
				if (open_list[x].current.x == not_check_coor.x && open_list[x].current.y == not_check_coor.y)
				{
					if (not_check_coor.x != tmp_node.current.x || not_check_coor.y != tmp_node.current.y)
					{
						estimate_g = 14;
					}
					else
					{
						estimate_g = 10;
					}
					if (tmp_node.g + estimate_g < open_list[x].g) {
						open_list[x].final = tmp_node.current;
					}
					exists_open_list = true;
					break;
				}
			}

			if (exists_open_list == false)
			{
				if (not_check_coor.x == tmp_node.current.x || not_check_coor.y == tmp_node.current.y) {
					estimate_g = 10;
				}
				else {
					estimate_g = 14;
				}
				not_check_node.g = tmp_node.g + estimate_g;
				not_check_node.h = abs(end.x - not_check_coor.x) * 10 + abs(end.y - not_check_coor.y) * 10;
				not_check_node.f = not_check_node.g + not_check_node.h;
				not_check_node.current = not_check_coor;
				not_check_node.final = tmp_node.current;
				open_list.insert(open_list.begin(), not_check_node);
			}
		}
	} while (open_list.size() != 0);
}

int DNF::arrangeCoor(std::vector<COORDINATE>imitation, std::vector<COORDINATE>& real_cross)
{
	int x, y, k = 0;
	COORDINATE tmp_coor;
	for (int i = 0; i < imitation.size(); i++)
	{
		x = (imitation[i].x + 2) % 3;
		y = (imitation[i].y + 2) % 3;
		if (x == 0 && y == 0)
		{
			tmp_coor.x = (imitation[i].x + 2) / 3 - 1;
			tmp_coor.y = (imitation[i].y + 2) / 3 - 1;
			real_cross.insert(real_cross.begin() + k, tmp_coor);
			k++;
		}
	}
	return(k);
}

void DNF::runToNextRoom(int direction)
{
	__int64 pass_room_data = passRoomData(direction);
	__int64 coor_struct = pass_room_data;

	int begin_x, begin_y, end_x, end_y, calc_x, calc_y;
	begin_x = readInt(coor_struct + 0);
	begin_y = readInt(coor_struct + 4);
	end_x = readInt(coor_struct + 8);
	end_y = readInt(coor_struct + 12);

	if (direction == 0)
	{
		calc_x = begin_x + end_x + 20; // 火魂向左顺图
		calc_y = begin_y + end_y / 2;
	}
	if (direction == 1)
	{
		calc_x = begin_x - 20; // 火魂向右顺图
		calc_y = begin_y + end_y / 2;
	}
	if (direction == 2)
	{
		calc_x = begin_x + end_x / 2; // 火魂向上顺图
		calc_y = begin_y + end_y + 20;

	}
	if (direction == 3)
	{
		calc_x = begin_x + end_x / 2; // 火魂向下顺图
		calc_y = begin_y - 20;
	}

	if (calc_x < 0 || calc_y < 0)
	{
		return;
	}

	COORDINATE cur_room;
	cur_room.x = (int)readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_X);
	cur_room.y = (int)readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_Y);

	runToDestination(begin_x + end_x / 2, begin_y, true, 2);

	// 查看房间是否改变
	if (readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_X) != cur_room.x ||
		readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_Y) != cur_room.y) {
		//programDelay(500);该延迟会导致跑图失误
		MainDlg->Log(L"房间跑图一次完成");
	}
	else {
		programDelay(100);
		handleEvents();

		runToDestination(calc_x, calc_y, true, 2);

		handleEvents();
	}


}

// 获取冷却技能键位
int DNF::getCoolDownKey()
{

	__int64 address;
	address = readLong(C_USER);
	address = readLong(address + C_SKILL_LIST);
	address = readLong(address + C_SKILL_LIST_OFFSET);

	__int64 position = rand() % 17;
	//__int64 position = 0;

	__int64 skill_p = readLong(address + position * 8);

	if (skill_p <= 0) {
		return 0;
	}

	__int64 emptyAddress = C_EMPTY_ADDRESS + 3000;
	std::vector<byte>asm_code;

	asm_code = makeByteArray({ 72,131,236,32 });
	asm_code = asm_code + makeByteArray({ 49,210 });
	asm_code = asm_code + makeByteArray({ 72,185 }) + intToBytes(skill_p);
	asm_code = asm_code + makeByteArray({ 255,21,2,0,0,0,235,8 });
	asm_code = asm_code + intToBytes(C_COOL_DOWN_JUDGE_CALL);
	asm_code = asm_code + makeByteArray({ 72,162 }) + intToBytes(emptyAddress);
	asm_code = asm_code + makeByteArray({ 72,131,196,32 });

	memoryAssambly(asm_code);

	if (readLong(emptyAddress) < 1) {
		switch (position)
		{
		case 0:
			return Keyboard_a;
			break;
		case 1:
			return Keyboard_s;
			break;
		case 2:
			return Keyboard_d;
			break;
		case 3:
			return Keyboard_f;
			break;
		case 4:
			return Keyboard_g;
			break;
		case 5:
			return Keyboard_h;
			break;
		case 6:
			return Keyboard_x;
			break;
		case 7:
			return Keyboard_q;
			break;
		case 8:
			return Keyboard_w;
			break;
		case 9:
			return Keyboard_e;
			break;
		case 10:
			return Keyboard_r;
			break;
		case 11:
			return Keyboard_t;
			break;
		case 12:
			return Keyboard_y;
			break;
		case 14:
			return Keyboard_LeftAlt;
			break;
		case 13:
		case 15:
		case 16:
		default:
			return Keyboard_x;
			break;
		}
	}
	else {
		return Keyboard_x;
	}
}

// 坐标CALL
void DNF::coorCall(int x, int y, int z)
{
	__int64 target_p = readLong(C_USER);

	if (target_p < 1) {
		return;
	}

	std::vector<byte> asm_code = makeByteArray({ 72,129,236,0,1,0,0 });
	asm_code = asm_code + makeByteArray({ 65,185 }) + intToBytes2(z);
	asm_code = asm_code + makeByteArray({ 65,184 }) + intToBytes2(y);
	asm_code = asm_code + makeByteArray({ 186 }) + intToBytes2(x);
	asm_code = asm_code + makeByteArray({ 72,185 }) + intToBytes(target_p);
	asm_code = asm_code + makeByteArray({ 72,139,1 });
	asm_code = asm_code + makeByteArray({ 255,144 }) + intToBytes(C_COOR_CALL_OFFSET);
	asm_code = asm_code + makeByteArray({ 72,129,196,0,1,0,0 });
	memoryAssambly(asm_code);
}

void DNF::getDungeonAllObj()
{
	// 遍历怪物
	if (judgeGameStatus() != 3)
	{
		_item_list.clear();
		_monster_list.clear();
		return;
	}

	__int64 head_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_HEAD);
	__int64 end_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_MAP_END);

	if (head_address == 0 || end_address == 0) {
		_item_list.clear();
		_monster_list.clear();
		return;
	}

	int item_quantity = 0;

	int monster_quantity = (int)(end_address - head_address) / 24;

	if (monster_quantity == 0) {
		_item_list.clear();
		_monster_list.clear();
		return;
	}

	for (__int64 i = 1; i <= monster_quantity; i++)
	{
		__int64 monster_address = readLong(readLong(head_address + i * 24) + 16) - 32;
		int monster_camp = readInt(monster_address + C_CAMP_OFFSET);
		int monster_type = readInt(monster_address + C_TYPE_OFFSET);
		int monster_code = readInt(monster_address + C_CODE_OFFSET);
		int monster_blood = readInt(monster_address + C_MONSTER_BLOOD);
		COORDINATE monster_coor = readCoordinate(monster_address);

		// 物品
		if (monster_type == 289 && monster_coor.z == 0)
		{
			DUNGEONOBJ item;
			item.p = monster_address;
			item.blood = monster_blood;
			item.type = monster_type;
			item.camp = monster_camp;
			item.code = monster_code;
			item.coor = monster_coor;
			_item_list.push_back(item);
		}

		// 怪物
		if (monster_type == 273 || monster_type == 529)
		{
			if (monster_camp != 0 && monster_blood != 0)
			{
				DUNGEONOBJ monster;
				monster.p = monster_address;
				monster.blood = monster_blood;
				monster.type = monster_type;
				monster.camp = monster_camp;
				monster.code = monster_code;
				monster.coor = monster_coor;
				if (monster_coor.x != 0 && monster_coor.y != 0)
				{
					_monster_list.push_back(monster);
				}
			}
		}
	}

	// 对怪物容器进行排序(冒泡)
	int i, j, len;
	DUNGEONOBJ temp;
	len = (int)_monster_list.size();
	for (i = 0; i < len - 1; i++) {
		for (j = 0; j < len - 1 - i; j++) {
			if (_monster_list[j].coor.x > _monster_list[j + 1].coor.x) {
				temp = _monster_list[j];
				_monster_list[j] = _monster_list[j + 1];
				_monster_list[j + 1] = temp;
			}
		}
	}
}