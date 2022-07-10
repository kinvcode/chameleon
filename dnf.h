#pragma once
#include <vector>

struct COORDINATE {
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

	// 基址区域
	__int64 C_USER = 0x1493C88A8; // 人物基址
	__int64 C_USER_POINTER; // 人物指针
	__int64 C_USER_CALL = 0x143DAD2B0; // 人物CALL
	int C_NAME_OFFSET = 0x838; // 名称基址
	int C_SHOE_OFFSET = 0x5358; // 鞋子偏移
	int C_ATTACK_SPEED = 0xC88; // 攻击速度
	int C_MOVE_SPEED = 0xC80; // 移动速度
	int C_CASTING_SPEED = 0xC90; // 释放速度
	__int64 C_EMPTY_ADDRESS = 0x13FCCA9B0; // 空白地址
	__int64 C_ASM_CALL = 0x13FDC0000; // 汇编CALL
	__int64 C_SUMMON_FIGURE = 0x1439C82C0; // 召唤人偶CALL
	__int64 C_SUMMON_MONSTER = 0x1439B9560; // 召唤怪物CALL
	__int64 C_HIDDEN_CALL = 0x144CFA7F0; // 透明CALL
	__int64 C_SKILL_CALL = 0x1438F55F0; // 技能CALL
	__int64 C_GAME_STATUS = 0x14912D540; // 游戏状态
	__int64 C_ROOM_NUMBER = 0x14944ED18; // 房间编号
	int C_TIME_ADDRESS = 0x20A050; // 时间基址
	int C_DOOR_TYPE_OFFSET = 0x168; // 门型偏移
	int C_BONFIRE_JUDGE = 0x1DA8; // 篝火判断
	__int64 C_SCORE_ADDRESS = 0x14944D8B8; // 评分基址
	int C_CE_SCORE = 0xC98; // CE评分
	int C_DOOR_OFFSET = 0x1E4; // 开门偏移（是否开门）
	int C_MAP_OFFSET = 0x180; // 地图偏移_1
	__int64 C_BOSS_ROOM_X = 0x1CF4; // BOSS房间_X
	__int64 C_BOSS_ROOM_Y = 0x1CFC; // BOSS房间_Y
	__int64 C_CURRENT_ROOM_X = 0x1BD8; // 当前房间_X
	__int64 C_CURRENT_ROOM_Y = 0x1BDC; // 当前房间_Y
	int C_MAP_HEAD = 0x148; // 地图开始
	int C_MAP_END = 0x150; // 地图结束
	int C_HEAD_ADDRESS = 0x148; // 首地址
	int C_END_ADDRESS = 0x150; // 尾地址
	int C_TYPE_OFFSET = 0x154; // 类型偏移
	int C_CAMP_OFFSET = 0xE60; // 阵营偏移
	int C_CODE_OFFSET = 0x830; // 代码偏移
	int C_MONSTER_BLOOD = 0x6038; // 怪物血量
	int C_READ_COORDINATE = 0x330; // 读取坐标
	int C_OBJECT_COORDINATE = 0x168; // 对象坐标
	__int64 C_AUTO_PICKUP = 0x143CB9F73; // 自动捡物
	int C_FLOAT_COOL_DOWN2 = 0x2288; // 浮点冷却2
	__int64 C_GLOBAL_ADDRESS = 0x143C704BE; // 全局基址
	int C_PASS_ROOM_OFFSET = 0xF0; // 顺图偏移
	__int64 C_COORDINATE_PASS_ROOM = 0x142D66F90; // 坐标顺图CALL

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

	// 人物指针线程
	void userPonterUpdate();

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

	// 获取人物指针
	__int64 getUserPointer(__int64 emptyAddress);

	// HOOK伤害
	void hookDamage(bool on);

	// 顺图数据
	__int64 passRoomData(int direction);
};