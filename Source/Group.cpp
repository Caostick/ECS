#include <ECS/Group.h>
#include <ECS/Component.h>
#include <ECS/Assert.h>

// 0 - Structure of arrays
// 1 - Array of structures
#define USE_ARRAY_OF_STRUCTURES_STORAGE 0

ecs::Group::Group(const Bitset& bits, uint32_t groupIndex) : m_EntityTypeBits(bits)
, m_GroupIndex(groupIndex)
, m_EntityCount(0)
, m_EntitySize(0) {
#if USE_ARRAY_OF_STRUCTURES_STORAGE
	for (uint32_t i = 0; i < MaxComponentCount; ++i) {
		const bool bitIsSet = m_EntityTypeBits[i];
		m_EntitySize += bitIsSet ? ComponentInfo::s_ComponentSizeInfo[i] : 0;

		m_ComponentOffsets[i] = bitIsSet ? m_EntitySize : -1;
	}
#else
	uint32_t componentPoolOffset = 0;
	for (uint32_t i = 0; i < MaxComponentCount; ++i) {
		const bool bitIsSet = m_EntityTypeBits[i];
		m_EntitySize += bitIsSet ? ComponentInfo::s_ComponentSizeInfo[i] : 0;

		m_ComponentOffsets[i] = bitIsSet ? componentPoolOffset : -1;
		componentPoolOffset += bitIsSet ? (ComponentInfo::s_ComponentSizeInfo[i] * EntitiesPerGroupDataPage) : 0;
}
#endif
}

ecs::Group::~Group() {
	ECSAssert(m_EntityCount == 0, "Leak! There are still entities exist!");

	for (auto* pageData : m_DataPages) {
		delete [] pageData;
	}
	m_DataPages.clear();
}

auto ecs::Group::GetIndex() const -> uint32_t {
	return m_GroupIndex;
}

auto ecs::Group::GetEntityCount() const -> uint32_t {
	return m_EntityCount;
}

bool ecs::Group::IsEmpty() const {
	return m_EntityCount == 0;
}

auto ecs::Group::GetTypeBits() const -> const Bitset& {
	return m_EntityTypeBits;
}

auto ecs::Group::AddEntity(ecs::EntityHandle entity) -> uint32_t {
	const auto entityCapacity = static_cast<uint32_t>(m_DataPages.size() * EntitiesPerGroupDataPage);
	if (m_EntityCount >= entityCapacity) {
		AddDataPage();
	}

	m_EntityBackRefs.push_back(entity);

	return m_EntityCount++;
}

auto ecs::Group::RemoveEntity(uint32_t localIndex) -> EntityHandle {
	const auto lastEntIndex = m_EntityCount - 1;

	EntityHandle changedEntity = m_EntityBackRefs[lastEntIndex];
	if (localIndex != lastEntIndex) {
		for (uint32_t i = 0; i < MaxComponentCount; ++i) {
			if(m_EntityTypeBits[i]) {
				uint8_t* compPtr = GetComponentData(lastEntIndex, ComponentTypeId(i));
				uint8_t* remCompPtr = GetComponentData(localIndex, ComponentTypeId(i));

				ComponentInfo::s_MoveComponentFunc[ComponentTypeId(i)](remCompPtr, compPtr);
			}
		}

		m_EntityBackRefs[localIndex] = changedEntity;
	}

	m_EntityBackRefs.pop_back();
	m_EntityCount--;

	const auto entityCapacity = static_cast<uint32_t>(m_DataPages.size() * EntitiesPerGroupDataPage);
	if (m_EntityCount < entityCapacity - EntitiesPerGroupDataPage) {
		RemoveDataPage();
	}

	return changedEntity;
}

auto ecs::Group::GetEntityBackReference(uint32_t localIndex) -> EntityHandle {
	return m_EntityBackRefs[localIndex];
}

auto ecs::Group::GetComponentData(uint32_t localIndex, ComponentTypeId componentTypeId) -> uint8_t* {
#if USE_ARRAY_OF_STRUCTURES_STORAGE
	const auto pageIndex = localIndex / EntitiesPerGroupDataPage;
	const auto pageLocalIndex = localIndex % EntitiesPerGroupDataPage;

	const uint32_t entityOffset = pageLocalIndex * m_EntitySize;
	const uint32_t componentOffset = m_ComponentOffsets[componentTypeId];

	return m_DataPages[pageIndex] + entityOffset + componentOffset;
#else
	const auto pageIndex = localIndex / EntitiesPerGroupDataPage;
	const auto pageLocalIndex = localIndex % EntitiesPerGroupDataPage;

	const uint32_t componentPoolOffset = m_ComponentOffsets[componentTypeId];
	const uint32_t componentOffset = pageLocalIndex * ComponentInfo::s_ComponentSizeInfo[componentTypeId];

	return m_DataPages[pageIndex] + componentPoolOffset + componentOffset;
#endif
}

void ecs::Group::AddDataPage() {
	uint8_t* pageData = new uint8_t[m_EntitySize * EntitiesPerGroupDataPage];
	m_DataPages.push_back(pageData);
}

void ecs::Group::RemoveDataPage() {
	delete [] m_DataPages[m_DataPages.size() - 1];
	m_DataPages.pop_back();
}