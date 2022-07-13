#pragma once

namespace {
	template<typename TL>
	struct ComponentTypeBits {
		static auto Bits() -> ecs::Bitset {
			if constexpr (ecs::IsEmpty<TL>::value) {
				return ecs::Bitset();

			} else {
				using Head = TL::Head;
				using Tail = TL::Tail;

				return ecs::Component<std::remove_const<Head>::type>::GetTypeBitmask() | ComponentTypeBits<Tail>::Bits();

			}
		}
	};
}

template<typename TL, typename WVTL>
ecs::QueryResult<TL, WVTL>::QueryResult(ecs::Group* group, uint32_t entityLocalIndex)
	: m_Group(group)
	, m_EntityLocalIndex(entityLocalIndex) {}

template<typename TL, typename WVTL>
template<typename T>
typename std::conditional_t<ecs::Contains<std::remove_const_t<T>, WVTL>::value, T&, const T&>
ecs::QueryResult<TL, WVTL>::Get() {
	static_assert(!std::is_const_v<T>, "Modifier const is deprecated!");
	static_assert(ecs::Contains<const T, TL>::value, "Component of type is inaccessible!");
	static_assert(!std::is_empty<T>::value, "Can't access empty component!");

	return *m_Group->GetComponent<std::remove_const_t<T>>(m_EntityLocalIndex);
}

template<typename TL, typename WVTL>
ecs::QueryResult<TL, WVTL>::operator ecs::EntityHandle () const {
	return m_Group->GetEntityBackReference(m_EntityLocalIndex);
}

template<typename TL_INCLUDE /*= ecs::TypeList<>*/, typename TL_EXCLUDE /*= ecs::TypeList<>*/, typename TL_ADDED /*= ecs::TypeList<>*/, typename TL_REMOVED /*= ecs::TypeList<>*/>
template<typename... Args>
auto ecs::Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, TL_REMOVED>::Iterate(ecs::WorldView<Args...>& worldView)->ecs::Vector<ecs::QueryResult<typename QueryResultList, typename ecs::ExtendWithConst<ecs::TypeList<Args...>>::type>> {
	using WorldViewTypeList = typename ecs::ExtendWithConst<ecs::TypeList<Args...>>::type;

	static_assert(ecs::ContainsList<TL_INCLUDE, WorldViewTypeList>::value, "WorldView doesn't contain any query components!");
	static_assert(ecs::ContainsList<TL_ADDED, WorldViewTypeList>::value, "WorldView doesn't contain any query components!");
	static_assert(ecs::ContainsList<TL_REMOVED, WorldViewTypeList>::value, "WorldView doesn't contain any query components!");

	const ecs::Bitset addedBits = ComponentTypeBits<TL_ADDED>::Bits();
	const ecs::Bitset removedBits = ComponentTypeBits<TL_REMOVED>::Bits();
	const ecs::Bitset includeBits = addedBits | ComponentTypeBits<TL_INCLUDE>::Bits();
	const ecs::Bitset excludeBits = ComponentTypeBits<TL_EXCLUDE>::Bits();

	ecs::Vector<ecs::QueryResult<typename QueryResultList, typename ecs::ExtendWithConst<ecs::TypeList<Args...>>::type>> results;

	// @TODO: Make proper iterator instead of filling and returning array
	ecs::World& world = worldView;
	for (const auto group : world.m_Groups) {
		if (!group->IsEmpty()) {
			const auto& groupBits = group->GetTypeBits();
			const bool includeFits = ((groupBits & includeBits) == includeBits);
			const bool excludeFits = ((~groupBits & excludeBits) == excludeBits);

			if (includeFits && excludeFits) {
				for (uint32_t i = 0; i < group->GetEntityCount(); ++i) {
					const auto entityHandle = group->GetEntityBackReference(i);

					const auto threadIdx = ecs::ToThreadIndex(entityHandle);
					const auto entityIdx = ecs::ToEntityIndex(entityHandle);
					auto& tls = world.m_TLS[threadIdx];
					auto& layout = tls.GetEntityLayout(entityIdx);

					const bool attachedFits = ((layout.m_State.m_BitsJustAttached & addedBits) == addedBits);
					const bool detachedFits = ((layout.m_State.m_BitsJustDetached & removedBits) == removedBits);
					const bool detachedNotIncludedFits = (layout.m_State.m_BitsJustDetached & includeBits).none();

					if (attachedFits && detachedFits && detachedNotIncludedFits) {
						results.emplace_back(ecs::QueryResult<typename QueryResultList, typename ecs::ExtendWithConst<ecs::TypeList<Args...>>::type>(group, i));
					}
				}
			}
		}
	}

	return results;
}