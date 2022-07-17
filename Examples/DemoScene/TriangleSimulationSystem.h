#pragma once

#include <ECS/System.h>

#include "PositionComponent.h"
#include "PositionHistoryComponent.h"
#include "TriangleComponent.h"

class TriangleSimulationSystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const TriangleComponent,
		PositionComponent,
		PositionHistoryComponent
	>;

	TriangleSimulationSystem(const glm::vec2& domMin, const glm::vec2& domMax);

	void Update(WorldView& world, float dt);

private:
	const glm::vec2 m_DomainMin;
	const glm::vec2 m_DomainMax;
};