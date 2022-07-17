#pragma once

struct PositionComponent {
	PositionComponent(int coord) : Coord(coord) {}
	PositionComponent(PositionComponent&&) = default;
	PositionComponent& operator = (PositionComponent&&) = default;

	PositionComponent(const PositionComponent&) = delete;
	PositionComponent& operator = (const PositionComponent&) = delete;

	int Coord;
};