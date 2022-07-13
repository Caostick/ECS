#pragma once

#include <ECS/Source/Common.h>

namespace ecs {
	struct Entity {
		uint32_t m_GroupIndex : 12;
		uint32_t m_LocalIndex : 20;
	};

	struct EntityLayout {
		ecs::EntityLayoutState m_State;
		ecs::Bitset m_RequiredBits;
	};
}