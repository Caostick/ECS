#pragma once

struct TransformHistoryComponent {
	TransformHistoryComponent() = default;
	TransformHistoryComponent(TransformHistoryComponent&&) = default;

	TransformHistoryComponent(const TransformHistoryComponent&) = delete;
	TransformHistoryComponent& operator = (const TransformHistoryComponent&) = delete;
	TransformHistoryComponent& operator = (TransformHistoryComponent&&) = delete;

	int Coord;
};