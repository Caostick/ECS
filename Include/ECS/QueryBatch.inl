#pragma once

template<typename TL, typename WVTL>
ecs::QueryBatch<TL, WVTL>::QueryBatch(
	const World& world,
	const QueryCache& cache)
	: m_World(world)
	, m_Cache(cache) {
}

template<typename TL, typename WVTL>
auto ecs::QueryBatch<TL, WVTL>::begin() -> Iterator {
	return Iterator(m_World, m_Cache, 0, 0);
}

template<typename TL, typename WVTL>
auto ecs::QueryBatch<TL, WVTL>::end() -> Sentinel {
	return {};
}

template<typename TL, typename WVTL>
ecs::QueryBatch<TL, WVTL>::Iterator::Iterator(const World& world,
											  const QueryCache& cache,
											  uint32_t groupIdx,
											  uint32_t entityIdx)
	: m_World(world)
	, m_Cache(cache)
	, m_GroupIdx(groupIdx)
	, m_GroupIdxEnd(uint32_t(m_Cache.m_Groups.size()))
	, m_EntityIdx(entityIdx) {
	while (!IsValid()) {
		Advance();
	}
}

template<typename TL, typename WVTL>
auto ecs::QueryBatch<TL, WVTL>::Iterator::operator++() -> Iterator& {
	do {
		Advance();
	} while (!IsValid());

	return *this;
}

template<typename TL, typename WVTL>
bool ecs::QueryBatch<TL, WVTL>::Iterator::operator != (const Sentinel&) const {
	return (m_GroupIdx != m_GroupIdxEnd) || (m_EntityIdx != 0);
}

template<typename TL, typename WVTL>
auto ecs::QueryBatch<TL, WVTL>::Iterator::operator* () const -> QueryResult<TL, WVTL> {
	auto group = m_Cache.m_Groups[m_GroupIdx];
	auto idx = m_EntityIdx;

	return QueryResult<TL, WVTL>(group, idx);
}

template<typename TL, typename WVTL>
bool ecs::QueryBatch<TL, WVTL>::Iterator::IsValid() const {
	const uint32_t endGroup = uint32_t(m_Cache.m_Groups.size());
	const uint32_t endEntity = 0;

	if (m_GroupIdx == endGroup && m_EntityIdx == endEntity) {
		return true;
	}

	auto group = m_Cache.m_Groups[m_GroupIdx];
	if (group->IsEmpty()) {
		return false;
	}

	const Bitset& addedBits = m_Cache.m_AddedBits;
	const Bitset& removedBits = m_Cache.m_RemovedBits;
	const Bitset& includeBits = m_Cache.m_IncludeBits;

	const auto entityHandle = group->GetEntityBackReference(m_EntityIdx);
	const auto threadIdx = ToThreadIndex(entityHandle);
	const auto entityIdx = ToEntityIndex(entityHandle);
	const auto& tls = m_World.m_TLS[threadIdx];
	const auto& layout = tls.GetEntityLayout(entityIdx);

	const bool attachedFits = ((layout.m_State.m_BitsJustAttached & addedBits) == addedBits);
	const bool detachedFits = ((layout.m_State.m_BitsJustDetached & removedBits) == removedBits);
	const bool detachedNotIncludedFits = (layout.m_State.m_BitsJustDetached & includeBits).none();

	if (attachedFits && detachedFits && detachedNotIncludedFits) {
		return true;
	}

	return false;
}

template<typename TL, typename WVTL>
void ecs::QueryBatch<TL, WVTL>::Iterator::Advance() {
	m_EntityIdx++;
	if (m_EntityIdx >= m_Cache.m_Groups[m_GroupIdx]->GetEntityCount()) {
		m_GroupIdx++;
		m_EntityIdx = 0;
	}
}