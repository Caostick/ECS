#pragma once

#include <ECS/Common.h>
#include <ECS/Entity.h>
#include <ECS/CommandBuffer.h>

namespace ecs {

	class WorldTLS {
	public:
		WorldTLS();

		uint32_t CreateEntity();

		void MarkForDestroy(uint32_t entityIndex, bool value);
		bool IsMarkedForDestroy(uint32_t entityIndex) const;
		void MarkForUpdate(uint32_t entityIndex, bool value);
		bool IsMarkedForUpdate(uint32_t entityIndex) const;
		void RequestComponentAttach(uint32_t entityIndex, uint32_t componentTypeId);
		void RequestComponentDetach(uint32_t entityIndex, uint32_t componentTypeId);
		void RequestAllComponentsDetach(uint32_t entityIndex);

		ecs::EntityLayout& GetEntityLayout(uint32_t entityIndex);
		ecs::Entity& GetEntity(uint32_t entityIndex);
		const ecs::Entity& GetEntity(uint32_t entityIndex) const;
		uint32_t GetEntityGroupIndex(uint32_t entityIndex) const;
		uint32_t GetEntityLocalIndex(uint32_t entityIndex) const;
		void DestroyEntity(uint32_t entityIndex);

		void ResetCommands();

		ecs::CommandBuffer& GetCommandBuffer();

	private:
		void IncreasePoolSize(uint32_t newPoolSize);

		ecs::CommandBuffer m_CommandBuffer;
		ecs::Vector<uint32_t> m_EntityIndices;
		ecs::Vector<Entity> m_Entities;
		ecs::Vector<EntityLayout> m_Layouts;
		ecs::Vector<bool> m_RemovedEntities;
		ecs::Vector<bool> m_DirtyEntities;
		uint32_t m_EntityPoolSize;
		uint32_t m_EntityCount;
		uint32_t m_AvailableEntityIndex;
	};
}
