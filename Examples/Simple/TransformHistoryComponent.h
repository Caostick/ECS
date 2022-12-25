#pragma once

struct TransformHistoryComponent {
	TransformHistoryComponent() = default;
	TransformHistoryComponent(TransformHistoryComponent&&) = default;
	TransformHistoryComponent& operator = (TransformHistoryComponent&&) = default;

	TransformHistoryComponent(const TransformHistoryComponent&) = delete;
	TransformHistoryComponent& operator = (const TransformHistoryComponent&) = delete;

	int Coord;
};