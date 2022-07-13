#pragma once

#include <ECS/Include/ECS/WorldView.h>
#include <ECS/Include/ECS/ThreadManager.h>
#include <ECS/Source/Allocator.h>

template<typename TEntityType>
auto ecs::World::Construct()->EntityViewConstructor<TEntityType, typename TEntityType::TypeList> {
	return EntityViewConstructor<TEntityType, typename TEntityType::TypeList>(*this, CreateEntity());
}

template<typename T, typename ...TArgs>
void ecs::World::RegisterSystem(TArgs&&... args) {
	ecs::SystemInfo systemInfo;

	systemInfo.m_System = ECSNew T(std::forward<TArgs>(args)...);
	systemInfo.m_UpdateFunction = &UpdateSystem<T>;
	m_SystemInfos.emplace_back(systemInfo);

	ECSPrintSystemInfo();
}

template<typename T, typename ...TArgs>
auto ecs::World::AttachComponent(ecs::EntityHandle entityHandle, TArgs&&... args) -> T& {
	const auto entityThreadIndex = ToThreadIndex(entityHandle);
	auto& tls = m_TLS[m_ThreadManager->GetThreadIndex()];
	auto& commandBuffer = tls.GetCommandBuffer();

	return commandBuffer.AttachComponent<std::remove_const_t<T>>(entityHandle, std::forward<TArgs>(args)...);
}

template<typename T>
void ecs::World::DetachComponent(ecs::EntityHandle entityHandle) {
	const auto entityThreadIndex = ToThreadIndex(entityHandle);
	auto& tls = m_TLS[m_ThreadManager->GetThreadIndex()];
	auto& commandBuffer = tls.GetCommandBuffer();

	commandBuffer.DetachComponent<std::remove_const_t<T>>(entityHandle);
}

template<typename T>
T& ecs::World::GetComponent(ecs::EntityHandle entityHandle) {
	const auto threadIndex = ToThreadIndex(entityHandle);
	const auto entityIndex = ToEntityIndex(entityHandle);
	auto& tls = m_TLS[threadIndex];

	auto& entity = tls.GetEntity(entityIndex);

	auto group = m_Groups[entity.m_GroupIndex];

	return *group->GetComponent<std::remove_const_t<T>>(entity.m_LocalIndex);
}

template<typename T>
bool ecs::World::IsAdded(ecs::EntityHandle entityHandle) const {
	const auto threadIndex = ToThreadIndex(entityHandle);
	const auto entityIndex = ToEntityIndex(entityHandle);
	auto& tls = m_TLS[threadIndex];

	auto& layout = tls.GetEntityLayout(entityIndex);

	return layout.m_State.m_BitsJustAttached[ecs::Component<std::remove_const_t<T>>::GetTypeId()];
}

template<typename T>
bool ecs::World::IsRemoved(ecs::EntityHandle entityHandle) const {
	const auto threadIndex = ToThreadIndex(entityHandle);
	const auto entityIndex = ToEntityIndex(entityHandle);
	auto& tls = m_TLS[threadIndex];

	auto& layout = tls.GetEntityLayout(entityIndex);

	return layout.m_State.m_BitsJustDetached[ecs::Component<std::remove_const_t<T>>::GetTypeId()];
}

template<typename T>
void ecs::World::UpdateSystem(ecs::World& world, ecs::System& system, float deltaTime) {
	T::WorldView worldView = T::WorldView(world);

	T& exactSystem = (T&)system;

	exactSystem.Update(worldView, deltaTime);
}