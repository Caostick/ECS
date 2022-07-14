#include "Common.h"

TEST_CASE("ComponentData") {

	ecs::World world;
	world.Init();

	ecs::EntityHandle handle1 = world.CreateEntity();
	ecs::EntityHandle handle2 = world.CreateEntity();

	world.AttachComponent<ComponentA>(handle1) = { 10 };
	world.AttachComponent<ComponentD>(handle2) = { 26, 27, 28, 29 };
	world.FinishAllCommands();

	REQUIRE(handle1 == world.GetEntityBackreference(handle1));
	REQUIRE(handle2 == world.GetEntityBackreference(handle2));
	REQUIRE(world.GetComponent<ComponentA>(handle1) == ComponentA({ 10 }));
	REQUIRE(world.GetComponent<ComponentD>(handle2) == ComponentD({ 26, 27, 28, 29 }));

	world.AttachComponent<ComponentB>(handle1) = { 11, 12 };
	world.AttachComponent<ComponentC>(handle2) = { 23, 24, 25 };
	world.FinishAllCommands();

	REQUIRE(handle1 == world.GetEntityBackreference(handle1));
	REQUIRE(handle2 == world.GetEntityBackreference(handle2));
	REQUIRE(world.GetComponent<ComponentA>(handle1) == ComponentA({ 10 }));
	REQUIRE(world.GetComponent<ComponentD>(handle2) == ComponentD({ 26, 27, 28, 29 }));
	REQUIRE(world.GetComponent<ComponentB>(handle1) == ComponentB({ 11, 12 }));
	REQUIRE(world.GetComponent<ComponentC>(handle2) == ComponentC({ 23, 24, 25 }));


	world.AttachComponent<ComponentC>(handle1) = { 13, 14, 15 };
	world.AttachComponent<ComponentB>(handle2) = { 21, 22 };
	world.FinishAllCommands();

	REQUIRE(handle1 == world.GetEntityBackreference(handle1));
	REQUIRE(handle2 == world.GetEntityBackreference(handle2));
	REQUIRE(world.GetComponent<ComponentA>(handle1) == ComponentA({ 10 }));
	REQUIRE(world.GetComponent<ComponentD>(handle2) == ComponentD({ 26, 27, 28, 29 }));
	REQUIRE(world.GetComponent<ComponentB>(handle1) == ComponentB({ 11, 12 }));
	REQUIRE(world.GetComponent<ComponentC>(handle2) == ComponentC({ 23, 24, 25 }));
	REQUIRE(world.GetComponent<ComponentC>(handle1) == ComponentC({ 13, 14, 15 }));
	REQUIRE(world.GetComponent<ComponentB>(handle2) == ComponentB({ 21, 22 }));

	world.AttachComponent<ComponentD>(handle1) = { 16, 17, 18, 19 };
	world.AttachComponent<ComponentA>(handle2) = { 20 };
	world.FinishAllCommands();

	REQUIRE(handle1 == world.GetEntityBackreference(handle1));
	REQUIRE(handle2 == world.GetEntityBackreference(handle2));
	REQUIRE(world.GetComponent<ComponentA>(handle1) == ComponentA({ 10 }));
	REQUIRE(world.GetComponent<ComponentD>(handle2) == ComponentD({ 26, 27, 28, 29 }));
	REQUIRE(world.GetComponent<ComponentB>(handle1) == ComponentB({ 11, 12 }));
	REQUIRE(world.GetComponent<ComponentC>(handle2) == ComponentC({ 23, 24, 25 }));
	REQUIRE(world.GetComponent<ComponentC>(handle1) == ComponentC({ 13, 14, 15 }));
	REQUIRE(world.GetComponent<ComponentB>(handle2) == ComponentB({ 21, 22 }));
	REQUIRE(world.GetComponent<ComponentD>(handle1) == ComponentD({ 16, 17, 18, 19 }));
	REQUIRE(world.GetComponent<ComponentA>(handle2) == ComponentA({ 20 }));

	world.DetachComponent<ComponentA>(handle1);
	world.DetachComponent<ComponentD>(handle2);
	world.FinishAllCommands();

	REQUIRE(handle1 == world.GetEntityBackreference(handle1));
	REQUIRE(handle2 == world.GetEntityBackreference(handle2));
	REQUIRE(world.GetComponent<ComponentB>(handle1) == ComponentB({ 11, 12 }));
	REQUIRE(world.GetComponent<ComponentC>(handle2) == ComponentC({ 23, 24, 25 }));
	REQUIRE(world.GetComponent<ComponentC>(handle1) == ComponentC({ 13, 14, 15 }));
	REQUIRE(world.GetComponent<ComponentB>(handle2) == ComponentB({ 21, 22 }));
	REQUIRE(world.GetComponent<ComponentD>(handle1) == ComponentD({ 16, 17, 18, 19 }));
	REQUIRE(world.GetComponent<ComponentA>(handle2) == ComponentA({ 20 }));

	world.DetachComponent<ComponentB>(handle1);
	world.DetachComponent<ComponentC>(handle2);
	world.FinishAllCommands();

	REQUIRE(handle1 == world.GetEntityBackreference(handle1));
	REQUIRE(handle2 == world.GetEntityBackreference(handle2));
	REQUIRE(world.GetComponent<ComponentC>(handle1) == ComponentC({ 13, 14, 15 }));
	REQUIRE(world.GetComponent<ComponentB>(handle2) == ComponentB({ 21, 22 }));
	REQUIRE(world.GetComponent<ComponentD>(handle1) == ComponentD({ 16, 17, 18, 19 }));
	REQUIRE(world.GetComponent<ComponentA>(handle2) == ComponentA({ 20 }));

	world.DetachComponent<ComponentC>(handle1);
	world.DetachComponent<ComponentB>(handle2);
	world.FinishAllCommands();

	REQUIRE(handle1 == world.GetEntityBackreference(handle1));
	REQUIRE(handle2 == world.GetEntityBackreference(handle2));
	REQUIRE(world.GetComponent<ComponentD>(handle1) == ComponentD({ 16, 17, 18, 19 }));
	REQUIRE(world.GetComponent<ComponentA>(handle2) == ComponentA({ 20 }));

	world.DetachComponent<ComponentD>(handle1);
	world.DetachComponent<ComponentA>(handle2);
	world.FinishAllCommands();

	REQUIRE(handle1 == world.GetEntityBackreference(handle1));
	REQUIRE(handle2 == world.GetEntityBackreference(handle2));


	world.DestroyEntity(handle1);
	world.DestroyEntity(handle2);
	world.FinishAllCommands();


	SECTION("Move components") {

		{
			handle1 = world.CreateEntity();
			auto& cE1 = world.AttachComponent<ComponentE>(handle1);
			REQUIRE(&cE1 == cE1.m_SelfPtr);
		}

		{
			world.FinishAllCommands();
			ComponentE& cE1 = world.GetComponent<ComponentE>(handle1);
			REQUIRE(&cE1 == cE1.m_SelfPtr);
		}

		{
			handle2 = world.CreateEntity();
			auto& cE2 = world.AttachComponent<ComponentE>(handle2);
			auto& cE1 = world.GetComponent<ComponentE>(handle1);
			REQUIRE(&cE1 == cE1.m_SelfPtr);
			REQUIRE(&cE2 == cE2.m_SelfPtr);
		}

		{
			world.FinishAllCommands();
			auto& cE1 = world.GetComponent<ComponentE>(handle1);
			auto& cE2 = world.GetComponent<ComponentE>(handle2);
			REQUIRE(&cE1 == cE1.m_SelfPtr);
			REQUIRE(&cE2 == cE2.m_SelfPtr);
		}

		{
			world.DetachComponent<ComponentE>(handle1);
			auto& cE2 = world.GetComponent<ComponentE>(handle2);
			REQUIRE(&cE2 == cE2.m_SelfPtr);
		}

		{
			world.FinishAllCommands();
			auto& cE2 = world.GetComponent<ComponentE>(handle2);
			REQUIRE(&cE2 == cE2.m_SelfPtr);
		}
	}
}
