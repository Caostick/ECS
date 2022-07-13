#pragma once

#include <ECS/Common.h>

namespace ecs {
	struct Group {
	public:
		Group(const ecs::Bitset& bits, uint32_t groupIndex);
		~Group();

		uint32_t GetIndex() const;
		uint32_t GetEntityCount() const;
		bool IsEmpty() const;
		const ecs::Bitset& GetTypeBits() const;

		uint32_t AddEntity(ecs::EntityHandle entity);
		ecs::EntityHandle RemoveEntity(uint32_t localIndex);
		ecs::EntityHandle GetEntityBackReference(uint32_t localIndex);

		uint8_t* GetComponentData(uint32_t localIndex, ecs::ComponentTypeId componentTypeId);

		template<typename T>
		T* GetComponent(uint32_t localIndex);

	private:
		void AddDataPage();
		void RemoveDataPage();

		int32_t m_ComponentOffsets[ecs::MaxComponentCount];
		ecs::Bitset m_EntityTypeBits;

		uint32_t m_GroupIndex;
		uint32_t m_EntityCount;
		uint32_t m_EntitySize;

		ecs::Vector<ecs::EntityHandle> m_EntityBackRefs;
		ecs::Vector<uint8_t*> m_DataPages;
	};
}

#include <ECS/Group.inl>