#include <ECS/CommandBuffer.h>
#include <ECS/Assert.h>


auto ecs::CommandBuffer::begin() -> const Vector<Command>::iterator {
	return m_Commands.begin();
}

auto ecs::CommandBuffer::end() -> const Vector<Command>::iterator {
	return m_Commands.end();
}

ecs::CommandBuffer::CommandBuffer()
	: m_DataSize(0) {
	m_Pages.push_back(new uint8_t[CommandBufferPageSize]);
}

ecs::CommandBuffer::~CommandBuffer() {
	for (auto page : m_Pages) {
		delete [] page;
	}
}

void ecs::CommandBuffer::DestroyEntity(EntityHandle entityHandle) {
	Command command;
	command.m_CommandType = ECommandType::DestroyEntity;
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

auto ecs::CommandBuffer::AllocateComponentData(uint32_t size) -> uint8_t* {
	ECSAssert(size <= CommandBufferPageSize, "Trying to allocate mem size more than cb page size!");

	const uint32_t currentPageIndex = m_DataSize / CommandBufferPageSize;
	const uint32_t usedPageMemorySize = m_DataSize % CommandBufferPageSize;
	const uint32_t availablePageMemorySize = CommandBufferPageSize - usedPageMemorySize;

	uint32_t pageIndex = currentPageIndex;
	uint8_t* dataPtr = m_Pages[pageIndex] + usedPageMemorySize;

	if (availablePageMemorySize < size) {
		m_DataSize += availablePageMemorySize;

		if (pageIndex >= m_Pages.size()) {
			dataPtr = new uint8_t[CommandBufferPageSize];
			m_Pages.push_back(dataPtr);
		}
	}

	m_DataSize += size;
	return dataPtr;
}