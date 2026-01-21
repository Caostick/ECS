#pragma once

#include <ECS/Common.h>
#include <ECS/Bitset.h>
#include <ECS/Component.h>

namespace ecs {
	namespace utils {
		template<typename TL>
		struct ComponentTypeBits {
			static auto Bits() -> ecs::Bitset;
		};

		template<typename TL>
		struct MutableComponentTypeBits {
			static auto Bits() -> ecs::Bitset;
		};
	}
}

#include <ECS/Utils.inl>