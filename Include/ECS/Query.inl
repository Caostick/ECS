#pragma once

#include <ECS/QueryCache.h>

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
ecs::QueryResult<TL, WVTL>::QueryResult(Group* group, uint32_t entityLocalIndex)
	: m_Group(group)
	, m_EntityLocalIndex(entityLocalIndex) {}

template<typename TL, typename WVTL>
template<typename T>
typename std::conditional_t<ecs::Contains<std::remove_const_t<T>, WVTL>::value, T&, const T&>
ecs::QueryResult<TL, WVTL>::Get() {
	static_assert(Contains<const T, TL>::value, "Component of type is inaccessible!");
	static_assert(!std::is_const_v<T>, "Modifier const is deprecated!");
	static_assert(!std::is_empty<T>::value, "Can't access empty component!");

	return *m_Group->GetComponent<std::remove_const_t<T>>(m_EntityLocalIndex);
}

template<typename TL, typename WVTL>
ecs::QueryResult<TL, WVTL>::operator ecs::EntityHandle () const {
	return m_Group->GetEntityBackReference(m_EntityLocalIndex);
}

template<typename TL_INCLUDE /*= ecs::TypeList<>*/, typename TL_EXCLUDE /*= ecs::TypeList<>*/, typename TL_ADDED /*= ecs::TypeList<>*/, typename TL_REMOVED /*= ecs::TypeList<>*/>
template<typename... Args>
auto ecs::Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, TL_REMOVED>::Iterate(WorldView<Args...>& worldView)->Vector<QueryResult<QueryResultList, typename ExtendWithConst<TypeList<Args...>>::type>> {

	using WorldViewTypeList = typename ExtendWithConst<TypeList<Args...>>::type;

	static_assert(ContainsList<TL_INCLUDE, WorldViewTypeList>::value, "WorldView doesn't contain any query components!");
	static_assert(ContainsList<TL_ADDED, WorldViewTypeList>::value, "WorldView doesn't contain any query components!");
	static_assert(ContainsList<TL_REMOVED, WorldViewTypeList>::value, "WorldView doesn't contain any query components!");

	const Bitset addedBits = ComponentTypeBits<TLAdded>::Bits();
	const Bitset removedBits = ComponentTypeBits<TL_REMOVED>::Bits();
	const Bitset includeBits = addedBits | ComponentTypeBits<TL_INCLUDE>::Bits();
	const Bitset excludeBits = ComponentTypeBits<TL_EXCLUDE>::Bits();

	Vector<QueryResult<QueryResultList, typename ExtendWithConst<TypeList<Args...>>::type>> results;

	World& world = worldView;

	const auto queryId = QueryTypeInfo<Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, TL_REMOVED>>::GetQueryId();
	auto& queryCache = world.GetQueryCache(queryId);

	const bool initialized =
		queryCache.m_AddedBits == addedBits &&
		queryCache.m_RemovedBits == removedBits &&
		queryCache.m_IncludeBits == includeBits &&
		queryCache.m_ExcludeBits == excludeBits;

	if (!initialized) {
		queryCache.m_AddedBits = addedBits;
		queryCache.m_RemovedBits = removedBits;
		queryCache.m_IncludeBits = includeBits;
		queryCache.m_ExcludeBits = excludeBits;
		queryCache.m_HasLayout = true;
		world.InitQueryCache(queryCache);
	}

	for (const auto group : queryCache.m_Groups) {
		if (group->IsEmpty()) {
			continue;
		}

		for (uint32_t i = 0; i < group->GetEntityCount(); ++i) {
			const auto entityHandle = group->GetEntityBackReference(i);

			const auto threadIdx = ToThreadIndex(entityHandle);
			const auto entityIdx = ToEntityIndex(entityHandle);
			auto &tls = world.m_TLS[threadIdx];
			auto &layout = tls.GetEntityLayout(entityIdx);

			const bool attachedFits = ((layout.m_State.m_BitsJustAttached & addedBits) == addedBits);
			const bool detachedFits = ((layout.m_State.m_BitsJustDetached & removedBits) == removedBits);
			const bool detachedNotIncludedFits = (layout.m_State.m_BitsJustDetached & includeBits).none();

			if (attachedFits && detachedFits && detachedNotIncludedFits) {
				results.emplace_back(QueryResult<QueryResultList, typename ExtendWithConst<TypeList<Args...>>::type>(group, i));
			}
		}
	}

	return results;
}





template<typename T>
auto ecs::QueryTypeInfo<T>::GetQueryId() -> QueryTypeId {
	return s_QueryTypeId;
}

template<typename T>
auto ecs::QueryTypeInfo<T>::GetQueryIdStatic() -> QueryTypeId {
	static QueryTypeId id = QueryRegistry::s_QueryCount++;

	printf("Registered Query: %s\n", typeid(T).name());

	return id;
}

