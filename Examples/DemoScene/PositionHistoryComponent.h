#pragma once

#include <glm/vec2.hpp>

struct PositionHistoryComponent {
	PositionHistoryComponent() = default;
	PositionHistoryComponent(PositionHistoryComponent&&) = default;
	PositionHistoryComponent& operator = (PositionHistoryComponent&&) = default;

	PositionHistoryComponent(const PositionHistoryComponent&) = delete;
	PositionHistoryComponent& operator = (const PositionHistoryComponent&) = delete;

	glm::vec2 Position;
};