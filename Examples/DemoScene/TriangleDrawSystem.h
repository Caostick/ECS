#pragma once

#include <ECS/System.h>

#include "PositionComponent.h"
#include "PositionHistoryComponent.h"
#include "TriangleComponent.h"

class TriangleDrawSystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const TriangleComponent,
		const PositionComponent,
		const PositionHistoryComponent
	>;

	void Update(WorldView& world, float dt);
};