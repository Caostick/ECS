#pragma once

#include <ECS/System.h>

#include "PositionComponent.h"
#include "PointComponent.h"

class PointDrawSystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const PointComponent,
		const PositionComponent
	>;

	void Update(WorldView& world, float dt);
};