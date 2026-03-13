#include "Common.h"

namespace {
	struct ComplexComponent0 {
		ComplexComponent0() = default;
		ComplexComponent0(ComplexComponent0&&) = default;

		ComplexComponent0(const ComplexComponent0&) = delete;
		ComplexComponent0& operator = (const ComplexComponent0&) = delete;
		ComplexComponent0& operator = (ComplexComponent0&&) = delete;

		std::string field;
	};

	struct ComplexComponentA {
		ComplexComponentA() = default;
		ComplexComponentA(ComplexComponentA&&) = default;

		ComplexComponentA(const ComplexComponentA&) = delete;
		ComplexComponentA& operator = (const ComplexComponentA&) = delete;
		ComplexComponentA& operator = (ComplexComponentA&&) = delete;

		std::string field;
	};

	struct ComplexComponentB {
		ComplexComponentB() = default;
		ComplexComponentB(ComplexComponentB&&) = default;

		ComplexComponentB(const ComplexComponentB&) = delete;
		ComplexComponentB& operator = (const ComplexComponentB&) = delete;
		ComplexComponentB& operator = (ComplexComponentB&&) = delete;

		std::string field;
		std::vector<std::string> fields;
	};
}

TEST_CASE("ComplexCommands") {

	// Test case for crash:
	// During single command execution:
	// Change 1st entity layout and move to the end of new group B
	// Change 2nd entity layout and remove it from B, it moves data from last one to removed entity position,
	// but data for last one is not yet initialized by moving data from command buffer...

	ecs::World world;
	world.Init();

	// Initially in group A
	auto e1 = world.CreateEntity();
	world.AttachComponent<ComplexComponentA>(e1);

	// Initially in group AB
	auto e2 = world.CreateEntity();
	world.AttachComponent<ComplexComponentA>(e2);
	world.AttachComponent<ComplexComponentB>(e2);

	world.ExecuteCommands();

	// Move to group AB
	world.AttachComponent<ComplexComponentB>(e1);

	// Move from group AB
	world.AttachComponent<ComplexComponent0>(e2);

	// Crash was here
	world.ExecuteCommands();

	world.DestroyEntity(e1);
	world.DestroyEntity(e2);
}