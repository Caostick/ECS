#include <ECS/Common.h>

uint32_t ecs::ToThreadIndex(EntityHandle handle) {
	return handle & ((1 << ThreadIndexBits) - 1);
}

uint32_t ecs::ToEntityIndex(EntityHandle handle) {
	return handle >> ThreadIndexBits;
}

ecs::EntityHandle ecs::ToEntityHandle(uint32_t threadIndex, uint32_t entityIndex) {
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
