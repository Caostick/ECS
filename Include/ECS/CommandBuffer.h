#pragma once

#include <ECS/Component.h>

namespace ecs {
	enum class ECommandType : uint16_t {
		Unknown,
		DestroyEntity,
		AttachComponent,
		DetachComponent
	};

	struct Command {
		EntityHandle m_EntityHandle;
		ECommandType m_CommandType;
		ComponentTypeId m_ComponentTypeId;
		uint8_t* m_ComponentDataPtr;
	};

	class CommandBuffer {
	public:
		auto begin() -> const Vector<Command>::iterator;
		auto end() -> const Vector<Command>::iterator;

	public:
		CommandBuffer();
		~CommandBuffer();

		template<typename T, typename ...TArgs>
		auto AttachComponent(EntityHandle entityHandle, TArgs&&... args) -> T&;

		template <typename T>
		void DetachComponent(EntityHandle entityHandle);

		void DestroyEntity(EntityHandle entityHandle);

		bool IsEmpty() const;
		void Reset();

	private:
		template<typename T, typename ...TArgs>
		auto AllocateAndConstructComponent(TArgs&&... args) -> T*;

		auto AllocateComponentData(uint32_t size) -> uint8_t*;

	private:
		uint32_t m_DataSize;

		Vector<Command> m_Commands;
		Vector<uint8_t*> m_Pages;
	};
}

#include <ECS/CommandBuffer.inl>