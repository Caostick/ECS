#pragma once

#include <ECS/Common.h>

namespace ecs {
	struct ComponentInfo {
		static ComponentCounterType s_ComponentCount;
		static Bitset s_ComponentBitsInfo[MaxComponentCount];
		static uint32_t s_ComponentAmountInfo[MaxComponentCount];
		static uint32_t s_ComponentSizeInfo[MaxComponentCount];
		static const char* s_ComponentNameInfo[MaxComponentCount];
		static void(*s_ConstructComponentFunc[MaxComponentCount])(void* comp);
		static void(*s_MoveComponentFunc[MaxComponentCount])(void* dst, void* src);
		static void(*s_DestructComponentFunc[MaxComponentCount])(void* comp);
	};

	template<typename T, typename Empty = void>
	struct Component;

	template<typename T>
	struct Component<T, typename std::enable_if_t<std::is_empty<T>::value>> {
	public:
		static ComponentTypeId s_ComponentTypeId;
		static Bitset s_ComponentTypeBitmask;
		static uint32_t s_ComponentTypeSize;

		static auto GetTypeId() -> ComponentTypeId;
		static auto GetTypeBitmask() -> Bitset;
		static auto GetTypeSize() -> uint32_t;

	private:
		static auto GetComponentId() -> ComponentTypeId;
	};

	template<typename T>
	struct Component<T, typename std::enable_if_t<!std::is_empty<T>::value>> {
	public:
		static ComponentTypeId s_ComponentTypeId;
		static Bitset s_ComponentTypeBitmask;
		static uint32_t s_ComponentTypeSize;

		static auto GetTypeId() -> ComponentTypeId;
		static auto GetTypeBitmask() -> Bitset;
		static auto GetTypeSize() -> uint32_t;

	private:
		static auto GetComponentId() -> ComponentTypeId;

		static_assert(sizeof(T) % Alignment == 0, "Component struct not aligned!");
		static_assert(!std::is_copy_constructible_v<T>, "Component shouldn't have copy c-tor!");
		static_assert(!std::is_copy_assignable_v<T>, "Component shouldn't have copy assignment!");
	};


}


template<typename T>
ecs::ComponentTypeId ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::s_ComponentTypeId = ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetComponentId();

template<typename T>
ecs::Bitset ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::s_ComponentTypeBitmask = ecs::ComponentInfo::s_ComponentBitsInfo[ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetComponentId()];

template<typename T>
uint32_t ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::s_ComponentTypeSize = ecs::ComponentInfo::s_ComponentSizeInfo[ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetComponentId()];


template<typename T>
ecs::ComponentTypeId ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::s_ComponentTypeId = ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetComponentId();

template<typename T>
ecs::Bitset ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::s_ComponentTypeBitmask = ecs::ComponentInfo::s_ComponentBitsInfo[ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetComponentId()];

template<typename T>
uint32_t ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::s_ComponentTypeSize = ecs::ComponentInfo::s_ComponentSizeInfo[ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetComponentId()];

#include <ECS/Component.inl>