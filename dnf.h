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

	// DNF���
	HANDLE handle = NULL;

	// ͼ�ڹ����б�
	std::vector<DUNGEONOBJ> _monster_list;

	// ͼ����Ʒ�б�
	std::vector<DUNGEONOBJ> _item_list;

	// �Ƿ񴰿��ö�
	bool windowTop = false;

	// 7�Ȼ�ַ����
	__int64 C_USER; // �����ַ
	__int64 C_USER_POINTER; // ����ָ��

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

	// �жϼ�����ȴ
	int judgeCoolDown();

	// �ж��Ƿ��ǼӰ����̵�
	bool judgeGabriel();

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

	// ͨ����Ʒ�б���о���
	void gatherItemsByItems(std::vector<DUNGEONOBJ> items);

	// ȫ������+�۹�+�۽�����
	void gatherAll();

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
	BOOL runToDestination(int x, int y, bool is_room, int target_range);

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

	// ��ȡ��ȴ���ܼ�λ
	int getCoolDownKey();

	// ����CALL
	void coorCall(int x, int y, int z);

	// ��ȡͼ��������Ҫ����Ķ���
	void getDungeonAllObj();
};