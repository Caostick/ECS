#pragma once

struct TriangleComponent {
	TriangleComponent(float speed) : Speed(speed) {}
	TriangleComponent(TriangleComponent&&) = default;
	TriangleComponent& operator = (TriangleComponent&&) = default;

	TriangleComponent(const TriangleComponent&) = delete;
	TriangleComponent& operator = (const TriangleComponent&) = delete;

	float Speed;
};