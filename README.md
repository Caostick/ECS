# ECS
Entity Component System (work in progress)
------------------------------------------

ECS from pet game project. Still needs some polish for new features, but interface is complete.

Features:
- Flexible
- Easily expandable
- Cache-friendly
- Lockless safe multithreaded

WIP:
- Remove component type limit
- Support for shared libs (currently works with static linked app, needs component sorting for shared).
- Task manager/scheduler for systems according component access mask.
- Global components(system context) with access mask. Instead of system dependency arguments, which can be thread-unsafe.

Initialization
--------------
```cpp
#include <ECS/World.h>

// Create ecs world instance
ecs::World world;

// Initialize world
// If you need multithreaded processing, provide
// ThreadManager implementation
world.Init();

// Register your ecs systems
world.RegisterSystem<MySystem>();
world.RegisterSystem<AnotherSystem>();
...
```

Updating
--------
```cpp
// Pass through all registered systems and call updates
world.Update(deltaTime);

// Executes all commands stored in command buffers
world.ExecuteCommands();
```

Deinitialization
----------------
```cpp
// Finish all commands which still can be stored in ecs command buffers
world.FinishAllCommands();

world.Release();
```




Components
----------

Components are just data and have no any declared methods. All logic performed in systems.

```cpp
struct MyComponentA {
  MyComponentA() = default;
  MyComponentA(MyComponentA&&) = default;
  MyComponentA& operator = (MyComponentA&&) = default;

  MyComponentA(const MyComponentA&) = delete;
  MyComponentA& operator = (const MyComponentA&) = delete;

  int Var;
};

struct MyComponentB {
  MyComponentB(int var) : Var(var) {}
  MyComponentB(MyComponentB&&) = default;
  MyComponentB& operator = (MyComponentB&&) = default;

  MyComponentB(const MyComponentB&) = delete;
  MyComponentB& operator = (const MyComponentB&) = delete;

  int Var;
};

// Component may not have data
// In this case component will be considered as flag and used only for access bit
struct MyComponentC {};
```

Systems
--------------

Systems are logic which works according to entity component combination.

.h
```cpp
#include <ECS/System.h>

#include <MyComponentA.h>
#include <MyComponentB.h>

class MySystem : public ecs::System {
public:
  // Component access mask. Const means read-only component access
  using WorldView = ecs::WorldView<
    const MyComponentA,
    MyComponentB
  >;

  void Update(WorldView& world, float dt);
};
```

.cpp
```cpp
#include <MySystem.h>
#include <ECS/Query.h>

void MySystem::Update(WorldView& world, float dt) {
  using Query = ecs::Query<>
    ::Include<MyComponentA>
    ::Include<MyComponentB>;

  // Pass through all entities, which have components in query
  for (auto e : Query::Iterate(world)) {
    // Will be const, since we declared it as const in WorldView
    auto& componentA = e.Get<MyComponentA>();
    
    // This component has both read/write access
    auto& componentB = e.Get<MyComponentB>();
    
    componentB.Var = componentA.Var;
  }
}
```

For query available next access masks:
- ::Include - entity has component
- ::Exclude - entity has no component
- ::Added - component has been just added to entity
- ::Removed - component has been just removed from entity. Component data lives one frame after component detaching

One may want to have optional access to a component what is not present in query, since it may/may not be attached to an entity. In this case there is a way to check whether component is attached or not. Component has to be present in WorldView as well. (IMPORTANT!!! Will be deprecated. EntityView will have access to optional instead of WorldView)
```cpp
  using WorldView = ecs::WorldView<OptionalComponent, ...>;
  
  ...
	
  if(world.Has<OptionalComponent>(e)) {
    // IMPORTANT!!! Will be deprecated. EntityView will have access to optional instead of WorldView
    auto& optionalComponent = world.GetComponent<OptionalComponent>(e);
  }
```

Entity
---------------

Entity is just component container that has own handle/index. All components are stored in cache-friendly groups(buffers) according to entity layout(components bits)

Entity and component creation
```cpp
// Entity creates immediately and stores in thread-based storage, so this operation is thread safe
auto e = world.CreateEntity();

// This operation is thread safe as well, since component attaching is command based operation
// Components will be availavle for querying of getting next frame after command buffers are executed
world.AttachComponent<MyComponentA>(e);
world.AttachComponent<MyComponentB>(e, 123);
```

If you want to has access to created component data right after creation, you may use entity creation via EntityView
```cpp
#include <ECS/EntityView.h>
#include <ECS/EntityViewConstructor.h>

using MyEntity = ecs::EntityView<
  MyComponentA,
  MyComponentB
>;

// Returns entity view. Available only current frame, so don't srore it anywhere
auto e = ecs::Construct<MyEntity>(world)
  .Construct<MyComponentA>()
  .Construct<MyComponentB>(123);
  
auto& myComponentA = e.Get<MyComponentA>();
myComponentA.Var = 321;
```

Entity and component removing
```
world.DetachComponent<MyComponentB>(e);

world.DestroyEntity(e);

// If you destroy entity, no need to detach components, components will be destoyed automatically
```
