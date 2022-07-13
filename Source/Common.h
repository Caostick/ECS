#pragma once

#include <stdint.h>

#include <ECS/Include/ECS/EntityHandle.h>
#include <ECS/Source/Bitset.h>
#include <ECS/Source/Vector.h>
#include <ECS/Source/TypeList.h>

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
	using ComponentCounterType = uint16_t;
	using Bitset = df::Bitset<ecs::MaxComponentCount>;

	constexpr uint32_t CommandBufferPageSize = 1024 * 1024;
	constexpr uint32_t ThreadIndexBits = 6;
	constexpr ecs::EntityHandle UnassignedEntityHandle = ecs::EntityHandle(-1);
	constexpr ecs::EntityGroupId UnassignedEntityGroupId = ecs::EntityGroupId(-1);
	constexpr int Alignment = 4;
	constexpr Bitset EmptyEntityLayout = 0;

	uint32_t ToThreadIndex(ecs::EntityHandle handle);
	uint32_t ToEntityIndex(ecs::EntityHandle handle);
	ecs::EntityHandle ToEntityHandle(uint32_t threadIndex, uint32_t entityIndex);

	struct EntityLayoutState {
		EntityLayoutState() = default;
		EntityLayoutState(const EntityLayoutState&) = default;
		EntityLayoutState(EntityLayoutState&&) = default;
		EntityLayoutState& operator = (const EntityLayoutState&) = default;
		EntityLayoutState& operator = (EntityLayoutState&&) = default;
		bool operator == (const EntityLayoutState& other) const;
		bool operator != (const EntityLayoutState& other) const;

		ecs::Bitset m_Bits;
		ecs::Bitset m_BitsJustAttached;
		ecs::Bitset m_BitsJustDetached;
	};
}