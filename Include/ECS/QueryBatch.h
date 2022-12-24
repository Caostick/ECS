#pragma once

#include <ECS/Common.h>
#include <ECS/QueryCache.h>
#include <ECS/QueryResult.h>
#include <ECS/World.h>
#include <ECS/WorldTLS.h>
#include <ECS/Entity.h>

namespace ecs {
	template<typename TL = TypeList<>, typename WVTL = TypeList<>>
	class QueryBatch {
	public:
		QueryBatch(const World& world, QueryCache& cache);

		class Iterator {
		public:
			Iterator() = delete;
			Iterator(const Iterator&) = default;
			Iterator(Iterator&&) = default;
			Iterator(const World& world, QueryCache& cache, uint32_t groupIdx, uint32_t entityIdx);
			Iterator(const World& world, QueryCache& cache, uint32_t groupIdx, uint32_t entityIdx, bool end);

			auto operator++()->Iterator&;
			bool operator != (const Iterator& other) const;
			auto operator* () const->QueryResult<TL, WVTL>;

		private:
			bool IsValid() const;
			void Advance();

		private:
			const World& m_World;
			QueryCache& m_Cache;
			uint32_t m_GroupIdx;
			uint32_t m_EntityIdx;
		};

		auto begin()->Iterator;
		auto end()->Iterator;

	private:
		const World& m_World;
		QueryCache& m_Cache;
		uint32_t m_GroupBegin;
		uint32_t m_EntityBegin;
		uint32_t m_GroupEnd;
		uint32_t m_EntityEnd;
	};
}

#include <ECS/QueryBatch.inl>