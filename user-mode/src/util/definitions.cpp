#pragma once
#include <Windows.h>

namespace driver {
	inline HANDLE driver_handle;

	namespace ctl {
		constexpr ULONG attach = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1337, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		constexpr ULONG read = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1338, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		constexpr ULONG write = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1339, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	}

	struct req {
		HANDLE process_id;
		PVOID target;
		PVOID buffer;
		SIZE_T size;
		SIZE_T return_size;
	};

	inline bool attach_to_process(const uintptr_t pid) {
		req r;
		r.process_id = reinterpret_cast<HANDLE>(pid);

		return DeviceIoControl(driver_handle, ctl::attach, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
	}

	template <class T>
	T read_memory(const uintptr_t address) {
		T temp = {};

		req r;
		r.target = reinterpret_cast<PVOID>(address);
		r.buffer = &temp;
		r.size = sizeof(T);

		DeviceIoControl(driver_handle, ctl::read, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);

		return temp;
	}

	template <class T>
	void write_memory(const uintptr_t address, const T& value) {
		req r;
		r.target = reinterpret_cast<PVOID>(address);
		r.buffer = (PVOID)&value;
		r.size = sizeof(T);

		DeviceIoControl(driver_handle, ctl::write, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
	}
}