#include "TriangleSimulationSystem.h"
#include "Utils.h"

#include <ECS/Query.h>

TriangleSimulationSystem::TriangleSimulationSystem(const glm::vec2& domMin, const glm::vec2& domMax)
	: m_DomainMin(domMin)
	, m_DomainMax(domMax) {}

void TriangleSimulationSystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<TriangleComponent>
		::Include<PositionComponent>
		::Include<PositionHistoryComponent>;

	for (auto e : Query::Iterate(world)) {
		auto& triangleComponent = e.Get<TriangleComponent>();
		auto& positionComponent = e.Get<PositionComponent>();

		positionComponent.Position += glm::vec2(0.0f, dt * triangleComponent.Speed);

		if (positionComponent.Position[1] > m_DomainMax[1]) {
			positionComponent.Position[1] = m_DomainMin[1];
		}
	}
}