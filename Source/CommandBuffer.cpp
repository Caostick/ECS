#include <ECS/CommandBuffer.h>
#include <ECS/Allocator.h>
#include <ECS/Assert.h>


const ecs::Vector<ecs::Command>::iterator ecs::CommandBuffer::begin() {
	return m_Commands.begin();
}

const ecs::Vector<ecs::Command>::iterator ecs::CommandBuffer::end() {
	return m_Commands.end();
}

ecs::CommandBuffer::CommandBuffer()
	: m_DataSize(0) {
	m_Pages.push_back(ECSNew uint8_t[ecs::CommandBufferPageSize]);
}

ecs::CommandBuffer::~CommandBuffer() {
	for (auto page : m_Pages) {
		ECSDelete [] page;
	}
}

void ecs::CommandBuffer::DestroyEntity(ecs::EntityHandle entityHandle) {
	ecs::Command command;
	command.m_CommandType = ecs::ECommandType::DestroyEntity;
	command.m_EntityHandle = entityHandle;

	m_Commands.emplace_back(command);
}

bool ecs::CommandBuffer::IsEmpty() const {
	return m_Commands.empty();
}

void ecs::CommandBuffer::Reset() {
	m_Commands.clear();
	m_DataSize = 0;
}

uint8_t* ecs::CommandBuffer::AllocateComponentData(uint32_t size) {
	ECSAssert(size <= ecs::CommandBufferPageSize, "Trying to allocate mem size more than cb page size!");

	const uint32_t currentPageIndex = m_DataSize / ecs::CommandBufferPageSize;
	const uint32_t usedPageMemorySize = m_DataSize % ecs::CommandBufferPageSize;
	const uint32_t availablePageMemorySize = ecs::CommandBufferPageSize - usedPageMemorySize;

	uint32_t pageIndex = currentPageIndex;
	uint8_t* dataPtr = m_Pages[pageIndex] + usedPageMemorySize;

	if (availablePageMemorySize < size) {
		m_DataSize += availablePageMemorySize;

		if (pageIndex >= m_Pages.size()) {
			dataPtr = ECSNew uint8_t[ecs::CommandBufferPageSize];
			m_Pages.push_back(dataPtr);
		}
	}

	m_DataSize += size;
	return dataPtr;
}