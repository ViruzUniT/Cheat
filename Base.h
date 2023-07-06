#pragma once
#include "stdafx.h"
DWORD GetProcId(const wchar_t* proc_name);
uintptr_t GetModuleBaseAddress(const wchar_t* mod_name, DWORD proc_id);
uintptr_t FindDmaAddy(const HANDLE h_proc, const uintptr_t ptr, const std::vector<unsigned int>& offsets);