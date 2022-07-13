#pragma once

template<typename T>
T* ecs::Group::GetComponent(uint32_t localIndex) {
	const auto pageIndex = localIndex / EntitiesPerGroupDataPage;
	const auto pageLocalIndex = localIndex % EntitiesPerGroupDataPage;

	const uint32_t entityOffset = pageLocalIndex * m_EntitySize;
	const uint32_t componentOffset = m_ComponentOffsets[ecs::Component<T>::GetTypeId()];

	return reinterpret_cast<T*>(m_DataPages[pageIndex] + entityOffset + componentOffset);
}