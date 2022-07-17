#include "PointSimulationSystem.h"

#include <ECS/Query.h>

PointSimulationSystem::PointSimulationSystem(const glm::vec2& domMin, const glm::vec2& domMax) 
	: m_DomainMin(domMin)
	, m_DomainMax(domMax) {
}

void PointSimulationSystem::Update(WorldView& world, float dt) {
	using Query = ecs::Query<>
		::Include<PointComponent>
		::Include<PositionComponent>;

	for (auto e : Query::Iterate(world)) {
		auto& pointComponent = e.Get<PointComponent>();
		auto& positionComponent = e.Get<PositionComponent>();

		positionComponent.Position += glm::vec2(0.0f, -dt * pointComponent.Speed);

		if (positionComponent.Position[1] < m_DomainMin[1]) {
			positionComponent.Position[1] = m_DomainMax[1];
		}
	}
}