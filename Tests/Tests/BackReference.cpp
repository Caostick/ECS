#include "Common.h"

TEST_CASE("BackReference") {

	ecs::World world;
	world.Init();

	ecs::EntityHandle handle1 = world.CreateEntity();
	ecs::EntityHandle handle2 = world.CreateEntity();
	ecs::EntityHandle handle3 = world.CreateEntity();

	world.AttachComponent<ComponentA>(handle1);
	world.AttachComponent<ComponentB>(handle1);
	world.FinishAllCommands();

	world.AttachComponent<ComponentC>(handle1);
	world.AttachComponent<ComponentD>(handle1);
	world.FinishAllCommands();



	world.AttachComponent<ComponentD>(handle2);
	world.FinishAllCommands();

	world.AttachComponent<ComponentC>(handle2);
	world.FinishAllCommands();

	world.AttachComponent<ComponentB>(handle2);
	world.FinishAllCommands();

	world.AttachComponent<ComponentA>(handle2);
	world.FinishAllCommands();



	world.AttachComponent<ComponentA>(handle3);
	world.FinishAllCommands();

	world.AttachComponent<ComponentB>(handle3);
	world.FinishAllCommands();

	world.AttachComponent<ComponentC>(handle3);
	world.FinishAllCommands();

	world.AttachComponent<ComponentD>(handle3);
	world.FinishAllCommands();

	REQUIRE(handle1 == world.GetEntityBackreference(handle1));
	REQUIRE(handle2 == world.GetEntityBackreference(handle2));
	REQUIRE(handle3 == world.GetEntityBackreference(handle3));

	world.DestroyEntity(handle1);
	world.DestroyEntity(handle2);
	world.DestroyEntity(handle3);
	world.FinishAllCommands();
}