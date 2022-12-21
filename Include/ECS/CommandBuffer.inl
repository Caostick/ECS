#pragma once

template<typename T, typename ...TArgs>
auto ecs::CommandBuffer::AttachComponent(EntityHandle entityHandle, TArgs&&... args) -> T& {
	T* compDataPtr = AllocateAndConstructComponent<T>(std::forward<TArgs>(args)...);

	Command command;
	command.m_EntityHandle = entityHandle;
	command.m_CommandType = ECommandType::AttachComponent;
	command.m_ComponentTypeId = Component<T>::GetTypeId();
	command.m_ComponentDataPtr = (uint8_t*)compDataPtr;

	m_Commands.emplace_back(command);

	return *compDataPtr;
}

template <typename T>
void ecs::CommandBuffer::DetachComponent(EntityHandle entityHandle) {
	Command command;
	command.m_EntityHandle = entityHandle;
	command.m_CommandType = ECommandType::DetachComponent;
	command.m_ComponentTypeId = Component<T>::GetTypeId();

	m_Commands.emplace_back(command);
}

template<typename T, typename ...TArgs>
auto ecs::CommandBuffer::AllocateAndConstructComponent(TArgs&&... args) -> T* {
	uint8_t* data = AllocateComponentData(sizeof(T));

	T* component = (T*)data;
	new(component) T(std::forward<TArgs>(args)...);

	return component;
}