#pragma once

#include <ECS/Source/Common.h>
#include <ECS/Source/Component.h>
#include <ECS/Source/Group.h>
#include <ECS/Include/ECS/WorldView.h>
#include <ECS/Include/ECS/World.h>

namespace ecs {
	template<typename TL = ecs::TypeList<>, typename WVTL = ecs::TypeList<>>
	class QueryResult {
	public:
		QueryResult(ecs::Group* group, uint32_t entityLocalIndex);

		template<typename T>
		auto Get() -> typename std::conditional_t<ecs::Contains<std::remove_const_t<T>, WVTL>::value, T&, const T&>;

		operator ecs::EntityHandle() const;
	private:
		ecs::Group* m_Group;
		uint32_t m_EntityLocalIndex;
	};

	template<typename TL_INCLUDE = ecs::TypeList<>, typename TL_EXCLUDE = ecs::TypeList<>, typename TL_ADDED = ecs::TypeList<>, typename TL_REMOVED = ecs::TypeList<>>
	struct Query {
		template<typename T, typename = std::enable_if_t<!std::is_const_v<T>, void>>
		using Include = Query<typename ecs::Append<const T, TL_INCLUDE>::type, TL_EXCLUDE, TL_ADDED, TL_REMOVED>;

		template<typename T, typename = std::enable_if_t<!std::is_const_v<T>, void>>
		using Exclude = Query<TL_INCLUDE, typename ecs::Append<const T, TL_EXCLUDE>::type, TL_ADDED, TL_REMOVED>;

		template<typename T, typename = std::enable_if_t<!std::is_const_v<T>, void>>
		using Added = Query<TL_INCLUDE, TL_EXCLUDE, typename ecs::Append<const T, TL_ADDED>::type, TL_REMOVED>;

		template<typename T, typename = std::enable_if_t<!std::is_const_v<T>, void>>
		using Removed = Query<TL_INCLUDE, TL_EXCLUDE, TL_ADDED, typename ecs::Append<const T, TL_REMOVED>::type>;

		using QRT1 = typename ecs::Append<TL_ADDED, TL_REMOVED>::type;
		using QRT2 = typename ecs::Append<TL_INCLUDE, typename QRT1>::type;
		using QueryResultList = typename ecs::RemoveDuplicates<typename QRT2>::type;

		template<typename... Args>
		static auto Iterate(ecs::WorldView<Args...>& worldView)->ecs::Vector<ecs::QueryResult<typename QueryResultList, typename ecs::ExtendWithConst<ecs::TypeList<Args...>>::type>>;
	};
}

#include <ECS/Source/Query.inl>