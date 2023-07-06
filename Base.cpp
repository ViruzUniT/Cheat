#include "stdafx.h"

DWORD GetProcId(const wchar_t* proc_name)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_tcsicmp(procEntry.szExeFile, proc_name))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t GetModuleBaseAddress(const wchar_t* mod_name, DWORD proc_id)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry{};
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_tcsicmp(modEntry.szModule, mod_name))
				{
					modBaseAddr = reinterpret_cast<uintptr_t>(modEntry.modBaseAddr);
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t FindDmaAddy(const HANDLE h_proc, const uintptr_t ptr, const std::vector<unsigned int>& offsets) {
	uintptr_t addr = ptr;
	for (const size_t offset : offsets) {
		ReadProcessMemory(h_proc, (BYTE*)addr, &addr, sizeof(addr), nullptr);
		addr += offset;
	}
	return addr;
}

