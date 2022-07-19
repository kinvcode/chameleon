#include "pch.h"
#include "dnf.h"
#include "chameleonDlg.h"
#include "overloading.h"
#include "msdk.h"
#include "UsbHidKeyCode.h"

extern CchameleonDlg* MainDlg;

DNF::DNF(DWORD PID)
{
	this->PID = PID;
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
	__int64 roomData = readLong(readLong(readLong(readLong(C_ROOM_NUMBER)) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET);
	coor.x = (int)decrypt(roomData + C_BOSS_ROOM_X);
	coor.y = (int)decrypt(roomData + C_BOSS_ROOM_Y);
	return coor;
}

COORDINATE DNF::judgeCurrentRoom()
{
	COORDINATE coor;
	__int64 roomData = readLong(readLong(readLong(readLong(C_ROOM_NUMBER)) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET);
	coor.x = (int)decrypt(roomData + C_CURRENT_ROOM_X);
	coor.y = (int)decrypt(roomData + C_CURRENT_ROOM_Y);
	return coor;
}

bool DNF::judgeHaveMonster()
{
	if (judgeGameStatus() != 3)
	{
		return false;
	}

	__int64 head_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_HEAD_ADDRESS);
	__int64 end_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_END_ADDRESS);

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

	__int64 head_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_HEAD_ADDRESS);
	__int64 end_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_END_ADDRESS);

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
	if (decrypt(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_DOOR_OFFSET) == 0) {
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
		if (dnf->readInt(dnf->C_GAME_STATUS) == 3)
		{
			// 开启拾取
			if (MainDlg->_switch_gather_items.GetCheck() == BST_CHECKED)
			{
				//全屏吸物
				dnf->gatherItems();
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
		if (dnf->readInt(dnf->C_GAME_STATUS) <= 2)
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

	//
	dnf->penetrate(true);

	while (true)
	{
		if (dnf->judgeGameStatus() == 3)
		{
			// 如果没开门
			if (dnf->judgeDoorOpen() == false)
			{
				handle_next_room = false;
				clearance = false;
				handle_room_times = 0;
				//每图循环(); 未完成:跟随怪物，输出伤害 // 打怪逻辑处理

				// 获取第一个怪物坐标
				__int64 head_address = dnf->readLong(dnf->readLong(dnf->readLong(dnf->readLong(dnf->C_USER) + dnf->C_MAP_OFFSET) + 16) + dnf->C_HEAD_ADDRESS);
				__int64 end_address = dnf->readLong(dnf->readLong(dnf->readLong(dnf->readLong(dnf->C_USER) + dnf->C_MAP_OFFSET) + 16) + dnf->C_END_ADDRESS);

				int monster_quantity = (int)(end_address - head_address) / 24;

				for (int i = 1; i <= monster_quantity; i++)
				{
					__int64 monster_address = dnf->readLong(dnf->readLong(head_address + (__int64)i * 24) + 16) - 32;
					int monster_type = dnf->readInt(monster_address + dnf->C_TYPE_OFFSET);
					int monster_camp = dnf->readInt(monster_address + dnf->C_CAMP_OFFSET);
					int monster_code = dnf->readInt(monster_address + dnf->C_CODE_OFFSET);
					int monster_blood = dnf->readInt(monster_address + dnf->C_MONSTER_BLOOD);

					// 处理特殊对象：格蓝迪柱子
					if (monster_code == 109051366 || monster_code == 109051365 || monster_code == 109051364) {
						// 暂时不处理
					}



					// 需要清理的怪物或建筑
					if (monster_type == 529 || monster_type == 273) {
						if (monster_address != dnf->readLong(dnf->C_USER)) {
							if (monster_camp != 0 && monster_code != 0 && monster_blood != 0) {
								// 获取到第一个怪物地址
								// 移动人物到此怪物地址
								COORDINATE monster_coor = dnf->readCoordinate(monster_address);

								// 输出目标名称
								CString name2;
								CStringA name;
								name = dnf->Unicode2Ansi(dnf->readByteArray(dnf->readLong(monster_address + dnf->C_NAME_OFFSET), 50));
								name2 = name;
								MainDlg->Log(name2);
								// 到达目标后立即进行攻击（无论目标是否已经移动）
								dnf->runToDestination(monster_coor.x, monster_coor.y, false);
								// 攻击
								M_KeyPress(MainDlg->msdk_handle, Keyboard_x, 3);
								
								break;
							}
						}
					}

					// 如果是物品，则进行捡物品
				}
			}
			else {
				// 如果当前是BOSS
				if (dnf->judgeIsBossRoom())
				{
					// 通关->通关处理
					if (dnf->judgeClearance())
					{
						first_room = false;
						if (clearance)
						{
							dnf->gatherItems();
							Gabriel += 1;
							if (Gabriel == 15)
							{
								//再次挑战();
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
					dnf->gatherItems();

					// 没有物品->进行过图处理
					if (!dnf->judgeHaveItem())
					{
						// 过图处理
						if (!first_room)
						{
							first_room = true;
							dnf->firstRoomFunctions();
						}
						handle_room_times += 1;
						if (handle_room_times >= 3)
						{
							// 自动寻路(); 未完成
							handle_room_times = 0;
						}
					}
				}
			}
		}

		if (dnf->judgeGameStatus() == 1 && change_user == false)
		{
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
		if (dnf->readInt(0x140000000) != 0x905A4D) {
			// 游戏结束
			statusChange = false;
			break;
		}

		if (dnf->judgeGameStatus() >= 1 && statusChange == false)
		{
			emptyAddress = dnf->C_EMPTY_ADDRESS + 4000;
			dnf->C_USER_POINTER = dnf->getUserPointer(emptyAddress);
			dnf->C_USER = emptyAddress;
			if (dnf->C_USER_POINTER == 0)
			{
				continue;
			}
			statusChange = true;

		}
		else if (dnf->judgeGameStatus() == 0)
		{
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

	__int64 head_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_HEAD_ADDRESS);
	__int64 end_address = readLong(readLong(readLong(readLong(C_USER) + C_MAP_OFFSET) + 16) + C_END_ADDRESS);
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
	if (MainDlg->_switch_score.GetCheck() == BST_CHECKED)
	{
		superScore();
	}

	if (MainDlg->_switch_cool_down.GetCheck() == BST_CHECKED)
	{
		CString num;
		MainDlg->_cool_down.GetWindowText(num);
		float number = (float)_ttof(num);
		skillCoolDown(number);
	}

	if (MainDlg->_switch_hook_damage.GetCheck() == BST_CHECKED)
	{
		hookDamage(true);
	}

	if (MainDlg->_switch_three_speed.GetCheck() == BST_CHECKED)
	{
		CString attack_speed, move_speed, casting_speed;
		MainDlg->_attack_speed.GetWindowText(attack_speed);
		MainDlg->_move_speed.GetWindowText(move_speed);
		MainDlg->_casting_speed.GetWindowText(casting_speed);
		threeSpeed(_ttoi(attack_speed), _ttoi(casting_speed), _ttoi(move_speed));
	}
}

void DNF::clearanceEvent()
{

}

void DNF::skillCoolDown(float num)
{
	encrypt(readLong(C_USER) + C_FLOAT_COOL_DOWN2, num);
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
	MainDlg->_damage_value.GetWindowText(value);

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
	asm_code = asm_code + makeByteArray({ 186 }) + intToBytes(direction);
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
BOOL DNF::runToDestination(int x, int y, bool is_room = false)
{
	if (x < 1 || y < 1) {
		return false;
	}

	__int64 a, b;
	if (is_room) {
		a = readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_X);
		b = readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_Y);
	}

	COORDINATE user_coor = readCoordinate(readLong(C_USER));

	CString start_x_str, start_y_str, end_x_str, end_y_str;
	start_x_str.Format(_T("人物坐标X:%d"), user_coor.x);
	start_y_str.Format(_T("人物坐标Y:%d"), user_coor.y);
	end_x_str.Format(_T("怪物坐标X:%d"), x);
	end_y_str.Format(_T("怪物坐标Y:%d"), y);
	MainDlg->Log(start_x_str);
	MainDlg->Log(start_y_str);
	MainDlg->Log(end_x_str);
	MainDlg->Log(end_y_str);

	bool x_arrived = false, y_arrived = false, isFirst = true;

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

	while (true) {

		user_coor = readCoordinate(readLong(C_USER));

		if (isFirst) {
			MainDlg->Log(L"重新跑图");
			M_KeyPress(MainDlg->msdk_handle, direction_x, 1);
			M_KeyDown(MainDlg->msdk_handle, direction_x);
			M_KeyDown(MainDlg->msdk_handle, direction_y);
		}

		// 如果切换到后台
		if (!windowIsTop()) {
			//MainDlg->Log(L"切换到后台，停止跑图");
			M_ReleaseAllKey(MainDlg->msdk_handle);
			break;
		}

		if (judgeGameStatus() != 3) {
			//MainDlg->Log(L"不在图内，停止跑图");
			M_ReleaseAllKey(MainDlg->msdk_handle);
			break;
		}

		// 判断人物动作
		//if (decrypt(readLong(C_USER) + C_MOVEMENT_ID) != 14) {
		//	MainDlg->Log(L"人物动作不符合，停止跑图");
		//	// 弹起移动按键
		//	keyboardUp(direction_x);
		//	keyboardUp(direction_y);
		//	break;
		//}

		// 判断房间：如果目标是下一个房间
		if (is_room) {
			if (readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_X) != a ||
				readLong(readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET) + C_CURRENT_ROOM_Y) != b) {
				// 弹起移动按键
				M_ReleaseAllKey(MainDlg->msdk_handle);
				break;
			}
		}

		// 优先到达Y坐标
		if (y_arrived == false) {
			if (direction_y == Keyboard_UpArrow) {
				if (y - user_coor.y > 10) {
					//MainDlg->Log(L"超出Y范围，停止");
					M_ReleaseAllKey(MainDlg->msdk_handle);
					break;
				}
			}
			else {
				if (user_coor.y - y > 10) {
					//MainDlg->Log(L"超出Y范围，停止");
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
				if (user_coor.x - x > 10) {
					//MainDlg->Log(L"超出X范围，停止");
					M_ReleaseAllKey(MainDlg->msdk_handle);
					break;
				}
			}
			else {
				if (x - user_coor.x > 10) {
					//MainDlg->Log(L"超出X范围，停止");
					M_ReleaseAllKey(MainDlg->msdk_handle);
					break;
				}
			}
			if (abs(user_coor.x - x) > 10) {
				isFirst = false;
				continue;
			}
			else {
				keyboardUp(direction_x);
				x_arrived = true;
			}
		}



		if (x_arrived && y_arrived) {
			M_ReleaseAllKey(MainDlg->msdk_handle);
			return true;
			break;
		}

		//programDelay(100);
	}

	//handleEvents();

	//MainDlg->Log(L"跑图结束");
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

	// 过图方向begin
	int x, y;

	// 地图数据begin
	__int64 room_data = readLong(readLong(readLong(C_ROOM_NUMBER) + C_TIME_ADDRESS) + C_DOOR_TYPE_OFFSET);
	int room_index = decrypt(room_data + C_MAP_CODE);
	int room_width = readInt(readLong(room_data + C_WH_OFFSET) + room_index * 8 + 0);
	int room_height = readInt(readLong(room_data + C_WH_OFFSET) + room_index * 8 + 4);
	long tmp_value = readLong(readLong(room_data + C_AISLE_OFFSET) + 32 * room_index + 8);
	int aisle_num = room_width * room_height;


	// 地图数据end

	//x = 地图数据.地图走法[1].x - 地图数据.地图走法[2].x;
	//y = 地图数据.地图走法[1].y - 地图数据.地图走法[2].y;

	int direction = 0;
	if (x == 0)
	{
		if (y == 1) {
			direction = 1;
		}
		else {
			direction = 2;
		}
	}
	if (y == 0) {
		if (x == 1) {
			direction = 3;
		}
		else {
			direction = 4;
		}
	}

	// 过图方向end

}