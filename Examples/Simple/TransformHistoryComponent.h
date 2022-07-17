#pragma once

struct PositionHistoryComponent {
	PositionHistoryComponent() = default;
	PositionHistoryComponent(PositionHistoryComponent&&) = default;
	PositionHistoryComponent& operator = (PositionHistoryComponent&&) = default;

	PositionHistoryComponent(const PositionHistoryComponent&) = delete;
	PositionHistoryComponent& operator = (const PositionHistoryComponent&) = delete;

	int Coord;
};