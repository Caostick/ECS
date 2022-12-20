#include "Common.h"

#include <ECS/EntityViewConstructor.h>

using EntityAB = ecs::EntityView<
	ComponentA,
	ComponentB
>;

using EntityAC = ecs::EntityView<
	ComponentA,
	ComponentC
>;

using EntityBC = ecs::EntityView<
	ComponentB,
	ComponentC
>;

TEST_CASE("EntityViewConstructor") {
    ecs::World world;
	world.Init();

	auto eAB = ecs::Construct<EntityAB>(world)
		.Construct<ComponentA>()
		.Construct<ComponentB>();

	auto eAC = ecs::Construct<EntityAC>(world)
		.Construct<ComponentA>()
		.Construct<ComponentC>();

	auto eBC = ecs::Construct<EntityBC>(world)
		.Construct<ComponentB>()
		.Construct<ComponentC>();

	world.ExecuteCommands();

	using QueryAB = ecs::Query<>
		::Include<ComponentA>
		::Include<ComponentB>;

	using QueryAC = ecs::Query<>
		::Include<ComponentA>
		::Include<ComponentC>;

	using QueryBC = ecs::Query<>
		::Include<ComponentB>
		::Include<ComponentC>;

	bool checkAB = false;
	bool checkAC = false;
	bool checkBC = false;

	auto worldView = ecs::WorldView<ComponentA, ComponentB, ComponentC>(world);

	for ([[maybe_unused]] auto e : QueryAB::Iterate(worldView)) {
		checkAB = true;
	}
	REQUIRE(checkAB == true);

	for ([[maybe_unused]] auto e : QueryAC::Iterate(worldView)) {
		checkAC = true;
	}
	REQUIRE(checkAC == true);

	for ([[maybe_unused]] auto e : QueryBC::Iterate(worldView)) {
		checkBC = true;
	}
	REQUIRE(checkBC == true);
}