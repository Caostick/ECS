#include "RectangleSimulationSystem.h"
#include "Utils.h"

#include <ECS/Query.h>

RectangleSimulationSystem::RectangleSimulationSystem(const glm::vec2& domMin, const glm::vec2& domMax)
	: m_DomainMin(domMin)
	, m_DomainMax(domMax) {}

void RectangleSimulationSystem::Update(WorldView& world, [[maybe_unused]] float dt) {

	using QueryAdded = ecs::Query<>
		::Added<RectangleComponent>
		::Include<PositionComponent>;

	for (auto e : QueryAdded::Iterate(world)) {
		auto& rectangleComponent = e.Get<RectangleComponent>();

		rectangleComponent.Size = RndRange(0.0f, 1.0f);
	}



	using Query = ecs::Query<>
		::Include<RectangleComponent>
		::Include<PositionComponent>;

	for (auto e : Query::Iterate(world)) {
		auto& rectangleComponent = e.Get<RectangleComponent>();
		auto& positionComponent = e.Get<PositionComponent>();

		positionComponent.Position += glm::vec2(0.0f, -dt * rectangleComponent.Size * 5.0f);

		if (positionComponent.Position[1] < m_DomainMin[1]) {
			world.DestroyEntity(e);
		}
	}
}