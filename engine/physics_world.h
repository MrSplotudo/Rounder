#pragma once
#include "Jolt/Jolt.h"
#include "physics_layers.h"


namespace JPH {
class TempAllocatorImpl;
class JobSystemThreadPool;
class PhysicsSystem;
}

class PhysicsWorld {
public:
    void create();
    void update(float deltaTime);
    void shutdown();

private:
    JPH::TempAllocatorImpl* tempAllocator = nullptr;
    JPH::JobSystemThreadPool* jobSystemPool = nullptr;
    JPH::PhysicsSystem* physicsSystem = nullptr;
};





