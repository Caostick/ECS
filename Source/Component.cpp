#include <ECS/Source/Component.h>
#include <ECS/Source/Allocator.h>

ecs::ComponentCounterType ecs::ComponentInfo::s_ComponentCount = 0;
ecs::Bitset ecs::ComponentInfo::s_ComponentBitsInfo[ecs::MaxComponentCount] = {};
uint32_t ecs::ComponentInfo::s_ComponentAmountInfo[ecs::MaxComponentCount] = {};
uint32_t ecs::ComponentInfo::s_ComponentSizeInfo[ecs::MaxComponentCount] = {};
const char* ecs::ComponentInfo::s_ComponentNameInfo[ecs::MaxComponentCount] = {};
void(*ecs::ComponentInfo::s_ConstructComponentFunc[ecs::MaxComponentCount])(void* comp) = {};
void(*ecs::ComponentInfo::s_MoveComponentFunc[ecs::MaxComponentCount])(void* dst, void* src) = {};
void(*ecs::ComponentInfo::s_DestructComponentFunc[ecs::MaxComponentCount])(void* comp) = {};