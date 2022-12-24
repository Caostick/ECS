#pragma once

#include <ECS/Common.h>
#include <ECS/Entity.h>
#include <ECS/CommandBuffer.h>

namespace ecs {

	class WorldTLS {
	public:
		WorldTLS();

		auto CreateEntity() -> uint32_t;

		void MarkForDestroy(uint32_t entityIndex, bool value);
		bool IsMarkedForDestroy(uint32_t entityIndex) const;
		void MarkForUpdate(uint32_t entityIndex, bool value);
		bool IsMarkedForUpdate(uint32_t entityIndex) const;
		void RequestComponentAttach(uint32_t entityIndex, uint32_t componentTypeId);
		void RequestComponentDetach(uint32_t entityIndex, uint32_t componentTypeId);
		void RequestAllComponentsDetach(uint32_t entityIndex);

		auto GetEntityLayout(uint32_t entityIndex) -> EntityLayout&;
		auto GetEntityLayout(uint32_t entityIndex) const -> const EntityLayout&;
		auto GetEntity(uint32_t entityIndex) -> Entity&;
		auto GetEntity(uint32_t entityIndex) const -> const Entity&;
		auto GetEntityGroupIndex(uint32_t entityIndex) const -> uint32_t;
		auto GetEntityLocalIndex(uint32_t entityIndex) const -> uint32_t;
		void DestroyEntity(uint32_t entityIndex);

		void ResetCommands();

		auto GetCommandBuffer() -> CommandBuffer&;

	private:
		void IncreasePoolSize(uint32_t newPoolSize);

		CommandBuffer m_CommandBuffer;
		Vector<uint32_t> m_EntityIndices;
		Vector<Entity> m_Entities;
		Vector<EntityLayout> m_Layouts;
		Vector<bool> m_RemovedEntities;
		Vector<bool> m_DirtyEntities;
		uint32_t m_EntityPoolSize;
		uint32_t m_EntityCount;
		uint32_t m_AvailableEntityIndex;
	};
}
