#pragma once

#include <ECS/Assert.h>


template<typename T>
ecs::ComponentTypeId ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetTypeId() {
	return s_ComponentTypeId;
}

template<typename T>
ecs::Bitset ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetTypeBitmask() {
	return s_ComponentTypeBitmask;
}

template<typename T>
uint32_t ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetTypeSize() {
	return 0;
}


template<typename T>
ecs::ComponentTypeId ecs::Component<T, typename std::enable_if<std::is_empty<T>::value>::type>::GetComponentId() {
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
ecs::ComponentTypeId ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetTypeId() {
	return s_ComponentTypeId;
}

template<typename T>
ecs::Bitset ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetTypeBitmask() {
	return s_ComponentTypeBitmask;
}

template<typename T>
uint32_t ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetTypeSize() {
	return s_ComponentTypeSize;
}


template<typename T>
ecs::ComponentTypeId ecs::Component<T, typename std::enable_if<!std::is_empty<T>::value>::type>::GetComponentId() {
	static ecs::ComponentTypeId id = ecs::ComponentInfo::s_ComponentCount++;
	ECSAssert(id < ecs::MaxComponentCount, "Increase max component count!");

	ecs::ComponentInfo::s_ComponentAmountInfo[id] = 0;
	ecs::ComponentInfo::s_ComponentSizeInfo[id] = sizeof(T);
	ecs::ComponentInfo::s_ComponentBitsInfo[id] = ecs::Bitset(1) << id;

	ecs::ComponentInfo::s_ConstructComponentFunc[id] = []([[maybe_unused]] void* comp) {
		if constexpr (std::is_constructible_v<T>) {
			new (reinterpret_cast<T*>(comp)) T();
		}
	};

	ecs::ComponentInfo::s_MoveComponentFunc[id] = [](void* dst, void* src) {
		if constexpr (std::is_constructible_v<T>) {
			T* srcComp = reinterpret_cast<T*>(src);
			T* dstComp = reinterpret_cast<T*>(dst);

			*dstComp = std::move(*srcComp);
		} else if constexpr (std::is_move_constructible_v<T>) {
			T* srcComp = reinterpret_cast<T*>(src);
			T* dstComp = reinterpret_cast<T*>(dst);

			new (dstComp) T(std::move(*srcComp));
		} else {
			static_assert(false, "No appropriate constructor for component!");
		}
	};

	ecs::ComponentInfo::s_DestructComponentFunc[id] = [](void* comp) {
		reinterpret_cast<T*>(comp)->~T();
	};

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

