#include <ECS/World.h>
#include <ECS/System.h>
#include <ECS/ThreadManager.h>
#include <ECS/Query.h>

bool ecs::SystemInfo::IsParallelCompatible(const SystemInfo& other) const {
	for (size_t i = 0; i < MaxComponentCount; ++i) {
		if (m_MutableComponentAccessMask[i] && other.m_ComponentAccessMask[i] ||
			other.m_MutableComponentAccessMask[i] && m_ComponentAccessMask[i]) {
			return false;
		}
	}

	return true;
}

void ecs::World::Init(const ThreadManager* threadManager /*= nullptr*/) {
	if (!threadManager) {
		m_DefaultThreadManager = new ThreadManager;
	}

	m_ThreadManager = m_DefaultThreadManager;

	m_TLS.resize(m_ThreadManager->GetThreadCount());

	m_Queries.resize(QueryRegistry::s_QueryCount);

	m_Groups.push_back(new Group(0, 0));
}

void ecs::World::Release() {
	for (SystemInfo& systemInfo : m_SystemInfos) {
		delete systemInfo.m_System;
	}
	m_SystemInfos.clear();

	for (Group* group : m_Groups) {
		delete group;
	}
	m_Groups.clear();

	delete m_DefaultThreadManager;
	m_DefaultThreadManager = nullptr;
	m_ThreadManager = nullptr;
}

auto ecs::World::CreateEntity() -> EntityHandle {
	const auto threadIndex = m_ThreadManager->GetThreadIndex();
	auto& tls = m_TLS[m_ThreadManager->GetThreadIndex()];
	return ToEntityHandle(threadIndex, tls.CreateEntity());
}

void ecs::World::DestroyEntity(EntityHandle entityHandle) {
	auto& tls = m_TLS[m_ThreadManager->GetThreadIndex()];
	auto& commandBuffer = tls.GetCommandBuffer();

	commandBuffer.DestroyEntity(entityHandle);
}

void ecs::World::Update(float dt) {
	for (SystemInfo& systemInfo : m_SystemInfos) {
		systemInfo.m_UpdateFunction(*this, *systemInfo.m_System, dt);
	}
}

void ecs::World::ExecuteCommands() {
	// 0: refresh added/removed entity data
	for (const auto& entityHandle : m_EntitiesToRefresh) {
		const auto threadIdx = ToThreadIndex(entityHandle);
		const auto entityIdx = ToEntityIndex(entityHandle);
		auto& tls = m_TLS[threadIdx];
		auto& layout = tls.GetEntityLayout(entityIdx);

		layout.m_State.m_BitsJustAttached = 0;

		if (layout.m_State.m_BitsJustDetached != 0) {
			layout.m_RequiredBits &= ~layout.m_State.m_BitsJustDetached;
			layout.m_State.m_BitsJustDetached = 0;

			m_EntitiesToChangeLayout.push_back(entityHandle);
		}

		if (tls.IsMarkedForDestroy(entityIdx)) {
			m_EntitiesToDestroy.push_back(entityHandle);
		}

		tls.MarkForUpdate(entityIdx, false);
	}
	m_EntitiesToRefresh.clear();

	// 1: go through all component requests and construct new layouts, write if entity remove requested
	//uint32_t idx = 0;
	for (auto& tls : m_TLS) {
		auto& commandBuffer = tls.GetCommandBuffer();
		if (!commandBuffer.IsEmpty()) {
			//std::cout << "Buffer [" << idx++ << "] commands: \n";
			for (const auto cmd : commandBuffer) {

				const auto entityHandle = cmd.m_EntityHandle;
				const auto threadIdx = ToThreadIndex(entityHandle);
				const auto entityIdx = ToEntityIndex(entityHandle);
				const auto componentTypeId = cmd.m_ComponentTypeId;

				switch (cmd.m_CommandType) {
				case ECommandType::DestroyEntity:
					//std::cout << "Destroy entity [" << entityHandle << "]\n";

					if (!m_TLS[threadIdx].IsMarkedForDestroy(entityIdx)) {
						m_TLS[threadIdx].MarkForDestroy(entityIdx, true);
						m_TLS[threadIdx].RequestAllComponentsDetach(entityIdx);

						if (!m_TLS[threadIdx].IsMarkedForUpdate(entityIdx)) {
							m_TLS[threadIdx].MarkForUpdate(entityIdx, true);
							m_EntitiesToRefresh.push_back(entityHandle);
						}
					}

					break;
				case ECommandType::AttachComponent:
					//std::cout << "Attach component [" << ComponentInfo::s_ComponentNameInfo[componentTypeId] << "] to entity [" << entityHandle << "]\n";

					if (!m_TLS[threadIdx].IsMarkedForDestroy(entityIdx)) {
						m_TLS[threadIdx].RequestComponentAttach(entityIdx, componentTypeId);
						m_EntitiesToChangeLayout.push_back(entityHandle);
					}

					break;
				case ECommandType::DetachComponent:
					//std::cout << "Detach component [" << ComponentInfo::s_ComponentNameInfo[componentTypeId] << "] from entity [" << entityHandle << "]\n";
					if (!m_TLS[threadIdx].IsMarkedForDestroy(entityIdx)) {
						m_TLS[threadIdx].RequestComponentDetach(entityIdx, componentTypeId);

						if (!m_TLS[threadIdx].IsMarkedForUpdate(entityIdx)) {
							m_TLS[threadIdx].MarkForUpdate(entityIdx, true);
							m_EntitiesToRefresh.push_back(entityHandle);
						}
						//m_EntitiesToChangeLayout.push_back(entityHandle);
					}
					break;
				default:
					break;
				}
			}
			//std::cout << "\n";
		}
	}

	// 2: execute layout changing(if it hasn't happened - current and required not the same), (if entity not marked for deletion)
	for (const auto& entityHandle : m_EntitiesToChangeLayout) {
		const auto threadIdx = ToThreadIndex(entityHandle);
		const auto entityIdx = ToEntityIndex(entityHandle);
		auto& tls = m_TLS[threadIdx];

		if (!tls.IsMarkedForDestroy(entityIdx)) {
			auto& layout = tls.GetEntityLayout(entityIdx);

			if (layout.m_State.m_Bits != layout.m_RequiredBits) {
				ExecuteChangeEntityLayout(entityHandle);
			}
		}
	}

	// 3: move components(if entity not marked for deletion)
	for (auto& tls : m_TLS) {
		auto& commandBuffer = tls.GetCommandBuffer();
		if (!commandBuffer.IsEmpty()) {
			for (const auto cmd : commandBuffer) {

				if (cmd.m_CommandType == ECommandType::AttachComponent) {

					const auto entityHandle = cmd.m_EntityHandle;
					const auto threadIdx = ToThreadIndex(entityHandle);
					const auto entityIdx = ToEntityIndex(entityHandle);
					const auto componentTypeId = cmd.m_ComponentTypeId;

					const auto& layout = m_TLS[threadIdx].GetEntityLayout(entityIdx);
					if (layout.m_State.m_Bits[componentTypeId]) {
						const auto groupIndex = m_TLS[threadIdx].GetEntityGroupIndex(entityIdx);
						auto group = m_Groups[groupIndex];
						auto& entity = m_TLS[threadIdx].GetEntity(entityIdx);

						uint8_t* compPtr = group->GetComponentData(entity.m_LocalIndex, componentTypeId);

						ComponentInfo::s_MoveComponentFunc[componentTypeId](compPtr, cmd.m_ComponentDataPtr);
						ComponentInfo::s_DestructComponentFunc[componentTypeId](cmd.m_ComponentDataPtr);
					}
				}
			}
		}
	}

	// 4: remove entities
	for (const auto& entityHandle : m_EntitiesToDestroy) {
		const auto threadIdx = ToThreadIndex(entityHandle);
		const auto entityIdx = ToEntityIndex(entityHandle);

		auto& tls = m_TLS[threadIdx];
		auto& layout = tls.GetEntityLayout(entityIdx);
		layout.m_RequiredBits = 0;
		ExecuteChangeEntityLayout(entityHandle);

		tls.DestroyEntity(entityIdx);
		tls.MarkForDestroy(entityIdx, false);
	}

	// 5: clear command buffers
	for (auto& tls : m_TLS) {
		tls.ResetCommands();
	}

	m_EntitiesToDestroy.clear();
	m_EntitiesToChangeLayout.clear();
}

void ecs::World::FinishAllCommands() {
	constexpr float fixedDeltaTime = 1.0f / 60.0f;

	bool finished = false;
	while (!finished) {
		finished = true;

		for (auto& tls : m_TLS) {
			auto& commandBuffer = tls.GetCommandBuffer();
			if (!commandBuffer.IsEmpty()) {
				finished = false;
			}
		}

		Update(fixedDeltaTime);
		ExecuteCommands();
	}
}

auto ecs::World::GetGroupEntityCount(const Bitset& typeBitmask) const -> uint32_t {
	for (auto group : m_Groups) {
		if (group->GetTypeBits() == typeBitmask) {
			return group->GetEntityCount();
		}
	}

	return 0;
}

auto ecs::World::GetEntityBackreference(EntityHandle entityHandle) const -> EntityHandle {
	const auto threadIdx = ToThreadIndex(entityHandle);
	const auto entityIdx = ToEntityIndex(entityHandle);

	const auto& tls = m_TLS[threadIdx];
	const auto& entity = tls.GetEntity(entityIdx);

	return entity.m_GroupIndex == 0 ? entityHandle : m_Groups[entity.m_GroupIndex]->GetEntityBackReference(entity.m_LocalIndex);
}

auto ecs::World::GetQueryCache(uint32_t id) -> QueryCache& {
	ECSAssert(id < m_Queries.size(), "QueryCache index is out of range!");
	return m_Queries[id];
}

void ecs::World::InitQueryCache(QueryCache& queryCache) {
	for (auto group : m_Groups) {
		TryCacheGroup(queryCache, group);
	}
}

void ecs::World::TryCacheGroup(QueryCache& queryCache, Group* group) {
	const auto& groupBits = group->GetTypeBits();
	const bool includeFits = ((groupBits & queryCache.m_IncludeBits) == queryCache.m_IncludeBits);
	const bool excludeFits = ((~groupBits & queryCache.m_ExcludeBits) == queryCache.m_ExcludeBits);

	if (!(includeFits && excludeFits)) {
		return;
	}

	queryCache.m_Groups.push_back(group);
}

void ecs::World::ExecuteChangeEntityLayout(EntityHandle entityHandle) {
	const auto threadIdx = ToThreadIndex(entityHandle);
	const auto entityIdx = ToEntityIndex(entityHandle);

	auto& tls = m_TLS[threadIdx];
	auto& layout = tls.GetEntityLayout(entityIdx);

	if (layout.m_State.m_Bits == EmptyEntityLayout) {
		// If OLD layout is empty

		auto group = GetGroup(layout.m_RequiredBits);
		const auto localIndex = group->AddEntity(entityHandle);

		auto& entity = tls.GetEntity(entityIdx);
		entity.m_GroupIndex = group->GetIndex();
		entity.m_LocalIndex = localIndex;

		ECSAssert(entityHandle == GetEntityBackreference(entityHandle), "Invalid entity backreference");

		// Construct new components
		const Bitset addedLayout = layout.m_RequiredBits;
		for (uint32_t i = 0; i < MaxComponentCount; ++i) {
			if (addedLayout[i]) {
				uint8_t* newCompPtr = group->GetComponentData(localIndex, ComponentTypeId(i));
				ComponentInfo::s_ConstructComponentFunc[ComponentTypeId(i)](newCompPtr);
			}
		}

	} else if (layout.m_RequiredBits == EmptyEntityLayout) {
		// If NEW layout is empty

		const auto groupIndex = tls.GetEntityGroupIndex(entityIdx);
		const auto localIndex = tls.GetEntityLocalIndex(entityIdx);

		auto group = m_Groups[groupIndex];

		auto& entity = tls.GetEntity(entityIdx);

		// Fix entity what has been changed within group
		ECSAssert(entityHandle == GetEntityBackreference(entityHandle), "Invalid entity backreference");

		// Destruct removed components
		const Bitset removedLayout = layout.m_State.m_Bits;
		for (uint32_t i = 0; i < MaxComponentCount; ++i) {
			if (removedLayout[i]) {
				uint8_t* compPtr = group->GetComponentData(entity.m_LocalIndex, ComponentTypeId(i));
				ComponentInfo::s_DestructComponentFunc[ComponentTypeId(i)](compPtr);
			}
		}

		const auto entityHandleToFix = group->RemoveEntity(localIndex);
		if (entityHandle != entityHandleToFix) {
			const auto threadIdxToFix = ToThreadIndex(entityHandleToFix);
			const auto entityIdxToFix = ToEntityIndex(entityHandleToFix);
			auto& entityToFix = m_TLS[threadIdxToFix].GetEntity(entityIdxToFix);
			entityToFix.m_LocalIndex = localIndex;
		}

		entity.m_GroupIndex = 0;
		entity.m_LocalIndex = 0;

		ECSAssert(entityHandle == GetEntityBackreference(entityHandle), "Invalid entity backreference");
		ECSAssert(entityHandleToFix == GetEntityBackreference(entityHandleToFix), "Invalid entity backreference");

	} else {
		// If both layouts exist

		// Get current group
		auto& entity = tls.GetEntity(entityIdx);
		auto group = m_Groups[entity.m_GroupIndex];

		// Create new group
		auto newGroup = GetGroup(layout.m_RequiredBits);
		const auto newLocalEntityIndex = newGroup->AddEntity(entityHandle);

		ECSAssert(entityHandle == GetEntityBackreference(entityHandle), "Invalid entity backreference");
		
		const Bitset mergedLayout = layout.m_RequiredBits & layout.m_State.m_Bits;
		const Bitset newLayout = layout.m_RequiredBits;
		const Bitset oldLayout = layout.m_State.m_Bits;

		// Construct new components
		for (uint32_t i = 0; i < MaxComponentCount; ++i) {
			if (newLayout[i]) {
				uint8_t* newCompPtr = newGroup->GetComponentData(newLocalEntityIndex, ComponentTypeId(i));
				ComponentInfo::s_ConstructComponentFunc[ComponentTypeId(i)](newCompPtr);
			}
		}

		// Move components from current group to new one
		for (uint32_t i = 0; i < MaxComponentCount; ++i) {
			if (mergedLayout[i]) {
				uint8_t* compPtr = group->GetComponentData(entity.m_LocalIndex, ComponentTypeId(i));
				uint8_t* newCompPtr = newGroup->GetComponentData(newLocalEntityIndex, ComponentTypeId(i));

				ComponentInfo::s_MoveComponentFunc[ComponentTypeId(i)](newCompPtr, compPtr);
			}
		}

		// Destruct removed components
		for (uint32_t i = 0; i < MaxComponentCount; ++i) {
			if (oldLayout[i]) {
				uint8_t* compPtr = group->GetComponentData(entity.m_LocalIndex, ComponentTypeId(i));
				ComponentInfo::s_DestructComponentFunc[ComponentTypeId(i)](compPtr);
			}
		}

		// @TODO: Make backreference check for each entity and store last operation

		// Fix entity what has been changed within group
		ECSAssert(entityHandle == GetEntityBackreference(entityHandle), "Invalid entity backreference");

		const auto entityHandleToFix = group->RemoveEntity(entity.m_LocalIndex);
		if (entityHandle != entityHandleToFix) {
			const auto threadIdxToFix = ToThreadIndex(entityHandleToFix);
			const auto entityIdxToFix = ToEntityIndex(entityHandleToFix);
			auto& entityToFix = m_TLS[threadIdxToFix].GetEntity(entityIdxToFix);
			entityToFix.m_LocalIndex = entity.m_LocalIndex;
		}

		entity.m_GroupIndex = newGroup->GetIndex();
		entity.m_LocalIndex = newLocalEntityIndex;

		ECSAssert(entityHandle == GetEntityBackreference(entityHandle), "Invalid entity backreference");
		ECSAssert(entityHandleToFix == GetEntityBackreference(entityHandleToFix), "Invalid entity backreference");
	}

	if (!m_TLS[threadIdx].IsMarkedForUpdate(entityIdx)) {
		m_TLS[threadIdx].MarkForUpdate(entityIdx, true);
		m_EntitiesToRefresh.push_back(entityHandle);
	}

	layout.m_State.m_Bits = layout.m_RequiredBits;
}

auto ecs::World::GetGroup(const Bitset& typeBitmask) -> Group* {
	for (auto group : m_Groups) {
		if (group->GetTypeBits() == typeBitmask) {
			return group;
		}
	}

	const uint32_t newGroupIndex = static_cast<uint32_t>(m_Groups.size());

	Group* group = new Group(typeBitmask, newGroupIndex);
	m_Groups.push_back(group);

	for(auto& queryCache : m_Queries) {
		if (!queryCache.m_HasLayout) {
			continue;
		}
		TryCacheGroup(queryCache, group);
	}

	return group;
}
