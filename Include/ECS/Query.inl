#pragma once

#include <ECS/QueryCache.h>
#include <ECS/Component.h>
#include <ECS/World.h>

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

template<typename TL_INCLUDE /*= ecs::TypeList<>*/, typename TL_EXCLUDE /*= ecs::TypeList<>*/, typename TL_ADDED /*= ecs::TypeList<>*/, typename TL_REMOVED /*= ecs::TypeList<>*/>
template<typename... Args>
auto ecs::Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, TL_REMOVED>::Iterate(WorldView<Args...>& worldView)->QueryBatch<QueryResultList, typename ExtendWithConst<TypeList<Args...>>::type> {

	using WorldViewTypeList = typename ExtendWithConst<TypeList<Args...>>::type;

	static_assert(ContainsList<TL_INCLUDE, WorldViewTypeList>::value, "WorldView doesn't contain any query components!");
	static_assert(ContainsList<TL_ADDED, WorldViewTypeList>::value, "WorldView doesn't contain any query components!");
	static_assert(ContainsList<TL_REMOVED, WorldViewTypeList>::value, "WorldView doesn't contain any query components!");

	World& world = worldView;

	const auto queryId = QueryTypeInfo<Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, TL_REMOVED>>::GetQueryId();
	auto& queryCache = world.GetQueryCache(queryId);

	if (!queryCache.m_HasLayout) {
		queryCache.m_AddedBits = ComponentTypeBits<TL_ADDED>::Bits();
		queryCache.m_RemovedBits = ComponentTypeBits<TL_REMOVED>::Bits();
		queryCache.m_IncludeBits = queryCache.m_AddedBits | ComponentTypeBits<TL_INCLUDE>::Bits();
		queryCache.m_ExcludeBits = ComponentTypeBits<TL_EXCLUDE>::Bits();
		queryCache.m_HasLayout = true;
		world.InitQueryCache(queryCache);
	}

	return QueryBatch<QueryResultList, typename ExtendWithConst<TypeList<Args...>>::type>(world, queryCache);
}





template<typename T>
auto ecs::QueryTypeInfo<T>::GetQueryId() -> QueryTypeId {
	return s_QueryTypeId;
}

template<typename T>
auto ecs::QueryTypeInfo<T>::GetQueryIdStatic() -> QueryTypeId {
	static QueryTypeId id = QueryRegistry::s_QueryCount++;

	return id;
}

