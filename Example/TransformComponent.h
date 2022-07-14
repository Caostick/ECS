#pragma once

struct TransformComponent {
	TransformComponent(int coord) : Coord(coord) {}
	TransformComponent(TransformComponent&&) = default;
	TransformComponent& operator = (TransformComponent&&) = default;

	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator = (const TransformComponent&) = delete;

	int Coord;
};