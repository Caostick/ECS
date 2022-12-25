#include "CharacterUpdateSystem.h"

#include <ECS/Query.h>

void CharacterUpdateSystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<CharacterComponent>;

	for (auto e : Query::Iterate(world)) {
		[[maybe_unused]] auto& characterComponent = e.Get<CharacterComponent>();
	}
}