#pragma once

#include <stdint.h>

#include <ECS/EntityHandle.h>
#include <ECS/Bitset.h>
#include <ECS/Vector.h>
#include <ECS/Lock.h>
#include <ECS/TypeList.h>

#if _DEBUG
#include <stdio.h>
#include <iostream>

#define ECSPrintSystemInfo() { \
    static bool registered = false; \
    if(!registered) { \
        printf("Registered system: <%s>\n", typeid(T).name()); \
        registered = true; \
    } \
}

#define ECSPrintTypeInfo() { \
    static bool registered = false; \
    if(!registered) { \
        printf("Registered component type: <%s, %d, %d bytes>\n", typeid(T).name(), id, ecs::ComponentInfo::s_ComponentSizeInfo[id]); \
        registered = true; \
    } \
}

#else
#define ECSPrintSystemInfo() void()
#define ECSPrintTypeInfo() void()
#endif // _DEBUG

namespace ecs {
	// ecs::EntityHandle : [ThreadIndex, EntityIndex]
	// ecs::Entity : [GroupIndex, LocalIndex]

	constexpr uint32_t MaxComponentCount = 64;
	constexpr uint32_t InitialEntityCount = 64;
	constexpr uint32_t EntitiesPerGroupDataPage = 16;

	using EntityGroupId = uint16_t;
	using ComponentTypeId = uint16_t;
	using QueryTypeId = uint32_t;
	using ComponentCounterType = uint16_t;
	using QueryCounterType = uint16_t;
	using Bitset = TBitset<MaxComponentCount>;

	constexpr uint32_t CommandBufferPageSize = 1024 * 1024;
	constexpr uint32_t ThreadIndexBits = 6;
	constexpr EntityHandle UnassignedEntityHandle = EntityHandle(-1);
	constexpr EntityGroupId UnassignedEntityGroupId = EntityGroupId(-1);
	constexpr int Alignment = 4;
	constexpr Bitset EmptyEntityLayout = 0;

	auto ToThreadIndex(EntityHandle handle) -> uint32_t;
	auto ToEntityIndex(EntityHandle handle) -> uint32_t;
	auto ToEntityHandle(uint32_t threadIndex, uint32_t entityIndex) -> EntityHandle;

	struct EntityLayoutState {
		EntityLayoutState() = default;
		EntityLayoutState(const EntityLayoutState&) = default;
		EntityLayoutState(EntityLayoutState&&) = default;
		EntityLayoutState& operator = (const EntityLayoutState&) = default;
		EntityLayoutState& operator = (EntityLayoutState&&) = default;
		bool operator == (const EntityLayoutState& other) const;
		bool operator != (const EntityLayoutState& other) const;

		Bitset m_Bits;
		Bitset m_BitsJustAttached;
		Bitset m_BitsJustDetached;
	};
}