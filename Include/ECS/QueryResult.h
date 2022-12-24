#pragma once

#include <ECS/Common.h>
#include <ECS/EntityHandle.h>

namespace ecs {
    struct Group;
}

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
}

#include <ECS/QueryResult.inl>