#include "Common.h"

TEST_CASE("Added") {

	ecs::World world;
	world.Init();

	auto worldView = ecs::WorldView<ComponentA, ComponentB, ComponentC>(world);

	std::unordered_set<ecs::EntityHandle> setA;
	std::unordered_set<ecs::EntityHandle> setA2;
	std::unordered_set<ecs::EntityHandle> setA3;


	for (int i = 0; i < 5; ++i) {
		const ecs::EntityHandle entityHandle = world.CreateEntity();
		world.AttachComponent<ComponentA>(entityHandle);
		world.AttachComponent<ComponentB>(entityHandle);

		setA.insert(entityHandle);
		setA2.insert(entityHandle);
		setA3.insert(entityHandle);
	}
	world.ExecuteCommands();


	using Query01 = ecs::Query<>::Added<ComponentA>;
	for (auto e : Query01::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setA.erase(entityHandle);
	}
	REQUIRE(setA.size() == 0);
	REQUIRE(setA2.size() == 5);



	for (int i = 0; i < 5; ++i) {
		const ecs::EntityHandle entityHandle = world.CreateEntity();
		world.AttachComponent<ComponentA>(entityHandle);
		world.AttachComponent<ComponentC>(entityHandle);

		setA2.insert(entityHandle);
		setA3.insert(entityHandle);
	}
	world.ExecuteCommands();



	using Query02 = ecs::Query<>::Added<ComponentA>;
	for (auto e : Query02::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setA2.erase(entityHandle);
	}
	REQUIRE(setA2.size() == 5);



	using Query03 = ecs::Query<>::Include<ComponentA>;
	for (auto e : Query03::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setA3.erase(entityHandle);
	}
	REQUIRE(setA3.size() == 0);
}