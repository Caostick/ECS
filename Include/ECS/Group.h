#pragma once

#include <ECS/Common.h>
#include <ECS/Component.h>

namespace ecs {
	struct Group {
	public:
		Group(const Bitset& bits, uint32_t groupIndex);
		~Group();

		auto GetIndex() const -> uint32_t;
		auto GetEntityCount() const -> uint32_t;
		bool IsEmpty() const;
		auto GetTypeBits() const -> const Bitset&;

		auto AddEntity(EntityHandle entity) -> uint32_t;
		auto RemoveEntity(uint32_t localIndex) -> EntityHandle;
		auto GetEntityBackReference(uint32_t localIndex) -> EntityHandle;

		auto GetComponentData(uint32_t localIndex, ComponentTypeId componentTypeId) -> uint8_t*;

		template<typename T>
		auto GetComponent(uint32_t localIndex) -> T*;

	private:
		void AddDataPage();
		void RemoveDataPage();

		int32_t m_ComponentOffsets[MaxComponentCount];
		Bitset m_EntityTypeBits;

		uint32_t m_GroupIndex;
		uint32_t m_EntityCount;
		uint32_t m_EntitySize;

		Vector<EntityHandle> m_EntityBackRefs;
		Vector<uint8_t*> m_DataPages;
	};
}

#include <ECS/Group.inl>