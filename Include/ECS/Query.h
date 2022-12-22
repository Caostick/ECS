#pragma once

#include <ECS/Common.h>
#include <ECS/Component.h>
#include <ECS/Group.h>
#include <ECS/WorldView.h>
#include <ECS/World.h>

namespace ecs {
	template<typename TL = TypeList<>, typename WVTL = TypeList<>>
	class QueryResult {
	public:
		QueryResult(Group* group, uint32_t entityLocalIndex);

		template<typename T>
		auto Get() -> typename std::conditional_t<Contains<std::remove_const_t<T>, WVTL>::value, T&, const T&>;

		operator EntityHandle() const;
	private:
		Group* m_Group;
		uint32_t m_EntityLocalIndex;
	};

	template<typename TL_INCLUDE = TypeList<>, typename TL_EXCLUDE = TypeList<>, typename TL_ADDED = TypeList<>, typename TL_REMOVED = TypeList<>>
	struct Query {
		template<typename T, typename = std::enable_if_t<!std::is_const_v<T>, void>>
		using Include = Query<typename Append<const T, TL_INCLUDE>::type, TL_EXCLUDE, TL_ADDED, TL_REMOVED>;

		template<typename T, typename = std::enable_if_t<!std::is_const_v<T>, void>>
		using Exclude = Query<TL_INCLUDE, typename Append<const T, TL_EXCLUDE>::type, TL_ADDED, TL_REMOVED>;

		template<typename T, typename = std::enable_if_t<!std::is_const_v<T>, void>>
		using Added = Query<TL_INCLUDE, TL_EXCLUDE, typename Append<const T, TL_ADDED>::type, TL_REMOVED>;

		template<typename T, typename = std::enable_if_t<!std::is_const_v<T>, void>>
		using Removed = Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, typename Append<const T, TL_REMOVED>::type>;

		using QRT1 = typename Append<TL_ADDED, TL_REMOVED>::type;
		using QRT2 = typename Append<TL_INCLUDE, typename QRT1>::type;
		using QueryResultList = typename RemoveDuplicates<typename QRT2>::type;

		template<typename... Args>
		static auto Iterate(WorldView<Args...>& worldView)->Vector<QueryResult<QueryResultList, typename ExtendWithConst<TypeList<Args...>>::type>>;
	};

	struct QueryRegistry {
		static QueryCounterType s_QueryCount;
	};

	template<typename T>
	struct QueryTypeInfo {
	public:
		static QueryTypeId s_QueryTypeId;

		static auto GetQueryId() -> QueryTypeId;

	private:
		static auto GetQueryIdStatic() -> QueryTypeId;
	};
}

//ecs::Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, TL_REMOVED>

template<typename T>
ecs::QueryTypeId ecs::QueryTypeInfo<T>::s_QueryTypeId = ecs::QueryTypeInfo<T>::GetQueryIdStatic();

#include <ECS/Query.inl>