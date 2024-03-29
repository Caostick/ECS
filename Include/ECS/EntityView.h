#pragma once

#include <ECS/Common.h>
#include <ECS/Entity.h>

namespace ecs {

	template<typename TL, typename Enable = void>
	struct ComponentArray;

	template<typename TL>
	struct ComponentArray<TL, typename std::enable_if<IsEmpty<TL>::value>::type> {};

	template<typename TL>
	struct ComponentArray<TL, typename std::enable_if<!IsEmpty<TL>::value>::type> {
		void* m_Pointers[Length<TL>::value];
	};

	template<typename... TArgs>
	class EntityView {
		template<typename, typename, typename>
		friend class EntityViewConstructor;
	public:
		using TypeList = TypeList<TArgs...>;

		template<typename T>
		auto Get() const -> T&;

		operator EntityHandle() const;

	private:
		EntityView(EntityHandle handle, const ComponentArray<TypeList>& components);

	private:
		EntityHandle m_EntityHandle;
		ComponentArray<TypeList> m_Components;
	};
}

#include <ECS/EntityView.inl>