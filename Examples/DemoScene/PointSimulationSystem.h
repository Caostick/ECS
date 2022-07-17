#pragma once

#include <ECS/System.h>
#include <glm/vec2.hpp>

#include "PositionComponent.h"
#include "PointComponent.h"

class PointSimulationSystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const PointComponent,
		PositionComponent
	>;

	PointSimulationSystem(const glm::vec2& domMin, const glm::vec2& domMax);

	void Update(WorldView& world, float dt);

private:
	const glm::vec2 m_DomainMin;
	const glm::vec2 m_DomainMax;
};