#pragma once
#include "../util/memory.h"
#include "../util/vector.h"
#include "../util/definitions.cpp"

#include "../dump/client_dll.hpp"
#include "../dump/offsets.hpp"

namespace aimbot {
	inline uintptr_t module_base;
	inline uintptr_t pid;

	float distance(vec3 p1, vec3 p2);
	void frame();
}