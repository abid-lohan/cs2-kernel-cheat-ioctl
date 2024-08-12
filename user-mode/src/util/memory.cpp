#include "memory.h"

uintptr_t memory::get_pid(const wchar_t* procName) {
	DWORD procID = 0;
	HANDLE snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapHandle != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(snapHandle, &procEntry)) {
			do {
				if (!_wcsicmp(procEntry.szExeFile, procName)) {
					procID = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(snapHandle, &procEntry));
		}
	}

	CloseHandle(snapHandle);
	return procID;
}

uintptr_t memory::get_module_base(uintptr_t procID, const wchar_t* modName) {
	uintptr_t moduleBaseAddress = 0;
	HANDLE snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

	if (snapHandle != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(snapHandle, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					moduleBaseAddress = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(snapHandle, &modEntry));
		}
	}

	CloseHandle(snapHandle);
	return moduleBaseAddress;
}