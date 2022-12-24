#pragma once

#include <ECS/Group.h>

template<typename TL, typename WVTL>
ecs::QueryResult<TL, WVTL>::QueryResult(Group* group, uint32_t entityLocalIndex)
	: m_Group(group)
	, m_EntityLocalIndex(entityLocalIndex) {}

template<typename TL, typename WVTL>
template<typename T>
typename std::conditional_t<ecs::Contains<std::remove_const_t<T>, WVTL>::value, T&, const T&>
ecs::QueryResult<TL, WVTL>::Get() {
	static_assert(Contains<const T, TL>::value, "Component of type is inaccessible!");
	static_assert(!std::is_const_v<T>, "Modifier const is deprecated!");
	static_assert(!std::is_empty<T>::value, "Can't access empty component!");

	return *m_Group->GetComponent<std::remove_const_t<T>>(m_EntityLocalIndex);
}

template<typename TL, typename WVTL>
ecs::QueryResult<TL, WVTL>::operator ecs::EntityHandle () const {
	return m_Group->GetEntityBackReference(m_EntityLocalIndex);
}