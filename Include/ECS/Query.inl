#pragma once

#include <ECS/QueryCache.h>
#include <ECS/Component.h>
#include <ECS/World.h>
#include <ECS/Utils.h>

template<typename TL_INCLUDE /*= ecs::TypeList<>*/, typename TL_EXCLUDE /*= ecs::TypeList<>*/, typename TL_ADDED /*= ecs::TypeList<>*/, typename TL_REMOVED /*= ecs::TypeList<>*/>
template<typename... Args>
auto ecs::Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, TL_REMOVED>::Iterate(WorldView<Args...>& worldView)->QueryBatch<QueryResultList, typename ExtendWithConst<TypeList<Args...>>::type> {

	using WorldViewTypeList = typename ExtendWithConst<TypeList<Args...>>::type;

	static_assert(ContainsList<TL_INCLUDE, WorldViewTypeList>::value, "WorldView doesn't contain some query components!");
	static_assert(ContainsList<TL_ADDED, WorldViewTypeList>::value, "WorldView doesn't contain some query components!");
	static_assert(ContainsList<TL_REMOVED, WorldViewTypeList>::value, "WorldView doesn't contain some query components!");

	World& world = worldView;

	const auto queryId = QueryTypeInfo<Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, TL_REMOVED>>::GetQueryId();
	auto& queryCache = world.GetQueryCache(queryId);

	if (!queryCache.m_HasLayout) {
		queryCache.m_AddedBits = utils::ComponentTypeBits<TL_ADDED>::Bits();
		queryCache.m_RemovedBits = utils::ComponentTypeBits<TL_REMOVED>::Bits();
		queryCache.m_IncludeBits = queryCache.m_AddedBits | utils::ComponentTypeBits<TL_INCLUDE>::Bits();
		queryCache.m_ExcludeBits = utils::ComponentTypeBits<TL_EXCLUDE>::Bits();
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

