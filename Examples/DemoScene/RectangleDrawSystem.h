#pragma once

#include <ECS/System.h>

#include "PositionComponent.h"
#include "RectangleComponent.h"

class RectangleDrawSystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const RectangleComponent,
		const PositionComponent
	>;

	void Update(WorldView& world, float dt);
};