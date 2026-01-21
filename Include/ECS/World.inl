#pragma once

#include <ECS/WorldView.h>
#include <ECS/ThreadManager.h>
#include <ECS/Component.h>
#include <ECS/Utils.h>

template<typename T, typename ...TArgs>
void ecs::World::RegisterSystem(TArgs&&... args) {
	using WorldView = typename T::WorldView;
	using TypeList = typename WorldView::TL;

	SystemInfo systemInfo;

	systemInfo.m_System = new T(std::forward<TArgs>(args)...);
	systemInfo.m_UpdateFunction = &UpdateSystem<T>;
	systemInfo.m_ComponentAccessMask = utils::ComponentTypeBits<TypeList>::Bits();
	systemInfo.m_MutableComponentAccessMask = utils::MutableComponentTypeBits<TypeList>::Bits();

	m_SystemInfos.emplace_back(std::move(systemInfo));

	ECSPrintSystemInfo();
}

template<typename T, typename ...TArgs>
auto ecs::World::AttachComponent(EntityHandle entityHandle, TArgs&&... args) -> T& {
	const auto threadIndex = m_ThreadManager->GetThreadIndex();
	auto& tls = m_TLS[threadIndex];
	auto& commandBuffer = tls.GetCommandBuffer();

	return commandBuffer.AttachComponent<std::remove_const_t<T>>(entityHandle, std::forward<TArgs>(args)...);
}

template<typename T>
void ecs::World::DetachComponent(EntityHandle entityHandle) {
	const auto threadIndex = m_ThreadManager->GetThreadIndex();
	auto& tls = m_TLS[threadIndex];
	auto& commandBuffer = tls.GetCommandBuffer();

	commandBuffer.DetachComponent<std::remove_const_t<T>>(entityHandle);
}

template<typename T>
auto ecs::World::GetComponent(EntityHandle entityHandle) -> T& {
	const auto threadIndex = ToThreadIndex(entityHandle);
	const auto entityIndex = ToEntityIndex(entityHandle);
	auto& tls = m_TLS[threadIndex];

	auto& entity = tls.GetEntity(entityIndex);

	auto group = m_Groups[entity.m_GroupIndex];

	return *group->GetComponent<std::remove_const_t<T>>(entity.m_LocalIndex);
}

template<typename T>
bool ecs::World::HasComponent(EntityHandle entityHandle) const {
	const auto threadIndex = ToThreadIndex(entityHandle);
	const auto entityIndex = ToEntityIndex(entityHandle);
	const auto& tls = m_TLS[threadIndex];
	const auto& entity = tls.GetEntity(entityIndex);
	const auto group = m_Groups[entity.m_GroupIndex];
	const auto& groupBits = group->GetTypeBits();
	const auto& componentBits = ecs::Component<std::remove_const_t<T>>::GetTypeBitmask();

	return (groupBits & componentBits) == componentBits;
}

template<typename T>
bool ecs::World::IsAdded(EntityHandle entityHandle) const {
	const auto threadIndex = ToThreadIndex(entityHandle);
	const auto entityIndex = ToEntityIndex(entityHandle);
	auto& tls = m_TLS[threadIndex];

	auto& layout = tls.GetEntityLayout(entityIndex);

	return layout.m_State.m_BitsJustAttached[Component<std::remove_const_t<T>>::GetTypeId()];
}

template<typename T>
bool ecs::World::IsRemoved(EntityHandle entityHandle) const {
	const auto threadIndex = ToThreadIndex(entityHandle);
	const auto entityIndex = ToEntityIndex(entityHandle);
	auto& tls = m_TLS[threadIndex];

	auto& layout = tls.GetEntityLayout(entityIndex);

	return layout.m_State.m_BitsJustDetached[Component<std::remove_const_t<T>>::GetTypeId()];
}

template<typename T>
void ecs::World::UpdateSystem(World& world, System& system, float deltaTime) {
	typename T::WorldView worldView = typename T::WorldView(world);

	T& exactSystem = (T&)system;

	exactSystem.Update(worldView, deltaTime);
}