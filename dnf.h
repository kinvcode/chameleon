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

	// DNF PID
	DWORD PID;
	
	// DNF句柄
	HANDLE handle = NULL;

	// 组包数据

	/**********[ 实 用 函 数 区 域 ]***********/

	std::vector<byte> Ansi2Unicode(CStringA  str);

	CStringA Unicode2Ansi(std::vector<byte> vby);

	std::vector<byte> makeByteArray(std::vector<byte> data);

	std::vector<byte> makeEmptyByteArray(int length);

	std::vector<byte> intToBytes(__int64 length);


	/**********[ 进 程 函 数 区 域 ]***********/

	// 事件处理
	void handleEvents();

	// 程序延迟
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

	// 改变角色名称
	void changeUserName(CStringA name);

	// 三速
	void threeSpeed(int attack, int casting, int move);

	// 召唤人偶
	void summonFigure(int code);

	// 召唤怪物
	void summonMonster(int code);

	// 透明无敌
	void hiddenUser();

	// 技能CALL
	void skillCall(__int64 pointer, int code, __int64 damage, int x, int y, int z, int skillSize);

	// 获取BOSS房间坐标
	COORDINATE judgeBossRoom();

	// 获取当前角色房间
	COORDINATE judgeCurrentRoom();

	// 判断是否有怪物
	bool judgeHaveMonster();

	// 判断是否有物品
	bool judgeHaveItem();

	// 判断游戏状态
	int judgeGameStatus();

	// 判断是否开门
	bool judgeDoorOpen();

	// 判断是否通关
	bool judgeClearance();

	// 判断当前是否是BOSS房间
	bool judgeIsBossRoom();

	// 手动线程控制
	void manualThreadControl();

	// 超级评分
	void superScore();

	// 全屏聚物
	void gatherItems();

	// 读取坐标
	COORDINATE readCoordinate(__int64 address);

	// 首图功能
	void firstRoomFunctions();

	// 通关处理
	void clearanceEvent();

	// 技能冷却缩减
	void skillCoolDown(float num);
};