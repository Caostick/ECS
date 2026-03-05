#pragma once

struct TransformComponent {
	TransformComponent(int coord) : Coord(coord) {}
	TransformComponent(TransformComponent&&) = default;

	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator = (const TransformComponent&) = delete;
	TransformComponent& operator = (TransformComponent&&) = delete;

	int Coord;
};