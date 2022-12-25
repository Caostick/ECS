#include "LevelOutputSystem.h"

#include <ECS/Query.h>

#include <iostream>

void MainLevelOutputSystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<CharacterComponent>
		::Include<TLevelComponent<ELevelName::MainLevelSlot>>;

	std::cout << "Main Level Characters:\n";

	for (auto e : Query::Iterate(world)) {
		auto& characterComponent = e.Get<CharacterComponent>();

		std::cout << "  - " << characterComponent.Name << "\n";
	}
}

void ExtraLevelOutputSystemA::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<CharacterComponent>
		::Include<TLevelComponent<ELevelName::LevelSlot1>>;

	std::cout << "Extra Level A Characters:\n";

	for (auto e : Query::Iterate(world)) {
		auto& characterComponent = e.Get<CharacterComponent>();

		std::cout << "  - " << characterComponent.Name << "\n";
	}
}

void ExtraLevelOutputSystemB::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<CharacterComponent>
		::Include<TLevelComponent<ELevelName::LevelSlot2>>;

	std::cout << "Extra Level B Characters:\n";

	for (auto e : Query::Iterate(world)) {
		auto& characterComponent = e.Get<CharacterComponent>();

		std::cout << "  - " << characterComponent.Name << "\n";
	}
}

void PaperdollLevelOutputSystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<CharacterComponent>
		::Include<TLevelComponent<ELevelName::PaperdollLevelSlot>>;

	std::cout << "Paperdoll Characters:\n";

	for (auto e : Query::Iterate(world)) {
		auto& characterComponent = e.Get<CharacterComponent>();

		std::cout << "  - " << characterComponent.Name << "\n";
	}
}