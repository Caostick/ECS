#include <ECS/Common.h>

auto ecs::ToThreadIndex(EntityHandle handle) -> uint32_t {
	return handle & ((1 << ThreadIndexBits) - 1);
}

auto ecs::ToEntityIndex(EntityHandle handle) -> uint32_t {
	return handle >> ThreadIndexBits;
}

auto ecs::ToEntityHandle(uint32_t threadIndex, uint32_t entityIndex) -> EntityHandle {
	return (threadIndex & ((1 << ThreadIndexBits) - 1)) | (entityIndex << ThreadIndexBits);
}

bool ecs::EntityLayoutState::operator==(const EntityLayoutState& other) const {
	return
		m_Bits == other.m_Bits &&
		m_BitsJustAttached == other.m_BitsJustAttached &&
		m_BitsJustDetached == other.m_BitsJustDetached;
}

bool ecs::EntityLayoutState::operator!=(const EntityLayoutState& other) const {
	return
		m_Bits != other.m_Bits ||
		m_BitsJustAttached != other.m_BitsJustAttached ||
		m_BitsJustDetached != other.m_BitsJustDetached;
}
