#include "TransformSystem.h"

#include <ECS/Query.h>

TransformSystem::TransformSystem(int step)
	: m_Step(step) {
}

void TransformSystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<TransformComponent>;

	for (auto e : Query::Iterate(world)) {
		auto& transformComponent = e.Get<TransformComponent>();

		transformComponent.Coord += m_Step;
	}
}