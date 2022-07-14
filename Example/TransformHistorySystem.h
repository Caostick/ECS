#pragma once

#include <ECS/System.h>

#include "TransformComponent.h"
#include "TransformHistoryComponent.h"

class TransformHistorySystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const TransformComponent,
		TransformHistoryComponent
	>;

	void Update(WorldView& world, float dt);
};