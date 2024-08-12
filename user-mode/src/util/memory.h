#pragma once

#include <Windows.h>
#include <TlHelp32.h>

namespace memory{
	inline HANDLE proc_handle;
	inline uintptr_t pid;

	uintptr_t get_pid(const wchar_t* process);
	uintptr_t get_module_base(uintptr_t procID, const wchar_t* module);
}
