#pragma once
#include <vector>

struct COORDINATE{
	int x;
	int y;
	int z;
};

class DNF
{
public:
	DNF(DWORD PID);
	~DNF();
	DWORD PID;
	HANDLE handle = NULL;

	/**********[ 实 用 函 数 区 域 ]***********/

	std::vector<byte> Ansi2Unicode(CStringA  str);

	CStringA Unicode2Ansi(std::vector<byte> vby);

	std::vector<byte> makeByteArray(std::vector<byte> data);

	std::vector<byte> makeEmptyByteArray(int length);

	std::vector<byte> intToBytes(__int64 length);


	/**********[ 进 程 函 数 区 域 ]***********/

	void handleEvents();

	bool programDelay(int time, int delayUnit);

	/**********[ 读 写 函 数 区 域 ]***********/

	int readInt(__int64 address);

	__int64 readLong(__int64 address);

	float readFloat(__int64 address);

	bool writeInt(__int64 address, __int64 value);

	bool writeFloat(__int64 address, float value);

	bool writeLong(__int64 address, __int64 value);

	std::vector<byte> readByteArray(__int64 address, int length);

	bool writeByteArray(__int64 address, std::vector<byte> Data);

	/**********[ 游 戏 功 能 区 域 ]***********/

	void encrypt(__int64 address, int value);

	__int64 decrypt(__int64 address);

	void memoryAssambly(std::vector<byte>asm_code);

	void changeUserName(CStringA name);

	void threeSpeed(int attack, int casting, int move);

	void summonFigure(int code);

	void summonMonster(int code);

	void hiddenUser();

	void skillCall(__int64 pointer, int code, __int64 damage, int x, int y, int z, int skillSize);

	COORDINATE judgeBossRoom();

	COORDINATE judgeCurrentRoom();

	bool judgeHaveMonster();

	bool judgeHaveItem();

	int judgeGameStatus();

	bool judgeDoorOpen();

	bool judgeClearance();

	bool judgeIsBossRoom();

	void manualThreadControl();

	void superScore();

	void gatherItems();

	COORDINATE readCoordinate(__int64 address);

	void firstRoomFunctions();

	void clearanceEvent();
};