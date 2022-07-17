#pragma once

#include <glm/vec2.hpp>

struct RectangleComponent {
	RectangleComponent() = default;
	RectangleComponent(RectangleComponent&&) = default;
	RectangleComponent& operator = (RectangleComponent&&) = default;

	RectangleComponent(const RectangleComponent&) = delete;
	RectangleComponent& operator = (const RectangleComponent&) = delete;

	float Size;
};