#pragma once

struct PointComponent {
	PointComponent(float speed) : Speed(speed) {}
	PointComponent(PointComponent&&) = default;
	PointComponent& operator = (PointComponent&&) = default;

	PointComponent(const PointComponent&) = delete;
	PointComponent& operator = (const PointComponent&) = delete;

	float Speed;
};