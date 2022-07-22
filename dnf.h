#pragma once
#include <vector>

struct COORDINATE {
	int x;
	int y;
	int z;
};

struct DUNGEONMAP
{
	char* map_name;
	int map_number;
	std::vector<int> aisle;
	COORDINATE begin;
	COORDINATE end;
	int width;
	int height;
	std::vector<COORDINATE> way;
	int fatigue;
	int aisle_num;
	__int64 tmp;
};

struct AISLEDATA
{
	COORDINATE coor;
	bool left;
	bool right;
	bool top;
	bool bottom;
	int aisle;
	int bg;
};

struct MAPNODE
{
	DWORD f;
	DWORD g;
	DWORD h;
	COORDINATE current;
	COORDINATE final;
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

	// 是否窗口置顶
	bool windowTop = false;

	// 7度基址区域
	__int64 C_USER = 0x14974D7B0; // 人物基址
	__int64 C_USER_POINTER; // 人物指针
	__int64 C_USER_CALL = 0x143DD4CA0; // 人物CALL
	int C_NAME_OFFSET = 0x840; // 名称偏移
	int C_SHOE_OFFSET = 0x5360; // 鞋子偏移
	int C_ATTACK_SPEED = 0xC88; // 攻击速度
	int C_MOVE_SPEED = 0xC80; // 移动速度
	int C_CASTING_SPEED = 0xC90; // 释放速度
	__int64 C_EMPTY_ADDRESS = 0x13FCA04C0; // 全局空白
	__int64 C_ASM_CALL = 0x13FDC0000; // 汇编CALL
	__int64 C_SUMMON_FIGURE = 0x1439EE750; // 召唤人偶CALL
	__int64 C_SUMMON_MONSTER = 0x1439DF9F0; // 召唤怪物CALL
	__int64 C_HIDDEN_CALL = 0x144D259C0; // 透明CALL
	__int64 C_SKILL_CALL = 0x14391B5A0; // 技能CALL
	__int64 C_GAME_STATUS = 0x14917D5E0; // 游戏状态
	__int64 C_ROOM_NUMBER = 0x14949FC98; // 房间编号
	int C_TIME_ADDRESS = 0x20A050; // 时间基址
	int C_DOOR_TYPE_OFFSET = 0x170; // 门型偏移
	int C_BONFIRE_JUDGE = 0x1DA8; // 篝火判断
	__int64 C_SCORE_ADDRESS = 0x14949E838; // 评分基址
	int C_CE_SCORE = 0xC98; // C_E_评分
	int C_DOOR_OFFSET = 0x1E4; // 是否开门
	int C_MAP_OFFSET = 0x180; // 地图偏移
	__int64 C_BOSS_ROOM_X = 0x1CE4; // BOSS房间X
	__int64 C_BOSS_ROOM_Y = 0x1CEC; // BOSS房间Y
	__int64 C_CURRENT_ROOM_X = 0x1BD8; // 当前房间X
	__int64 C_CURRENT_ROOM_Y = 0x1BDC; // 当前房间Y
	__int64 C_BEGIN_ROOM_X = 0x178; // 起始坐标X
	__int64 C_BEGIN_ROOM_Y = 0x17C; // 起始坐标Y
	int C_MAP_HEAD = 0x148; // 地图开始2
	int C_MAP_END = 0x150; // 地图结束2
	int C_HEAD_ADDRESS = 0x148; // 首地址
	int C_END_ADDRESS = 0x150; // 尾地址
	int C_TYPE_OFFSET = 0x154; // 类型偏移
	int C_CAMP_OFFSET = 0xE68; // 阵营偏移
	int C_CODE_OFFSET = 0x838; // 代码偏移
	int C_MONSTER_BLOOD = 0x6040; // 怪物血量
	int C_READ_COORDINATE = 0x338; // 读取坐标
	int C_OBJECT_COORDINATE = 0x168; // 对象坐标
	__int64 C_GLOBAL_ADDRESS = 0x143C96C4E; // 全局基址
	int C_PASS_ROOM_OFFSET = 0xF0; // 顺图偏移
	__int64 C_COORDINATE_PASS_ROOM = 0x142D8C0F0; // 坐标顺图CALL
	__int64 C_AUTO_PICKUP = 0x143CE103A; // 自动捡物
	int C_MOVEMENT_ID = 0x5450; // 动作ID
	int C_PENETRATE_MAP = 0x848; // 地图穿透
	int C_PENETRATE_BUILDING = 0x84C; // 建筑穿透
	int C_WH_OFFSET = 0x7A0; // 宽高偏移
	int C_AISLE_OFFSET = 0x7C0; // 数组偏移|通道偏移
	int C_FLOAT_COOL_DOWN2 = 0x2290; // 浮点冷却2
	int C_MAP_CODE = 0x1CCC; // 索引偏移|地图编码(心悦)


	// 组包数据

	/**********[ 实 用 函 数 区 域 ]***********/

	std::vector<byte> Ansi2Unicode(CStringA  str);

	CStringA Unicode2Ansi(std::vector<byte> vby);

	std::vector<byte> makeByteArray(std::vector<byte> data);

	std::vector<byte> makeEmptyByteArray(int length);

	std::vector<byte> intToBytes(__int64 length);
	std::vector<byte> intToBytes2(int length);


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
	__int64 decrypt2(__int64 address);

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

	// 自动线程控制
	void autoThreadControl();

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

	// 关闭图内功能
	void closeDungeonFunctions();

	// 跑到目标
	BOOL runToDestination(int x, int y, bool is_room);

	// 走到目标
	void walkToDestination();

	// 弹起指定按键
	void keyboardUp(int key);

	// 游戏窗口是否置顶
	bool windowIsTop();

	// 地图和建筑物穿透
	void penetrate(bool on);

	// 自动下个房间
	void autoNextRoom();

	// 判断下个房间的方向
	int judgeNextRoomDiretion(COORDINATE current, COORDINATE boss);

	// 地图数据
	DUNGEONMAP mapData();

	// 获取走法
	int getWay(std::vector<int> aisle, int width, int height, COORDINATE begin, COORDINATE end, std::vector<COORDINATE>& way);

	// 生成地图
	void createMap(int width, int height, std::vector<int> aisle, std::vector<std::vector<AISLEDATA>>& map);

	// 显示地图
	void showMap(std::vector<std::vector<AISLEDATA>> map, int width, int height, std::vector<std::vector<AISLEDATA>>& tag);

	// 判断方向
	bool judgeDirection(int aisle, int direction);
	
	// 路径算法
	void pathCalc(std::vector<std::vector<AISLEDATA>> map_tag, COORDINATE begin, COORDINATE end, int width, int height, std::vector<COORDINATE>& cross_way);

	// 整理坐标
	int arrangeCoor(std::vector<COORDINATE>imitation, std::vector<COORDINATE>& real_cross);

	// 奔跑进入下一个房间
	void runToNextRoom(int direction);
};