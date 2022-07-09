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
	
	// DNF���
	HANDLE handle = NULL;

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
};