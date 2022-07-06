#include "pch.h"
#include "dnf.h"
#include "chameleonDlg.h"
#include "constant.h"
#include "overloading.h"


extern CchameleonDlg* MainDlg;

DNF::DNF(DWORD PID)
{
	this->PID = PID;
	//this->MainWindow = dlg;
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

UINT manualThread(LPVOID pParam)
{
	// 手动逻辑处理
	AfxMessageBox(L"手动线程开启");

	return 0;   // thread completed successfully
}

void DNF::manualThreadControl()
{
	// 开启线程逻辑
	AfxBeginThread(manualThread, this);
}

