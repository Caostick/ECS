#pragma once

#include <ECS/Component.h>

template<typename TL>
auto ecs::utils::ComponentTypeBits<TL>::Bits() -> ecs::Bitset {
	if constexpr (ecs::IsEmpty<TL>::value) {
		return ecs::Bitset();

	} else {
		using Head = TL::Head;
		using Tail = TL::Tail;

		return ecs::Component<std::remove_const<Head>::type>::GetTypeBitmask() | ComponentTypeBits<Tail>::Bits();
	}
}

template<typename TL>
auto ecs::utils::MutableComponentTypeBits<TL>::Bits() -> ecs::Bitset {
	if constexpr (ecs::IsEmpty<TL>::value) {
		return ecs::Bitset();

	} else {
		using Head = TL::Head;
		using Tail = TL::Tail;

		if constexpr (std::is_const<Head>::value) {
			return ecs::Bitset() | MutableComponentTypeBits<Tail>::Bits();
		} else {
			return ecs::Component<std::remove_const<Head>::type>::GetTypeBitmask() | MutableComponentTypeBits<Tail>::Bits();
		}
	}
}