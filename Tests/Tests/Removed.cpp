#include "Common.h"

TEST_CASE("AddedRemoved") {

	ecs::World world;
	world.Init();

	auto worldView = ecs::WorldView<ComponentA, ComponentB, ComponentC, ComponentD, ComponentE, ComponentF>(world);


	{
		bool check1 = false;
		bool check2 = false;
		bool check3 = false;
		bool check4 = false;
		bool check5 = false;
		bool check6 = false;

		using QueryUpdateF = ecs::Query<>
			::Include<ComponentF>;

		using QueryAddedF = ecs::Query<>
			::Added<ComponentF>;

		using QueryRemovedF = ecs::Query<>
			::Removed<ComponentF>;

		const ecs::EntityHandle handle = worldView.CreateEntity();
		worldView.AttachComponent<ComponentF>(handle);

		world.ExecuteCommands();

		for (auto e : QueryAddedF::Iterate(worldView)) {
			check1 = true;
		}

		for (auto e : QueryUpdateF::Iterate(worldView)) {
			check2 = true;
		}

		world.DestroyEntity(handle);
		world.ExecuteCommands();

		for (auto e : QueryRemovedF::Iterate(worldView)) {
			check3 = true;
		}

		for (auto e : QueryUpdateF::Iterate(worldView)) {
			check4 = true;
		}

		world.ExecuteCommands();

		for (auto e : QueryRemovedF::Iterate(worldView)) {
			check5 = true;
		}

		for (auto e : QueryUpdateF::Iterate(worldView)) {
			check6 = true;
		}

		REQUIRE(check1 == true);
		REQUIRE(check2 == true);
		REQUIRE(check3 == true);
		REQUIRE(check4 == false);
		REQUIRE(check5 == false);
		REQUIRE(check6 == false);
	}






	ecs::EntityHandle entityHandles[15];

	std::unordered_set<ecs::EntityHandle> setABC;
	std::unordered_set<ecs::EntityHandle> setABC2;
	std::unordered_set<ecs::EntityHandle> setABC3;
	std::unordered_set<ecs::EntityHandle> setABC4;
	std::unordered_set<ecs::EntityHandle> setA;
	std::unordered_set<ecs::EntityHandle> setB;
	std::unordered_set<ecs::EntityHandle> setC;
	std::unordered_set<ecs::EntityHandle> setD;
	std::unordered_set<ecs::EntityHandle> setE;
	std::unordered_set<ecs::EntityHandle> setDE;



	for (int i = 0; i < 15; ++i) {
		const ecs::EntityHandle entityHandle = world.CreateEntity();
		world.AttachComponent<ComponentA>(entityHandle);
		world.AttachComponent<ComponentB>(entityHandle);
		world.AttachComponent<ComponentC>(entityHandle);
		world.AttachComponent<ComponentD>(entityHandle);
		world.AttachComponent<ComponentE>(entityHandle);

		setA.insert(entityHandle);
		setB.insert(entityHandle);
		setC.insert(entityHandle);
		setD.insert(entityHandle);
		setE.insert(entityHandle);
		setDE.insert(entityHandle);
		setABC.insert(entityHandle);
		setABC2.insert(entityHandle);
		setABC3.insert(entityHandle);
		setABC4.insert(entityHandle);

		entityHandles[i] = entityHandle;
	}
	world.ExecuteCommands();

	using QueryABC = ecs::Query<>
		::Added<ComponentA>
		::Added<ComponentB>
		::Added<ComponentC>;

	for (auto e : QueryABC::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;	
		setABC.erase(entityHandle);
	}
	REQUIRE(setABC.size() == 0);




	for (int i = 0; i < 15; ++i) {
		world.DetachComponent<ComponentA>(entityHandles[i]);

		if (i >= 10) {
			continue;
		}
		world.DetachComponent<ComponentB>(entityHandles[i]);

		if (i >= 5) {
			continue;
		}
		world.DetachComponent<ComponentC>(entityHandles[i]);
	}
	world.ExecuteCommands();



	using QueryA = ecs::Query<>
		::Removed<ComponentA>;
	for (auto e : QueryA::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setA.erase(entityHandle);
	}
	REQUIRE(setA.size() == 0);



	using QueryB = ecs::Query<>
		::Removed<ComponentB>;
	for (auto e : QueryB::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setB.erase(entityHandle);
	}
	REQUIRE(setB.size() == 5);



	using QueryC = ecs::Query<>
		::Removed<ComponentC>;
	for (auto e : QueryC::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setC.erase(entityHandle);
	}
	REQUIRE(setC.size() == 10);



	using QueryABC2 = ecs::Query<>
		::Include<ComponentA>
		::Include<ComponentB>
		::Include<ComponentC>;
	for (auto e : QueryABC2::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setABC2.erase(entityHandle);
	}
	REQUIRE(setABC2.size() == 15);


	using QueryABC3 = ecs::Query<>
		::Removed<ComponentA>
		::Removed<ComponentB>
		::Removed<ComponentC>;
	for (auto e : QueryABC3::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setABC3.erase(entityHandle);
	}
	REQUIRE(setABC3.size() == 10);



	using QueryABC4 = ecs::Query<>
		::Removed<ComponentA>
		::Removed<ComponentB>
		::Include<ComponentC>;
	for (auto e : QueryABC4::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setABC4.erase(entityHandle);
	}
	REQUIRE(setABC4.size() == 10);

	world.ExecuteCommands();



	for (int i = 0; i < 15; ++i) {
		world.DestroyEntity(entityHandles[i]);
	}

	world.ExecuteCommands();

	using QueryD = ecs::Query<>
		::Removed<ComponentD>;
	for (auto e : QueryD::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setD.erase(entityHandle);
	}
	REQUIRE(setD.size() == 0);



	using QueryE = ecs::Query<>
		::Removed<ComponentE>;
	for (auto e : QueryE::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setE.erase(entityHandle);
	}
	REQUIRE(setE.size() == 0);



	using QueryDE = ecs::Query<>
		::Removed<ComponentD>
		::Removed<ComponentE>;
	for (auto e : QueryDE::Iterate(worldView)) {
		const ecs::EntityHandle entityHandle = e;
		setDE.erase(entityHandle);
	}
	REQUIRE(setDE.size() == 0);
}