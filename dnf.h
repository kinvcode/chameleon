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

struct DUNGEONOBJ
{
	__int64 p;
	int type;
	int camp;
	int code;
	int blood;
	COORDINATE coor;
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

	// 图内怪物列表
	std::vector<DUNGEONOBJ> _monster_list;

	// 图内物品列表
	std::vector<DUNGEONOBJ> _item_list;

	// 是否窗口置顶
	bool windowTop = false;

	// 7度基址区域
	__int64 C_USER; // 人物基址
	__int64 C_USER_POINTER; // 人物指针

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

	// 判断技能冷却
	int judgeCoolDown();

	// 判断是否是加百利商店
	bool judgeGabriel();

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

	// 通过物品列表进行聚物
	void gatherItemsByItems(std::vector<DUNGEONOBJ> items);

	// 全屏聚物+聚怪+聚建筑物
	void gatherAll();

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
	BOOL runToDestination(int x, int y, bool is_room, int target_range);

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

	// 获取冷却技能键位
	int getCoolDownKey();

	// 坐标CALL
	void coorCall(int x, int y, int z);

	// 获取图内所有需要处理的对象
	void getDungeonAllObj();
};