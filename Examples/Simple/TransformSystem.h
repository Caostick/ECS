#pragma once

#include <ECS/System.h>

#include "TransformComponent.h"

class TransformSystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		PositionComponent
	>;

	TransformSystem(int step);

	void Update(WorldView& world, float dt);

private:
	int m_Step;
};