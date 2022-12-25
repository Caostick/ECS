#pragma once

enum class ELevelName {
	MainLevelSlot,
	LevelSlot1,
	LevelSlot2,

	PaperdollLevelSlot
};

template<ELevelName LVL_IDX>
struct TLevelComponent {};
