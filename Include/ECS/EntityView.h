#pragma once

#include <ECS/Source/Common.h>
#include <ECS/Source/Entity.h>

namespace ecs {

	template<typename TL, typename Enable = void>
	struct ComponentArray;

	template<typename TL>
	struct ComponentArray<TL, typename std::enable_if<ecs::IsEmpty<TL>::value>::type> {};

	template<typename TL>
	struct ComponentArray<TL, typename std::enable_if<!ecs::IsEmpty<TL>::value>::type> {
		void* m_Pointers[ecs::Length<TL>::value];
	};

	template<typename... TArgs>
	class EntityView {
		template<typename, typename, typename>
		friend class EntityViewConstructor;
	public:
		using TypeList = ecs::TypeList<TArgs...>;

		template<typename T>
		auto Get() const -> T&;

		operator EntityHandle() const;

	private:
		EntityView(ecs::EntityHandle handle, const ComponentArray<TypeList>& components);

	private:
		EntityHandle m_EntityHandle;
		ComponentArray<TypeList> m_Components;
	};
}

#include <ECS/Source/EntityView.inl>