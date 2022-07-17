#pragma once

#include <glm/vec2.hpp>

struct PositionComponent {
	PositionComponent(const glm::vec2& position) : Position(position) {}
	PositionComponent(PositionComponent&&) = default;
	PositionComponent& operator = (PositionComponent&&) = default;

	PositionComponent(const PositionComponent&) = delete;
	PositionComponent& operator = (const PositionComponent&) = delete;

	glm::vec2 Position;
};