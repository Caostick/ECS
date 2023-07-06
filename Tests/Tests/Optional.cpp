#include "Common.h"

TEST_CASE("Optional") {

	ecs::World world;
	world.Init();

	auto worldView = ecs::WorldView<ComponentA, ComponentB>(world);

	std::unordered_set<ecs::EntityHandle> setA;
	std::unordered_set<ecs::EntityHandle> setAB;
	std::unordered_set<ecs::EntityHandle> setAnoB;
	std::unordered_set<ecs::EntityHandle> setAwithB;
	std::unordered_set<ecs::EntityHandle> setAwithoutB;

	for (int i = 0; i < 5; ++i) {
		const ecs::EntityHandle entityHandle = world.CreateEntity();
		world.AttachComponent<ComponentA>(entityHandle);
		world.AttachComponent<ComponentB>(entityHandle);

		setA.insert(entityHandle);
		setAB.insert(entityHandle);
	}

	for(int i = 0; i < 3; ++i) {
		const ecs::EntityHandle entityHandle = world.CreateEntity();
		world.AttachComponent<ComponentA>(entityHandle);

		setA.insert(entityHandle);
		setAnoB.insert(entityHandle);
	}

	world.ExecuteCommands();

	bool hasC = false;

	using Query = ecs::Query<>::Include<ComponentA>;
	for (auto e : Query::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;

		hasC |= worldView.HasComponent<ComponentC>(e);

		if(worldView.HasComponent<ComponentB>(e)) {
			[[maybe_unused]] auto&& b = worldView.GetComponent<ComponentB>(e);
			setAwithB.insert(entityHandle);
		} else {
			setAwithoutB.insert(entityHandle);
		}
	}

	REQUIRE(hasC == false);

	auto setABEx = setAwithB;
	for(auto e : setAwithoutB) {
		setABEx.insert(e);
	}

	REQUIRE(setAB == setAwithB);
	REQUIRE(setAnoB == setAwithoutB);
	REQUIRE(setABEx == setA);
}