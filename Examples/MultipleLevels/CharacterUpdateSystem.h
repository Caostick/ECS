#pragma once

#include <ECS/System.h>

#include "CharacterComponent.h"

class CharacterUpdateSystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		CharacterComponent
	>;

	void Update(WorldView& world, float dt);
};