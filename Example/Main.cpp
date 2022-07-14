#include <ECS/World.h>

#include "TransformSystem.h"
#include "TransformHistorySystem.h"

#include <iostream>

int main() {
	constexpr float deltaTime = 1.0f / 60.0f;

	// Initialization
	ecs::World world;

	world.Init();
	world.RegisterSystem<TransformSystem>(2);
	world.RegisterSystem<TransformHistorySystem>();


	auto e = world.CreateEntity();
	world.AttachComponent<TransformComponent>(e, 10);
	world.AttachComponent<TransformHistoryComponent>(e);

	// Update
	for (int i = 0; i < 100; ++i) {
		world.Update(deltaTime);
		world.ExecuteCommands();
	}
	world.FinishAllCommands();

	std::cout << "Coord: " << world.GetComponent<TransformHistoryComponent>(e).Coord << '\n';

	// Deinitialization
	world.Release();

	return 0;
}