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

	// DNF���
	HANDLE handle = NULL;

	// �Ƿ񴰿��ö�
	bool windowTop = false;

	// 7�Ȼ�ַ����
	__int64 C_USER = 0x14974D7B0; // �����ַ
	__int64 C_USER_POINTER; // ����ָ��
	__int64 C_USER_CALL = 0x143DD4CA0; // ����CALL
	int C_NAME_OFFSET = 0x840; // ����ƫ��
	int C_SHOE_OFFSET = 0x5360; // Ь��ƫ��
	int C_ATTACK_SPEED = 0xC88; // �����ٶ�
	int C_MOVE_SPEED = 0xC80; // �ƶ��ٶ�
	int C_CASTING_SPEED = 0xC90; // �ͷ��ٶ�
	__int64 C_EMPTY_ADDRESS = 0x13FCA04C0; // ȫ�ֿհ�
	__int64 C_ASM_CALL = 0x13FDC0000; // ���CALL
	__int64 C_SUMMON_FIGURE = 0x1439EE750; // �ٻ���żCALL
	__int64 C_SUMMON_MONSTER = 0x1439DF9F0; // �ٻ�����CALL
	__int64 C_HIDDEN_CALL = 0x144D259C0; // ͸��CALL
	__int64 C_SKILL_CALL = 0x14391B5A0; // ����CALL
	__int64 C_GAME_STATUS = 0x14917D5E0; // ��Ϸ״̬
	__int64 C_ROOM_NUMBER = 0x14949FC98; // ������
	int C_TIME_ADDRESS = 0x20A050; // ʱ���ַ
	int C_DOOR_TYPE_OFFSET = 0x170; // ����ƫ��
	int C_BONFIRE_JUDGE = 0x1DA8; // �����ж�
	__int64 C_SCORE_ADDRESS = 0x14949E838; // ���ֻ�ַ
	int C_CE_SCORE = 0xC98; // C_E_����
	int C_DOOR_OFFSET = 0x1E4; // �Ƿ���
	int C_MAP_OFFSET = 0x180; // ��ͼƫ��
	__int64 C_BOSS_ROOM_X = 0x1CE4; // BOSS����X
	__int64 C_BOSS_ROOM_Y = 0x1CEC; // BOSS����Y
	__int64 C_CURRENT_ROOM_X = 0x1BD8; // ��ǰ����X
	__int64 C_CURRENT_ROOM_Y = 0x1BDC; // ��ǰ����Y
	__int64 C_BEGIN_ROOM_X = 0x178; // ��ʼ����X
	__int64 C_BEGIN_ROOM_Y = 0x17C; // ��ʼ����Y
	int C_MAP_HEAD = 0x148; // ��ͼ��ʼ2
	int C_MAP_END = 0x150; // ��ͼ����2
	int C_HEAD_ADDRESS = 0x148; // �׵�ַ
	int C_END_ADDRESS = 0x150; // β��ַ
	int C_TYPE_OFFSET = 0x154; // ����ƫ��
	int C_CAMP_OFFSET = 0xE68; // ��Ӫƫ��
	int C_CODE_OFFSET = 0x838; // ����ƫ��
	int C_MONSTER_BLOOD = 0x6040; // ����Ѫ��
	int C_READ_COORDINATE = 0x338; // ��ȡ����
	int C_OBJECT_COORDINATE = 0x168; // ��������
	__int64 C_GLOBAL_ADDRESS = 0x143C96C4E; // ȫ�ֻ�ַ
	int C_PASS_ROOM_OFFSET = 0xF0; // ˳ͼƫ��
	__int64 C_COORDINATE_PASS_ROOM = 0x142D8C0F0; // ����˳ͼCALL
	__int64 C_AUTO_PICKUP = 0x143CE103A; // �Զ�����
	int C_MOVEMENT_ID = 0x5450; // ����ID
	int C_PENETRATE_MAP = 0x848; // ��ͼ��͸
	int C_PENETRATE_BUILDING = 0x84C; // ������͸
	int C_WH_OFFSET = 0x7A0; // ���ƫ��
	int C_AISLE_OFFSET = 0x7C0; // ����ƫ��|ͨ��ƫ��
	int C_FLOAT_COOL_DOWN2 = 0x2290; // ������ȴ2
	int C_MAP_CODE = 0x1CCC; // ����ƫ��|��ͼ����(����)


	// �������

	/**********[ ʵ �� �� �� �� �� ]***********/

	std::vector<byte> Ansi2Unicode(CStringA  str);

	CStringA Unicode2Ansi(std::vector<byte> vby);

	std::vector<byte> makeByteArray(std::vector<byte> data);

	std::vector<byte> makeEmptyByteArray(int length);

	std::vector<byte> intToBytes(__int64 length);
	std::vector<byte> intToBytes2(int length);


	/**********[ �� �� �� �� �� �� ]***********/

	// �¼�����
	void handleEvents();

	// �����ӳ�
	bool programDelay(int time, int delayUnit);

	/**********[ �� д �� �� �� �� ]***********/

	int readInt(__int64 address);

	__int64 readLong(__int64 address);

	float readFloat(__int64 address);

	bool writeInt(__int64 address, __int64 value);

	bool writeFloat(__int64 address, float value);

	bool writeLong(__int64 address, __int64 value);

	std::vector<byte> readByteArray(__int64 address, int length);

	bool writeByteArray(__int64 address, std::vector<byte> Data);

	/**********[ �� Ϸ �� �� �� �� ]***********/

	void encrypt(__int64 address, int value);

	__int64 decrypt(__int64 address);
	__int64 decrypt2(__int64 address);

	void memoryAssambly(std::vector<byte>asm_code);

	// �ı��ɫ����
	void changeUserName(CStringA name);

	// ����
	void threeSpeed(int attack, int casting, int move);

	// �ٻ���ż
	void summonFigure(int code);

	// �ٻ�����
	void summonMonster(int code);

	// ͸���޵�
	void hiddenUser();

	// ����CALL
	void skillCall(__int64 pointer, int code, __int64 damage, int x, int y, int z, int skillSize);

	// ��ȡBOSS��������
	COORDINATE judgeBossRoom();

	// ��ȡ��ǰ��ɫ����
	COORDINATE judgeCurrentRoom();

	// �ж��Ƿ��й���
	bool judgeHaveMonster();

	// �ж��Ƿ�����Ʒ
	bool judgeHaveItem();

	// �ж���Ϸ״̬
	int judgeGameStatus();

	// �ж��Ƿ���
	bool judgeDoorOpen();

	// �ж��Ƿ�ͨ��
	bool judgeClearance();

	// �жϵ�ǰ�Ƿ���BOSS����
	bool judgeIsBossRoom();

	// �ֶ��߳̿���
	void manualThreadControl();

	// �Զ��߳̿���
	void autoThreadControl();

	// ����ָ���߳�
	void userPonterUpdate();

	// ��������
	void superScore();

	// ȫ������
	void gatherItems();

	// ��ȡ����
	COORDINATE readCoordinate(__int64 address);

	// ��ͼ����
	void firstRoomFunctions();

	// ͨ�ش���
	void clearanceEvent();

	// ������ȴ����
	void skillCoolDown(float num);

	// ��ȡ����ָ��
	__int64 getUserPointer(__int64 emptyAddress);

	// HOOK�˺�
	void hookDamage(bool on);

	// ˳ͼ����
	__int64 passRoomData(int direction);

	// �ر�ͼ�ڹ���
	void closeDungeonFunctions();

	// �ܵ�Ŀ��
	BOOL runToDestination(int x, int y, bool is_room);

	// �ߵ�Ŀ��
	void walkToDestination();

	// ����ָ������
	void keyboardUp(int key);

	// ��Ϸ�����Ƿ��ö�
	bool windowIsTop();

	// ��ͼ�ͽ����ﴩ͸
	void penetrate(bool on);

	// �Զ��¸�����
	void autoNextRoom();

	// �ж��¸�����ķ���
	int judgeNextRoomDiretion(COORDINATE current, COORDINATE boss);

	// ��ͼ����
	DUNGEONMAP mapData();

	// ��ȡ�߷�
	int getWay(std::vector<int> aisle, int width, int height, COORDINATE begin, COORDINATE end, std::vector<COORDINATE>& way);

	// ���ɵ�ͼ
	void createMap(int width, int height, std::vector<int> aisle, std::vector<std::vector<AISLEDATA>>& map);

	// ��ʾ��ͼ
	void showMap(std::vector<std::vector<AISLEDATA>> map, int width, int height, std::vector<std::vector<AISLEDATA>>& tag);

	// �жϷ���
	bool judgeDirection(int aisle, int direction);
	
	// ·���㷨
	void pathCalc(std::vector<std::vector<AISLEDATA>> map_tag, COORDINATE begin, COORDINATE end, int width, int height, std::vector<COORDINATE>& cross_way);

	// ��������
	int arrangeCoor(std::vector<COORDINATE>imitation, std::vector<COORDINATE>& real_cross);

	// ���ܽ�����һ������
	void runToNextRoom(int direction);
};