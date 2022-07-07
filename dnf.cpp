#include "pch.h"
#include "dnf.h"
#include "chameleonDlg.h"
#include "constant.h"
#include "overloading.h"

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
		
		if (monster_type == 0x111 || monster_type == 211)
		{
			if (monster_camp !=0 && monster_blood != 0) 
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
		if (dnf->readInt(C_GAME_STATUS) == 3)
		{
			// 开启拾取
			if (false)
			{
				//全屏吸物
			}

			// 如果是第一个房间
			if (first_room == false && dnf->judgeClearance() == false)
			{
				first_room = true;

				// 开启图内功能
				//firstRoomFunctions();
				if (MainDlg->_switch_three_speed.GetCheck() == BST_CHECKED)
				{
					CString attack_speed, move_speed, casting_speed;
					MainDlg->_attack_speed.GetWindowText(attack_speed);
					MainDlg->_move_speed.GetWindowText(move_speed);
					MainDlg->_casting_speed.GetWindowText(casting_speed);
					dnf->threeSpeed(_ttoi(attack_speed), _ttoi(casting_speed), _ttoi(move_speed));
				}
				if (MainDlg->_switch_score.GetCheck() == BST_CHECKED)
				{
					dnf->superScore();
				}
			}

			// 如果已经通关
			if (clearance_judge == false && dnf->judgeClearance() == true)
			{
				first_room = false;
				clearance_judge = true;

				// 关闭图内功能
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

void DNF::manualThreadControl()
{
	// 开启线程逻辑
	AfxBeginThread(manualThread, this);
}

void DNF::superScore()
{
	writeLong(readLong(C_SCORE_ADDRESS) + C_CE_SCORE, 999999);
}

