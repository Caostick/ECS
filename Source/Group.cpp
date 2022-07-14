#include <ECS/Group.h>
#include <ECS/Component.h>
#include <ECS/Assert.h>

ecs::Group::Group(const ecs::Bitset& bits, uint32_t groupIndex) : m_EntityTypeBits(bits)
, m_GroupIndex(groupIndex)
, m_EntityCount(0)
, m_EntitySize(0) {
	for (uint32_t i = 0; i < ecs::MaxComponentCount; ++i) {
		const bool bitIsSet = m_EntityTypeBits[i];
		m_ComponentOffsets[i] = bitIsSet ? m_EntitySize : -1;
		m_EntitySize += bitIsSet ? ecs::ComponentInfo::s_ComponentSizeInfo[i] : 0;
	}
}

ecs::Group::~Group() {
	ECSAssert(m_EntityCount == 0, "Leak! There are still entities exist!");

	for (auto* pageData : m_DataPages) {
		delete [] pageData;
	}
	m_DataPages.clear();
}

uint32_t ecs::Group::GetIndex() const {
	return m_GroupIndex;
}

uint32_t ecs::Group::GetEntityCount() const {
	return m_EntityCount;
}

bool ecs::Group::IsEmpty() const {
	return m_EntityCount == 0;
}

const ecs::Bitset& ecs::Group::GetTypeBits() const {
	return m_EntityTypeBits;
}

uint32_t ecs::Group::AddEntity(ecs::EntityHandle entity) {
	const auto entityCapacity = static_cast<uint32_t>(m_DataPages.size() * EntitiesPerGroupDataPage);
	if (m_EntityCount >= entityCapacity) {
		AddDataPage();
	}

	m_EntityBackRefs.push_back(entity);

	return m_EntityCount++;
}

ecs::EntityHandle ecs::Group::RemoveEntity(uint32_t localIndex) {
	const auto lastEntIndex = m_EntityCount - 1;

	ecs::EntityHandle changedEntity = m_EntityBackRefs[lastEntIndex];
	if (localIndex != lastEntIndex) {
		for (uint32_t i = 0; i < ecs::MaxComponentCount; ++i) {
			if(m_EntityTypeBits[i]) {
				uint8_t* compPtr = GetComponentData(lastEntIndex, ecs::ComponentTypeId(i));
				uint8_t* remCompPtr = GetComponentData(localIndex, ecs::ComponentTypeId(i));

				ecs::ComponentInfo::s_MoveComponentFunc[ecs::ComponentTypeId(i)](remCompPtr, compPtr);
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

ecs::EntityHandle ecs::Group::GetEntityBackReference(uint32_t localIndex) {
	return m_EntityBackRefs[localIndex];
}

uint8_t* ecs::Group::GetComponentData(uint32_t localIndex, ecs::ComponentTypeId componentTypeId) {
	const auto pageIndex = localIndex / EntitiesPerGroupDataPage;
	const auto pageLocalIndex = localIndex % EntitiesPerGroupDataPage;

	const uint32_t entityOffset = pageLocalIndex * m_EntitySize;
	const uint32_t componentOffset = m_ComponentOffsets[componentTypeId];

	return m_DataPages[pageIndex] + entityOffset + componentOffset;
}

void ecs::Group::AddDataPage() {
	uint8_t* pageData = new uint8_t[m_EntitySize * EntitiesPerGroupDataPage];
	m_DataPages.push_back(pageData);
}

void ecs::Group::RemoveDataPage() {
	delete [] m_DataPages[m_DataPages.size() - 1];
	m_DataPages.pop_back();
}