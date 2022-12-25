#pragma once

#include <ECS/System.h>

#include "CharacterComponent.h"
#include "LevelComponent.h"

// Systems may be templates as well as components if the logic is shared

class MainLevelOutputSystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const CharacterComponent,
		const TLevelComponent<ELevelName::MainLevelSlot>
	>;

	void Update(WorldView& world, float dt);
};

class ExtraLevelOutputSystemA : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const CharacterComponent,
		const TLevelComponent<ELevelName::LevelSlot1>
	>;

	void Update(WorldView& world, float dt);
};

class ExtraLevelOutputSystemB : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const CharacterComponent,
		const TLevelComponent<ELevelName::LevelSlot2>
	>;

	void Update(WorldView& world, float dt);
};

class PaperdollLevelOutputSystem : public ecs::System {
public:
	using WorldView = ecs::WorldView<
		const CharacterComponent,
		const TLevelComponent<ELevelName::PaperdollLevelSlot>
	>;

	void Update(WorldView& world, float dt);
};