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

	// DNF���
	HANDLE handle = NULL;

	// ��ַ����
	__int64 C_USER = 0x1493C88A8; // �����ַ
	__int64 C_USER_POINTER; // ����ָ��
	__int64 C_USER_CALL = 0x143DAD2B0; // ����CALL
	int C_NAME_OFFSET = 0x838; // ���ƻ�ַ
	int C_SHOE_OFFSET = 0x5358; // Ь��ƫ��
	int C_ATTACK_SPEED = 0xC88; // �����ٶ�
	int C_MOVE_SPEED = 0xC80; // �ƶ��ٶ�
	int C_CASTING_SPEED = 0xC90; // �ͷ��ٶ�
	__int64 C_EMPTY_ADDRESS = 0x13FCCA9B0; // �հ׵�ַ
	__int64 C_ASM_CALL = 0x13FDC0000; // ���CALL
	__int64 C_SUMMON_FIGURE = 0x1439C82C0; // �ٻ���żCALL
	__int64 C_SUMMON_MONSTER = 0x1439B9560; // �ٻ�����CALL
	__int64 C_HIDDEN_CALL = 0x144CFA7F0; // ͸��CALL
	__int64 C_SKILL_CALL = 0x1438F55F0; // ����CALL
	__int64 C_GAME_STATUS = 0x14912D540; // ��Ϸ״̬
	__int64 C_ROOM_NUMBER = 0x14944ED18; // ������
	int C_TIME_ADDRESS = 0x20A050; // ʱ���ַ
	int C_DOOR_TYPE_OFFSET = 0x168; // ����ƫ��
	int C_BONFIRE_JUDGE = 0x1DA8; // �����ж�
	__int64 C_SCORE_ADDRESS = 0x14944D8B8; // ���ֻ�ַ
	int C_CE_SCORE = 0xC98; // CE����
	int C_DOOR_OFFSET = 0x1E4; // ����ƫ�ƣ��Ƿ��ţ�
	int C_MAP_OFFSET = 0x180; // ��ͼƫ��_1
	__int64 C_BOSS_ROOM_X = 0x1CF4; // BOSS����_X
	__int64 C_BOSS_ROOM_Y = 0x1CFC; // BOSS����_Y
	__int64 C_CURRENT_ROOM_X = 0x1BD8; // ��ǰ����_X
	__int64 C_CURRENT_ROOM_Y = 0x1BDC; // ��ǰ����_Y
	int C_MAP_HEAD = 0x148; // ��ͼ��ʼ
	int C_MAP_END = 0x150; // ��ͼ����
	int C_HEAD_ADDRESS = 0x148; // �׵�ַ
	int C_END_ADDRESS = 0x150; // β��ַ
	int C_TYPE_OFFSET = 0x154; // ����ƫ��
	int C_CAMP_OFFSET = 0xE60; // ��Ӫƫ��
	int C_CODE_OFFSET = 0x830; // ����ƫ��
	int C_MONSTER_BLOOD = 0x6038; // ����Ѫ��
	int C_READ_COORDINATE = 0x330; // ��ȡ����
	int C_OBJECT_COORDINATE = 0x168; // ��������
	__int64 C_AUTO_PICKUP = 0x143CB9F73; // �Զ�����
	int C_FLOAT_COOL_DOWN2 = 0x2288; // ������ȴ2
	__int64 C_GLOBAL_ADDRESS = 0x143C704BE; // ȫ�ֻ�ַ
	int C_PASS_ROOM_OFFSET = 0xF0; // ˳ͼƫ��
	__int64 C_COORDINATE_PASS_ROOM = 0x142D66F90; // ����˳ͼCALL

	// �������

	/**********[ ʵ �� �� �� �� �� ]***********/

	std::vector<byte> Ansi2Unicode(CStringA  str);

	CStringA Unicode2Ansi(std::vector<byte> vby);

	std::vector<byte> makeByteArray(std::vector<byte> data);

	std::vector<byte> makeEmptyByteArray(int length);

	std::vector<byte> intToBytes(__int64 length);


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
};