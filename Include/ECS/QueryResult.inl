#pragma once

#include <ECS/Group.h>

template<typename TL, typename WVTL>
ecs::QueryResult<TL, WVTL>::QueryResult(Group* group, uint32_t entityLocalIndex)
	: m_Group(group)
	, m_EntityLocalIndex(entityLocalIndex) {}

template<typename TL, typename WVTL>
template<typename T> requires (ecs::Contains<const T, TL>::value) && (not std::is_const_v<T>) && (not std::is_empty<T>::value)
auto ecs::QueryResult<TL, WVTL>::Get() -> typename std::conditional_t<ecs::Contains<std::remove_const_t<T>, WVTL>::value, T&, const T&> {
	static_assert(Contains<const T, TL>::value, "Component of type is inaccessible!");
	static_assert(!std::is_const_v<T>, "Modifier const is deprecated!");
	static_assert(!std::is_empty<T>::value, "Can't access empty component!");

	return *m_Group->GetComponent<std::remove_const_t<T>>(m_EntityLocalIndex);
}

template<typename TL, typename WVTL>
template<typename T>
bool ecs::QueryResult<TL, WVTL>::Has() const {
	const auto& groupBits = m_Group->GetTypeBits();

	return groupBits.test(ecs::Component<std::remove_const_t<T>>::GetTypeId());
}

template<typename TL, typename WVTL>
template<typename T> requires (ecs::Contains<const T, WVTL>::value) && (not std::is_const_v<T>) && (not std::is_empty<T>::value)
auto ecs::QueryResult<TL, WVTL>::GetOptional() -> typename std::conditional_t<ecs::Contains<std::remove_const_t<T>, WVTL>::value, T*, const T*> {
	static_assert(Contains<const T, WVTL>::value, "Component of type is inaccessible!");
	static_assert(!std::is_const_v<T>, "Modifier const is deprecated!");
	static_assert(!std::is_empty<T>::value, "Can't access empty component!");

	const auto& groupBits = m_Group->GetTypeBits();
	const bool has = groupBits.test(ecs::Component<std::remove_const_t<T>>::GetTypeId());

	return has ? m_Group->GetComponent<std::remove_const_t<T>>(m_EntityLocalIndex) : nullptr;
}

template<typename TL, typename WVTL>
ecs::QueryResult<TL, WVTL>::operator ecs::EntityHandle () const {
	return m_Group->GetEntityBackReference(m_EntityLocalIndex);
}