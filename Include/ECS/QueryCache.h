#pragma once

#include <ECS/Common.h>

namespace ecs {
	struct Group;
}

namespace ecs {
	class QueryCache {
	public:
		bool m_HasLayout = false;
		Bitset m_AddedBits;
		Bitset m_RemovedBits;
		Bitset m_IncludeBits;
		Bitset m_ExcludeBits;

		Vector<Group*> m_Groups;
	};
}