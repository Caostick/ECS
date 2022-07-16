# ECS
Entity Component System
-----------------------

Features:
- Flexible
- Easily expandable
- Cache-friendly
- Lockless safe multithreaded

WIP:
- Support for shared libs (currently works with static linked app, needs component sorting for shared).
- Task manager/scheduler for systems according component access mask.
- Global components(world context) with access mask. Instead of system dependency arguments, which can be thread-unsafe.

Initialization
--------------
```
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
```
// Pass through all registered systems and call updates
world.Update(deltaTime);

// Executes all commands stored in command buffers
world.ExecuteCommands();
```

Deinitialization
----------------
```
// Finish all commands which still can be stored in ecs command biffers
world.FinishAllCommands();

world.Release();
```




Components
----------

Components are just data and have no any declared methods. All logic performed in systems.

```
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
```

Systems
--------------

Systems are logic which works according to entity component combination.

.h
```
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
```
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
    
    // This component has both read/write acess
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

Entity
---------------

Entity is juct component container that has own handle/index. All components are stored in cache-friendly groups(buffers) according to entity layout(components bits)

Entity and component creation
```
// Entity creates immediately and stores in thread-based storage, so this operation is thread safe
auto e = world.CreateEntity();

// This operation is thread safe as well, since component attaching is command based operation
// Components will be availavle for querying of getting next frame after command buffers are executed
world.AttachComponent<MyComponentA>(e);
world.AttachComponent<MyComponentB>(e, 123);
```

If you want to has access to created component data right after creation, you may use entity creation via EntityView
```
#include <ECS/EntityView.h>

using MyEntity = ecs::EntityView<
  MyComponentA,
  MyComponentB
>;

// Returns entity view. Available only current frame, so don't srore it anywhere
auto e = world.Construct<MyEntity>()
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
