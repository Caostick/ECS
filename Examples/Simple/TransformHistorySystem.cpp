#include "TransformHistorySystem.h"

#include <ECS/Query.h>

void PositionHistorySystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<PositionComponent>
		::Include<PositionHistoryComponent>;

	for (auto e : Query::Iterate(world)) {
		auto& transformComponent = e.Get<PositionComponent>();
		auto& transformHistoryComponent = e.Get<PositionHistoryComponent>();

		transformHistoryComponent.Coord = transformComponent.Coord;
	}
}