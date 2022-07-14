#include "Common.h"

TEST_CASE("Query") {

	ecs::World world;
	world.Init();

	auto worldViewA = ecs::WorldView<ComponentA>(world);
	auto worldViewB = ecs::WorldView<ComponentB>(world);
	auto worldViewC = ecs::WorldView<ComponentC>(world);
	auto worldViewD = ecs::WorldView<ComponentD>(world);
	auto worldViewAB = ecs::WorldView<ComponentA, ComponentB>(world);
	auto worldViewAC = ecs::WorldView<ComponentA, ComponentC>(world);
	auto worldViewBC = ecs::WorldView<ComponentB, ComponentC>(world);
	auto worldViewABC = ecs::WorldView<ComponentA, ComponentB, ComponentC>(world);


	std::unordered_set<ecs::EntityHandle> setA;
	std::unordered_set<ecs::EntityHandle> setB;
	std::unordered_set<ecs::EntityHandle> setC;
	std::unordered_set<ecs::EntityHandle> setAB;
	std::unordered_set<ecs::EntityHandle> setAC;
	std::unordered_set<ecs::EntityHandle> setBC;
	std::unordered_set<ecs::EntityHandle> setABC;

	std::unordered_set<ecs::EntityHandle> setA2;
	std::unordered_set<ecs::EntityHandle> setB2;
	std::unordered_set<ecs::EntityHandle> setC2;
	std::unordered_set<ecs::EntityHandle> setAB2;
	std::unordered_set<ecs::EntityHandle> setAC2;
	std::unordered_set<ecs::EntityHandle> setBC2;
	std::unordered_set<ecs::EntityHandle> setABC2;




	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentB>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentC>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentD>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentB>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentC>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentB, ComponentC>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentB, ComponentC>::Get()) == 0);



	auto e8 = world.CreateEntity();
	world.AttachComponent<ComponentA>(e8);
	world.AttachComponent<ComponentC>(e8);

	auto e9 = world.CreateEntity();
	world.AttachComponent<ComponentB>(e9);
	world.AttachComponent<ComponentC>(e9);

	auto e10 = world.CreateEntity();
	world.AttachComponent<ComponentD>(e10);

	auto e1 = world.CreateEntity();
	world.AttachComponent<ComponentA>(e1);
	setA.insert(e1);

	auto e2 = world.CreateEntity();
	world.AttachComponent<ComponentB>(e2);
	setB.insert(e2);

	auto e3 = world.CreateEntity();
	world.AttachComponent<ComponentC>(e3);
	setC.insert(e3);

	auto e4 = world.CreateEntity();
	world.AttachComponent<ComponentA>(e4);
	world.AttachComponent<ComponentB>(e4);
	setAB.insert(e4);

	world.FinishAllCommands();
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentB>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentC>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentD>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentB>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentC>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentB, ComponentC>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentB, ComponentC>::Get()) == 0);

	world.DestroyEntity(e9);
	world.DestroyEntity(e10);
	world.DestroyEntity(e8);

	auto e5 = world.CreateEntity();
	world.AttachComponent<ComponentA>(e5);
	world.AttachComponent<ComponentC>(e5);
	setAC.insert(e5);

	auto e6 = world.CreateEntity();
	world.AttachComponent<ComponentB>(e6);
	world.AttachComponent<ComponentC>(e6);
	setBC.insert(e6);

	auto e7 = world.CreateEntity();
	world.AttachComponent<ComponentA>(e7);
	world.AttachComponent<ComponentB>(e7);
	world.AttachComponent<ComponentC>(e7);
	setABC.insert(e7);

	world.FinishAllCommands();
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentB>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentC>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentD>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentB>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentC>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentB, ComponentC>::Get()) == 1);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentB, ComponentC>::Get()) == 1);

	{
		using Query = ecs::Query<>
			::Include<ComponentA>;
		for (auto e : Query::Iterate(worldViewA)) {
			setA.erase(e);
			setA2.insert(e);
		}
		REQUIRE(setA.empty());
		REQUIRE(setA2.size() == 4);
	}

	{
		using Query = ecs::Query<>
			::Include<ComponentB>;
		for (auto e : Query::Iterate(worldViewB)) {
			setB.erase(e);
			setB2.insert(e);
		}
		REQUIRE(setB.empty());
		REQUIRE(setB2.size() == 4);
	}

	{
		using Query = ecs::Query<>
			::Include<ComponentC>;
		for (auto e : Query::Iterate(worldViewC)) {
			setC.erase(e);
			setC2.insert(e);
		}
		REQUIRE(setC.empty());
		REQUIRE(setC2.size() == 4);
	}

	{
		using Query = ecs::Query<>
			::Include<ComponentA>
			::Include<ComponentB>;
		for (auto e : Query::Iterate(worldViewAB)) {
			setAB.erase(e);
			setAB2.insert(e);
		}
		REQUIRE(setAB.empty());
		REQUIRE(setAB2.size() == 2);
	}

	{
		using Query = ecs::Query<>
			::Include<ComponentA>
			::Include<ComponentC>;
		for (auto e : Query::Iterate(worldViewAC)) {
			setAC.erase(e);
			setAC2.insert(e);
		}
		REQUIRE(setAC.empty());
		REQUIRE(setAC2.size() == 2);
	}

	{
		using Query = ecs::Query<>
			::Include<ComponentB>
			::Include<ComponentC>;
		for (auto e : Query::Iterate(worldViewBC)) {
			setBC.erase(e);
			setBC2.insert(e);
		}
		REQUIRE(setBC.empty());
		REQUIRE(setBC2.size() == 2);
	}

	{
		using Query = ecs::Query<>
			::Include<ComponentA>
			::Include<ComponentB>
			::Include<ComponentC>;
		for (auto e : Query::Iterate(worldViewABC)) {
			setABC.erase(e);
			setABC2.insert(e);
		}
		REQUIRE(setABC.empty());
		REQUIRE(setABC2.size() == 1);
	}

	{
		using Query = ecs::Query<>
			::Include<ComponentD>;
		for (auto e : Query::Iterate(worldViewD)) {
			REQUIRE(false);
		}
	}


	world.DestroyEntity(e1);
	world.DestroyEntity(e2);
	world.DestroyEntity(e3);
	world.DestroyEntity(e4);
	world.DestroyEntity(e5);
	world.DestroyEntity(e6);
	world.DestroyEntity(e7);
	world.FinishAllCommands();

	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentB>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentC>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentD>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentB>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentC>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentB, ComponentC>::Get()) == 0);
	REQUIRE(world.GetGroupEntityCount(ecs::ut::GroupBitmask<ComponentA, ComponentB, ComponentC>::Get()) == 0);
}