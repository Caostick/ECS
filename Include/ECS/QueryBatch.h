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
		QueryBatch(const World& world, const QueryCache& cache);

		struct Sentinel {};

		class Iterator {
		public:
			Iterator() = delete;
			Iterator(const Iterator&) = default;
			Iterator(Iterator&&) = default;
			Iterator(const World& world, const QueryCache& cache, uint32_t groupIdx, uint32_t entityIdx);

			auto operator++()->Iterator&;
			bool operator != (const Sentinel& other) const;
			auto operator* () const->QueryResult<TL, WVTL>;

		private:
			bool IsValid() const;
			void Advance();

		private:
			const World& m_World;
			const QueryCache& m_Cache;
			uint32_t m_GroupIdx;
			uint32_t m_GroupIdxEnd;
			uint32_t m_EntityIdx;
		};

		auto begin()->Iterator;
		auto end()->Sentinel;

	private:
		const World& m_World;
		const QueryCache& m_Cache;
	};
}

#include <ECS/QueryBatch.inl>