#include <ECS/World.h>

#include "CharacterUpdateSystem.h"
#include "LevelOutputSystem.h"

template<ELevelName LVL_NAME>
auto CreateCharacter(ecs::World& world, const char* name) {
	auto e = world.CreateEntity();
	world.AttachComponent<CharacterComponent>(e, name);
	world.AttachComponent<TLevelComponent<LVL_NAME>>(e);

	return e;
}

int main() {
	constexpr float deltaTime = 1.0f / 60.0f;

	// One of the ways to implement multiple levels
	// Adding entities to level happens by attaching certain level slot to entity
	// Certain levels can be attached to level slots in another level manager or something like this

	ecs::World world;

	world.Init();
	world.RegisterSystem<CharacterUpdateSystem>();
	world.RegisterSystem<MainLevelOutputSystem>();
	world.RegisterSystem<ExtraLevelOutputSystemA>();
	world.RegisterSystem<ExtraLevelOutputSystemB>();
	world.RegisterSystem<PaperdollLevelOutputSystem>();

	CreateCharacter<ELevelName::MainLevelSlot>(world, "Hero");
	CreateCharacter<ELevelName::MainLevelSlot>(world, "Town NPC");
	CreateCharacter<ELevelName::MainLevelSlot>(world, "Another town NPC");

	CreateCharacter<ELevelName::LevelSlot1>(world, "Monster A_1");
	CreateCharacter<ELevelName::LevelSlot1>(world, "Monster A_2");

	CreateCharacter<ELevelName::LevelSlot2>(world, "Boss of the dungeon");

	CreateCharacter<ELevelName::PaperdollLevelSlot>(world, "Hero");

	world.ExecuteCommands();
	world.Update(deltaTime);
	world.FinishAllCommands();

	world.Release();

	return 0;
}