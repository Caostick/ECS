#include "TransformHistorySystem.h"

#include <ECS/Query.h>

void PositionHistorySystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<TransformComponent>
		::Include<TransformHistoryComponent>;

	for (auto e : Query::Iterate(world)) {
		auto& transformComponent = e.Get<TransformComponent>();
		auto& transformHistoryComponent = e.Get<TransformHistoryComponent>();

		transformHistoryComponent.Coord = transformComponent.Coord;
	}
}