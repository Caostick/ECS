#include "Common.h"

TEST_CASE("Excluded") {

	ecs::World world;
	world.Init();

	auto worldView = ecs::WorldView<ComponentA, ComponentB, ComponentC>(world);

	std::unordered_set<ecs::EntityHandle> setAB;
	std::unordered_set<ecs::EntityHandle> setAC;
	std::unordered_set<ecs::EntityHandle> setABC;

	std::unordered_set<ecs::EntityHandle> setAnoB;
	std::unordered_set<ecs::EntityHandle> setAnoC;
	std::unordered_set<ecs::EntityHandle> setBnoC;
	std::unordered_set<ecs::EntityHandle> setCnoB;

	for (int i = 0; i < 2; ++i) {
		const ecs::EntityHandle entityHandle = world.CreateEntity();
		world.AttachComponent<ComponentA>(entityHandle);
		world.AttachComponent<ComponentB>(entityHandle);

		setAnoC.insert(entityHandle);
		setBnoC.insert(entityHandle);
		setAB.insert(entityHandle);
	}

	for (int i = 0; i < 3; ++i) {
		const ecs::EntityHandle entityHandle = world.CreateEntity();
		world.AttachComponent<ComponentA>(entityHandle);
		world.AttachComponent<ComponentC>(entityHandle);

		setAnoB.insert(entityHandle);
		setAC.insert(entityHandle);
	}

	for (int i = 0; i < 4; ++i) {
		const ecs::EntityHandle entityHandle = world.CreateEntity();
		world.AttachComponent<ComponentA>(entityHandle);
		world.AttachComponent<ComponentB>(entityHandle);
		world.AttachComponent<ComponentC>(entityHandle);

		setABC.insert(entityHandle);
	}

	world.ExecuteCommands();

	using queryA = ecs::Query<>
		::Include<ComponentA>;

	using queryAB = ecs::Query<>
		::Include<ComponentA>
		::Include<ComponentB>;

	using queryAC = ecs::Query<>
		::Include<ComponentA>
		::Include<ComponentC>;

	using queryABC = ecs::Query<>
		::Include<ComponentA>
		::Include<ComponentB>
		::Include<ComponentC>;

	using queryAnoB = ecs::Query<>
		::Include<ComponentA>
		::Exclude<ComponentB>;

	using queryAnoC = ecs::Query<>
		::Include<ComponentA>
		::Exclude<ComponentC>;

	using queryBnoC = ecs::Query<>
		::Include<ComponentB>
		::Exclude<ComponentC>;

	using queryCnoB = ecs::Query<>
		::Include<ComponentC>
		::Exclude<ComponentB>;

	int countA = 0;
	for (auto e : queryA::Iterate(worldView)) {
		countA++;
	}
	REQUIRE(countA == 9);

	for (auto e : queryAB::Iterate(worldView)) {
		setAB.erase(e);
	}
	REQUIRE(setAB.size() == 0);

	for (auto e : queryAC::Iterate(worldView)) {
		setAC.erase(e);
	}
	REQUIRE(setAC.size() == 0);

	for (auto e : queryABC::Iterate(worldView)) {
		setABC.erase(e);
	}
	REQUIRE(setABC.size() == 0);

	for (auto e : queryAnoB::Iterate(worldView)) {
		setAnoB.erase(e);
	}
	REQUIRE(setAnoB.size() == 0);

	for (auto e : queryAnoC::Iterate(worldView)) {
		setAnoC.erase(e);
	}
	REQUIRE(setAnoC.size() == 0);

	for (auto e : queryBnoC::Iterate(worldView)) {
		setBnoC.erase(e);
	}
	REQUIRE(setBnoC.size() == 0);

	for (auto e : queryCnoB::Iterate(worldView)) {
		setCnoB.erase(e);
	}
	REQUIRE(setCnoB.size() == 0);
}