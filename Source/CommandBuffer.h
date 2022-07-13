#pragma once

#include <ECS/Source/Component.h>

namespace ecs {
	enum class ECommandType : uint16_t {
		Unknown,
		DestroyEntity,
		AttachComponent,
		DetachComponent
	};

	struct Command {
		ecs::EntityHandle m_EntityHandle;
		ecs::ECommandType m_CommandType;
		ecs::ComponentTypeId m_ComponentTypeId;
		uint8_t* m_ComponentDataPtr;
	};

	class CommandBuffer {
	public:
		const ecs::Vector<ecs::Command>::iterator begin();
		const ecs::Vector<ecs::Command>::iterator end();

	public:
		CommandBuffer();
		~CommandBuffer();

		template<typename T, typename ...TArgs>
		T& AttachComponent(ecs::EntityHandle entityHandle, TArgs&&... args);

		template <typename T>
		void DetachComponent(ecs::EntityHandle entityHandle);

		void DestroyEntity(ecs::EntityHandle entityHandle);

		bool IsEmpty() const;
		void Reset();

	private:
		template<typename T, typename ...TArgs>
		T* AllocateAndConstructComponent(TArgs&&... args);

		uint8_t* AllocateComponentData(uint32_t size);

		uint32_t m_DataSize;

		ecs::Vector<Command> m_Commands;
		ecs::Vector<uint8_t*> m_Pages;
	};
}

#include <ECS/Source/CommandBuffer.inl>