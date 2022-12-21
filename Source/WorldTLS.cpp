#include <ECS/WorldTLS.h>

ecs::WorldTLS::WorldTLS() 
	: m_EntityPoolSize(0)
	, m_EntityCount(0)
	, m_AvailableEntityIndex(0) {
	IncreasePoolSize(InitialEntityCount);
}

auto ecs::WorldTLS::CreateEntity() -> uint32_t {
	if (m_AvailableEntityIndex >= m_EntityPoolSize) {
		IncreasePoolSize(m_EntityPoolSize * 2);
	}

	const uint32_t entityIndex = m_AvailableEntityIndex;
	m_AvailableEntityIndex = m_EntityIndices[m_AvailableEntityIndex];
	m_EntityCount++;

	return entityIndex;
}

void ecs::WorldTLS::MarkForDestroy(uint32_t entityIndex, bool value) {
	m_RemovedEntities[entityIndex] = value;
}

bool ecs::WorldTLS::IsMarkedForDestroy(uint32_t entityIndex) const {
	return m_RemovedEntities[entityIndex];
}

void ecs::WorldTLS::MarkForUpdate(uint32_t entityIndex, bool value) {
	m_DirtyEntities[entityIndex] = value;
}

bool ecs::WorldTLS::IsMarkedForUpdate(uint32_t entityIndex) const {
	return m_DirtyEntities[entityIndex];
}

void ecs::WorldTLS::RequestComponentAttach(uint32_t entityIndex, uint32_t componentTypeId) {
	ECSAssert(m_Layouts[entityIndex].m_State.m_Bits[componentTypeId] == false, "Component already attached!");

	m_Layouts[entityIndex].m_RequiredBits[componentTypeId] = true;
	m_Layouts[entityIndex].m_State.m_BitsJustAttached[componentTypeId] = true;
}

void ecs::WorldTLS::RequestComponentDetach(uint32_t entityIndex, uint32_t componentTypeId) {
	ECSAssert(m_Layouts[entityIndex].m_State.m_Bits[componentTypeId] == true, "Component already detached!");

	m_Layouts[entityIndex].m_RequiredBits[componentTypeId] = false;
	m_Layouts[entityIndex].m_State.m_BitsJustDetached[componentTypeId] = true;
}

void ecs::WorldTLS::RequestAllComponentsDetach(uint32_t entityIndex) {
	m_Layouts[entityIndex].m_State.m_BitsJustDetached = m_Layouts[entityIndex].m_RequiredBits;
	m_Layouts[entityIndex].m_RequiredBits.reset();
}

auto ecs::WorldTLS::GetEntityLayout(uint32_t entityIndex) -> EntityLayout& {
	return m_Layouts[entityIndex];
}

auto ecs::WorldTLS::GetEntity(uint32_t entityIndex) -> Entity& {
	return m_Entities[entityIndex];
}

auto ecs::WorldTLS::GetEntity(uint32_t entityIndex) const -> const Entity& {
	return m_Entities[entityIndex];
}

auto ecs::WorldTLS::GetEntityGroupIndex(uint32_t entityIndex) const -> uint32_t {
	return m_Entities[entityIndex].m_GroupIndex;
}

auto ecs::WorldTLS::GetEntityLocalIndex(uint32_t entityIndex) const -> uint32_t {
	return m_Entities[entityIndex].m_LocalIndex;
}

void ecs::WorldTLS::DestroyEntity(uint32_t entityIndex) {
	m_Entities[entityIndex].m_GroupIndex = 0;
	m_Entities[entityIndex].m_LocalIndex = 0;
	m_RemovedEntities[entityIndex] = false;
	m_DirtyEntities[entityIndex] = false;
	m_Layouts[entityIndex].m_State = EntityLayoutState();
	m_Layouts[entityIndex].m_RequiredBits = 0;

	m_EntityIndices[entityIndex] = m_AvailableEntityIndex;
	m_AvailableEntityIndex = entityIndex;

	m_EntityCount--;
}

void ecs::WorldTLS::ResetCommands() {
	m_CommandBuffer.Reset();
}

auto ecs::WorldTLS::GetCommandBuffer() -> CommandBuffer& {
	return m_CommandBuffer;
}


void ecs::WorldTLS::IncreasePoolSize(uint32_t newPoolSize) {
	m_EntityIndices.resize(newPoolSize);
	m_Entities.resize(newPoolSize);
	m_Layouts.resize(newPoolSize);
	m_RemovedEntities.resize(newPoolSize);
	m_DirtyEntities.resize(newPoolSize);
	for (uint32_t i = m_EntityPoolSize; i < newPoolSize; ++i) {
		m_EntityIndices[i] = i + 1;
		m_Entities[i].m_GroupIndex = 0;
		m_Entities[i].m_LocalIndex = 0;
		m_Layouts[i].m_State = EntityLayoutState();
		m_Layouts[i].m_RequiredBits = 0;
		m_RemovedEntities[i] = false;
		m_DirtyEntities[i] = false;
	}

	m_EntityPoolSize = newPoolSize;
}
