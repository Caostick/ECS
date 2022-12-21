#pragma once

#include <ECS/Assert.h>


template<typename T>
auto ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetTypeId() -> ComponentTypeId {
	return s_ComponentTypeId;
}

template<typename T>
auto ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetTypeBitmask() -> Bitset {
	return s_ComponentTypeBitmask;
}

template<typename T>
auto ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetTypeSize() -> uint32_t {
	return 0;
}


template<typename T>
auto ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetComponentId() -> ComponentTypeId {
	static ecs::ComponentTypeId id = ecs::ComponentInfo::s_ComponentCount++;
	ECSAssert(id < ecs::MaxComponentCount, "Increase max component count!");

	ecs::ComponentInfo::s_ComponentAmountInfo[id] = 0;
	ecs::ComponentInfo::s_ComponentSizeInfo[id] = 0;
	ecs::ComponentInfo::s_ComponentBitsInfo[id] = ecs::Bitset(1) << id;

	ecs::ComponentInfo::s_ConstructComponentFunc[id] = [](void*) {};
	ecs::ComponentInfo::s_MoveComponentFunc[id] = [](void*, void*) {};
	ecs::ComponentInfo::s_DestructComponentFunc[id] = [](void*) {};

	if (!strncmp("struct", typeid(T).name(), 6)) {
		ecs::ComponentInfo::s_ComponentNameInfo[id] = typeid(T).name() + 7;
	} else if (!strncmp("class", typeid(T).name(), 5)) {
		ecs::ComponentInfo::s_ComponentNameInfo[id] = typeid(T).name() + 6;
	} else {
		ecs::ComponentInfo::s_ComponentNameInfo[id] = "Unknown";
	}

	ECSPrintTypeInfo();
	return id;
}






template<typename T>
auto ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetTypeId() -> ComponentTypeId {
	return s_ComponentTypeId;
}

template<typename T>
auto ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetTypeBitmask() -> Bitset {
	return s_ComponentTypeBitmask;
}

template<typename T>
auto ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetTypeSize() -> uint32_t {
	return s_ComponentTypeSize;
}


template<typename T>
auto ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetComponentId() -> ComponentTypeId {
	static ComponentTypeId id = ComponentInfo::s_ComponentCount++;
	ECSAssert(id < MaxComponentCount, "Increase max component count!");

	ComponentInfo::s_ComponentAmountInfo[id] = 0;
	ComponentInfo::s_ComponentSizeInfo[id] = sizeof(T);
	ComponentInfo::s_ComponentBitsInfo[id] = Bitset(1) << id;

	ComponentInfo::s_ConstructComponentFunc[id] = []([[maybe_unused]] void* comp) {
		if constexpr (std::is_constructible_v<T>) {
			new (reinterpret_cast<T*>(comp)) T();
		}
	};

	ComponentInfo::s_MoveComponentFunc[id] = [](void* dst, void* src) {
		if constexpr (std::is_constructible_v<T>) {
			T* srcComp = reinterpret_cast<T*>(src);
			T* dstComp = reinterpret_cast<T*>(dst);

			*dstComp = std::move(*srcComp);
		} else if constexpr (std::is_move_constructible_v<T>) {
			T* srcComp = reinterpret_cast<T*>(src);
			T* dstComp = reinterpret_cast<T*>(dst);

			new (dstComp) T(std::move(*srcComp));
		}

		constexpr bool isValidType = std::is_constructible_v<T> || std::is_move_constructible_v<T>;
		static_assert(isValidType, "No appropriate constructor for component!");
	};

	ComponentInfo::s_DestructComponentFunc[id] = [](void* comp) {
		reinterpret_cast<T*>(comp)->~T();
	};

	if (!strncmp("struct", typeid(T).name(), 6)) {
		ComponentInfo::s_ComponentNameInfo[id] = typeid(T).name() + 7;
	} else if (!strncmp("class", typeid(T).name(), 5)) {
		ComponentInfo::s_ComponentNameInfo[id] = typeid(T).name() + 6;
	} else {
		ComponentInfo::s_ComponentNameInfo[id] = "Unknown";
	}

	ECSPrintTypeInfo();
	return id;
}

