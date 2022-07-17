#pragma once

#include <ECS/System.h>

#include "PositionComponent.h"
#include "PositionHistoryComponent.h"

class PositionHistorySystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const PositionComponent,
		PositionHistoryComponent
	>;

	void Update(WorldView& world, float dt);
};