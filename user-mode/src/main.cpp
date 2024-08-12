#include <iostream>
#include <thread>

#include "util/definitions.cpp"
#include "modules/aimbot.h"

int main() {
	aimbot::pid = memory::get_pid(L"cs2.exe");
	aimbot::module_base = memory::get_module_base(aimbot::pid, L"client.dll");

	if (aimbot::pid == 0 || aimbot::module_base == 0) {
		std::cout << "[x] Failed to find Assault Cube module.\n";
		std::cin.get();

		return 1;
	}

	std::cout << "[!] Module base: 0x" << std::hex << aimbot::module_base << ".\n";

	driver::driver_handle = CreateFile(L"\\\\.\\IoctlDriver", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (driver::driver_handle == INVALID_HANDLE_VALUE) {
		std::cout << "[x] Failed to create driver handle.\n";
		std::cin.get();

		return 1;
	}

	if (driver::attach_to_process(aimbot::pid) == false) {
		std::cout << "[x] Failed to attach to process.\n";
		std::cin.get();

		return 1;
	}

	std::cout << "[!] Attached successfully.\n";
	std::cout << "[!] Cheat started. Press LSHIFT to aimbot.\n";

	while (true) {
		if (GetAsyncKeyState(VK_LSHIFT)) {
			aimbot::frame();
		}
	}

	CloseHandle(driver::driver_handle);
	std::cin.get();
	return 0;
}