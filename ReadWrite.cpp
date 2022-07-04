#include "pch.h"
#include "ReadWrite.h"

DWORD ReadWrite::GamePid = 0;
HANDLE ReadWrite::StaProcHandle = 0;
ReadWrite::ReadWrite()
{

}

ReadWrite::~ReadWrite()
{

}

BOOL ReadWrite::IsWin10System()
{

	std::string vname;
	// 先判断是否为win8.1或win10
	typedef void(__stdcall* NTPROC)(DWORD*, DWORD*, DWORD*);
	DWORD dwMajor, dwMinor, dwBuildNumber;
	NTPROC proc = (NTPROC)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "RtlGetNtVersionNumbers");
	proc(&dwMajor, &dwMinor, &dwBuildNumber);

	if (dwMajor == 10 && dwMinor == 0)
	{
		return TRUE;
	}
	return FALSE;
}

DWORD ReadWrite::GetPid()
{
	return GamePid;
}

__int64 ReadWrite::GetProcessModule(DWORD pid, wchar_t Name[])
{

	MODULEENTRY32 module = { 0 };

	__int64 temp = 0;

	HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetPid());
	HANDLE modules = 0;
	if (hModuleSnap != 0)
	{
		module.dwSize = sizeof(MODULEENTRY32);

		modules = (HANDLE)Module32First(hModuleSnap, &module);

		while (modules != 0)
		{
			if (lstrcmpi(module.szModule, Name) == 0)
			{
				CloseHandle(hModuleSnap);

				temp = (__int64)module.modBaseAddr;
				return temp;
			}

			modules = (HANDLE)Module32Next(hModuleSnap, &module);
		}
		CloseHandle(hModuleSnap);
	}

	return -1;
}


int ReadWrite::asSystem(int Pid, char FileName[])
{

	BOOLEAN  nEn = 0;
	SIZE_T cbAttributeListSize = 0;
	HANDLE ProcessHandle = 0;
	PPROC_THREAD_ATTRIBUTE_LIST ps = 0;
	STARTUPINFOEXA si;
	ZeroMemory(&si, sizeof(si));
	si.StartupInfo.cb = sizeof(si);

	typedef LONG(*type_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
	type_RtlAdjustPrivilege RtlAdjustPrivilege = (type_RtlAdjustPrivilege)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "RtlAdjustPrivilege");
	RtlAdjustPrivilege(20, 1, 0, &nEn);
	ProcessHandle = OpenProcess(2035711, FALSE, Pid);
	InitializeProcThreadAttributeList(NULL, 1, 0, &cbAttributeListSize);
	char* temp = new char[cbAttributeListSize];
	PPROC_THREAD_ATTRIBUTE_LIST AttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)temp;
	InitializeProcThreadAttributeList(AttributeList, 1, 0, &cbAttributeListSize);

	if (!UpdateProcThreadAttribute(AttributeList, 0, 131072, &ProcessHandle, sizeof(HANDLE), NULL, NULL))
	{
		MessageBoxW(NULL, L"UpdateProcThreadAttribute 失败 !", L"凉城", MB_OK);
		ExitProcess(0);
	}
	si.lpAttributeList = AttributeList;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	si.StartupInfo.cb = 68;
	si.StartupInfo.dwFlags = 0;
	si.StartupInfo.wShowWindow = 1;
	if (CreateProcessAsUserA(NULL, 0, FileName, 0, 0, 0, 524288, 0, 0, (LPSTARTUPINFOA)&si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		MessageBoxW(NULL, L"CreateProcessAsUserA 失败!", L"凉城", MB_OK);
		ExitProcess(0);
	}
	CloseHandle(ProcessHandle);
	DeleteProcThreadAttributeList(AttributeList);
	delete temp;
	return pi.dwProcessId;
}

bool ReadWrite::processExists(wchar_t Name[])
{

	PROCESSENTRY32 Processinformation = { 0 };
	HANDLE Processhandle, Processsnapshot;

	Processsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Processsnapshot != 0)
	{
		Processinformation.dwSize = sizeof(PROCESSENTRY32);
		Processhandle = (HANDLE)Process32First(Processsnapshot, &Processinformation);
		while (Processhandle != 0)
		{
			if (lstrcmpi(Name, Processinformation.szExeFile) == 0)
			{
				CloseHandle(Processsnapshot);

				return  true;
			}
			Processhandle = (HANDLE)Process32Next(Processsnapshot, &Processinformation);
		}
		CloseHandle(Processsnapshot);
	}
	return  false;
}

int ReadWrite::TakePid(wchar_t Name[])
{

	PROCESSENTRY32 Processinformation = { 0 };
	HANDLE Processhandle, Processsnapshot;

	Processsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Processsnapshot != 0)
	{
		Processinformation.dwSize = sizeof(PROCESSENTRY32);
		Processhandle = (HANDLE)Process32First(Processsnapshot, &Processinformation);
		while (Processhandle != 0)
		{
			if (lstrcmpi(Name, Processinformation.szExeFile) == 0)
			{
				CloseHandle(Processsnapshot);

				return Processinformation.th32ProcessID;
			}
			Processhandle = (HANDLE)Process32Next(Processsnapshot, &Processinformation);
		}
		CloseHandle(Processsnapshot);

	}
	return 0;
}