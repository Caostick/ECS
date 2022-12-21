#pragma once

#include <ECS/Common.h>

namespace ecs {
	struct Entity {
		uint32_t m_GroupIndex : 12;
		uint32_t m_LocalIndex : 20;
	};

	struct EntityLayout {
		EntityLayoutState m_State;
		Bitset m_RequiredBits;
	};
}