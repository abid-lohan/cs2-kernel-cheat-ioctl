#include "aimbot.h"
#include <iostream>

float aimbot::distance(vec3 p1, vec3 p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

void aimbot::frame() {
	uintptr_t entity_list = driver::read_memory<uintptr_t>(module_base + cs2_dumper::offsets::client_dll::dwEntityList);
	uintptr_t local_player_pawn = driver::read_memory<uintptr_t>(module_base + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
	if (!entity_list || !local_player_pawn) return;

	BYTE team = driver::read_memory<BYTE>(local_player_pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

	vec3 local_eye_pos = driver::read_memory<vec3>(local_player_pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin) +
		driver::read_memory<vec3>(local_player_pawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);

	float closest_enemy_distance = -1;
	vec3 enemy_pos;

	for (int i = 0; i < 32; i++) {
		uintptr_t list_entry = driver::read_memory<uintptr_t>(entity_list + ((8 * (i & 0x7ff) >> 9) + 16));
		if (!list_entry) continue;

		uintptr_t entity_controller = driver::read_memory<uintptr_t>(list_entry + 120 * (i & 0x1ff));
		if (!entity_controller) continue;

		uintptr_t entity_controller_pawn = driver::read_memory<uintptr_t>(entity_controller + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		if (!entity_controller_pawn) continue;

		uintptr_t entity_pawn = driver::read_memory<uintptr_t>(list_entry + 120 * (entity_controller_pawn & 0x1ff));
		if (!entity_pawn) continue;

		if (team == driver::read_memory<byte>(entity_pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum)) continue;

		if (driver::read_memory<uint32_t>(entity_pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) <= 0) continue;

		vec3 entity_eye_pos = driver::read_memory<vec3>(entity_pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin) +
			driver::read_memory<vec3>(entity_pawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);

		float current_distance = distance(local_eye_pos, entity_eye_pos);

		if (closest_enemy_distance < 0 || closest_enemy_distance > current_distance) {
			closest_enemy_distance = current_distance;
			enemy_pos = entity_eye_pos;
		}
	}

	vec3 relative_angle = (enemy_pos - local_eye_pos).RelativeAngle();
	driver::write_memory<vec3>(module_base + cs2_dumper::offsets::client_dll::dwViewAngles, relative_angle);
}