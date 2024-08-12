#pragma once

#include <cmath>
#include <numbers>

struct vec2
{
	float x, y;
};

struct vec4
{
	float w, x, y, z;
};

struct vec3
{
	float x, y, z;
	vec3 operator+(vec3 second) {
		return { this->x + second.x, this->y + second.y, this->z + second.z };
	}

	vec3 operator-(vec3 second) {
		return { this->x - second.x, this->y - second.y, this->z - second.z };
	}

	vec3 RelativeAngle() {
		return {
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f
		};
	}
};