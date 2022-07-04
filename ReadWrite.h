#pragma once
#include <iostream>
#include <TlHelp32.h>
#include <vector>
//#include "pch.h"
using namespace std;

class ReadWrite
{
public:
	ReadWrite();
	~ReadWrite();

	BOOL IsWin10System();

	static __int64 GetProcessModule(DWORD pid, wchar_t Name[]);

	static int asSystem(int Pid, char FileName[]);

	static bool processExists(wchar_t Name[]);

	static int TakePid(wchar_t Name[]);

	static DWORD GetPid();

	HANDLE ProcHandle;
	DWORD Pid;
	static DWORD GamePid;
	static 	HANDLE StaProcHandle;

private:

};



