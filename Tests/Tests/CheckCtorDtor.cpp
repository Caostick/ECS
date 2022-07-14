#include "Common.h"

namespace {
	int GlobalComponentRefcount = 0;

	struct ComponentExtend {
		ComponentExtend() = default;
		ComponentExtend(const ComponentExtend&) = delete;
		ComponentExtend(ComponentExtend&&) = default;
		ComponentExtend& operator = (const ComponentExtend&) = delete;
		ComponentExtend& operator = (ComponentExtend&&) = default;

		int m_Whatever = 0;
	};

	struct ComponentRefcount {
		ComponentRefcount() {
			GlobalComponentRefcount++;
		}

		~ComponentRefcount() {
			GlobalComponentRefcount--;
		}

		ComponentRefcount(const ComponentRefcount&) = delete;

		ComponentRefcount(ComponentRefcount&&) = default;

		ComponentRefcount& operator = (const ComponentRefcount&) = delete;

		ComponentRefcount& operator = (ComponentRefcount&&) = default;

		int m_Whatever = 0;
	};
}

TEST_CASE("CheckCtorDtor") {

	ecs::World world;
	world.Init();

	ecs::Vector<ecs::EntityHandle> entSet;

	for (int i = 0; i < 3; ++i) {
		ecs::EntityHandle e = world.CreateEntity();
		world.AttachComponent<ComponentRefcount>(e);
		entSet.push_back(e);
	}
	REQUIRE(GlobalComponentRefcount == 3);

	world.FinishAllCommands();
	REQUIRE(GlobalComponentRefcount == 3);

	for (const auto& e : entSet) {
		world.AttachComponent<ComponentExtend>(e);
	}
	world.FinishAllCommands();
	REQUIRE(GlobalComponentRefcount == 3);

	for (const auto& e : entSet) {
		world.DetachComponent<ComponentExtend>(e);
	}
	world.FinishAllCommands();
	REQUIRE(GlobalComponentRefcount == 3);

	for (const auto& e : entSet) {
		world.DetachComponent<ComponentRefcount>(e);
		world.DestroyEntity(e);
	}
	world.FinishAllCommands();

	REQUIRE(GlobalComponentRefcount == 0);
	world.Release();
	REQUIRE(GlobalComponentRefcount == 0);
}