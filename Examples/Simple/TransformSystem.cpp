#include "TransformSystem.h"

#include <ECS/Query.h>

TransformSystem::TransformSystem(int step)
	: m_Step(step) {
}

void TransformSystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<PositionComponent>;

	for (auto e : Query::Iterate(world)) {
		auto& transformComponent = e.Get<PositionComponent>();

		transformComponent.Coord += m_Step;
	}
}