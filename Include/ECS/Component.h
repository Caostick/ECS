#pragma once

#include <ECS/Common.h>

namespace ecs {
	struct ComponentInfo {
		static ecs::ComponentCounterType s_ComponentCount;
		static ecs::Bitset s_ComponentBitsInfo[ecs::MaxComponentCount];
		static uint32_t s_ComponentAmountInfo[ecs::MaxComponentCount];
		static uint32_t s_ComponentSizeInfo[ecs::MaxComponentCount];
		static const char* s_ComponentNameInfo[ecs::MaxComponentCount];
		static void(*s_ConstructComponentFunc[ecs::MaxComponentCount])(void* comp);
		static void(*s_MoveComponentFunc[ecs::MaxComponentCount])(void* dst, void* src);
		static void(*s_DestructComponentFunc[ecs::MaxComponentCount])(void* comp);
	};

	template<typename T, typename Empty = void>
	struct Component;

	template<typename T>
	struct Component<T, typename std::enable_if<std::is_empty<T>::value>::type> {
	public:
		static ecs::ComponentTypeId s_ComponentTypeId;
		static ecs::Bitset s_ComponentTypeBitmask;
		static uint32_t s_ComponentTypeSize;

		static ecs::ComponentTypeId GetTypeId();
		static ecs::Bitset GetTypeBitmask();
		static uint32_t GetTypeSize();

	private:
		static ecs::ComponentTypeId GetComponentId();
	};

	template<typename T>
	struct Component<T, typename std::enable_if<!std::is_empty<T>::value>::type> {
	public:
		static ecs::ComponentTypeId s_ComponentTypeId;
		static ecs::Bitset s_ComponentTypeBitmask;
		static uint32_t s_ComponentTypeSize;

		static ecs::ComponentTypeId GetTypeId();
		static ecs::Bitset GetTypeBitmask();
		static uint32_t GetTypeSize();

	private:
		static ecs::ComponentTypeId GetComponentId();

		static_assert(sizeof(T) % Alignment == 0, "ecs::Component struct not aligned!");
		static_assert(!std::is_copy_constructible_v<T>, "ecs::Component shouldn't have copy c-tor!");
		static_assert(!std::is_copy_assignable_v<T>, "ecs::Component shouldn't have copy assignment!");
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