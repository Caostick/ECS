#include "PositionHistorySystem.h"

#include <ECS/Query.h>

void PositionHistorySystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<PositionComponent>
		::Include<PositionHistoryComponent>;

	for (auto e : Query::Iterate(world)) {
		auto& positionComponent = e.Get<PositionComponent>();
		auto& positionHistoryComponent = e.Get<PositionHistoryComponent>();

		positionHistoryComponent.Position = positionComponent.Position;
	}
}