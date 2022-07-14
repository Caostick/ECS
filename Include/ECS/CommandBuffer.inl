#pragma once

template<typename T, typename ...TArgs>
T& ecs::CommandBuffer::AttachComponent(ecs::EntityHandle entityHandle, TArgs&&... args) {
	T* compDataPtr = AllocateAndConstructComponent<T>(std::forward<TArgs>(args)...);

	ecs::Command command;
	command.m_EntityHandle = entityHandle;
	command.m_CommandType = ecs::ECommandType::AttachComponent;
	command.m_ComponentTypeId = ecs::Component<T>::GetTypeId();
	command.m_ComponentDataPtr = (uint8_t*)compDataPtr;

	m_Commands.emplace_back(command);

	return *compDataPtr;
}

template <typename T>
void ecs::CommandBuffer::DetachComponent(ecs::EntityHandle entityHandle) {
	ecs::Command command;
	command.m_EntityHandle = entityHandle;
	command.m_CommandType = ecs::ECommandType::DetachComponent;
	command.m_ComponentTypeId = ecs::Component<T>::GetTypeId();

	m_Commands.emplace_back(command);
}

template<typename T, typename ...TArgs>
T* ecs::CommandBuffer::AllocateAndConstructComponent(TArgs&&... args) {
	uint8_t* data = AllocateComponentData(sizeof(T));

	T* component = (T*)data;
	new(component) T(std::forward<TArgs>(args)...);

	return component;
}