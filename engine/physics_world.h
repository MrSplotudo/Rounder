#pragma once
#include "Jolt/Jolt.h"
#include "physics_layers.h"


namespace JPH {
enum class EMotionType : uint8;
class BodyInterface;
class BodyID;
class TempAllocatorImpl;
class JobSystemThreadPool;
class PhysicsSystem;
}

class PhysicsWorld {
public:
    void create();
    void update(float deltaTime);
    void shutdown();

    JPH::BodyID createBody(JPH::RVec3 position, JPH::Vec3 halfExtents, JPH::EMotionType motionType, JPH::ObjectLayer layer);
    JPH::RVec3 getPosition(JPH::BodyID bodyID);
    JPH::Quat getRotation(JPH::BodyID bodyID);

private:
    JPH::TempAllocatorImpl* tempAllocator = nullptr;
    JPH::JobSystemThreadPool* jobSystemPool = nullptr;
    JPH::PhysicsSystem* physicsSystem = nullptr;
    JPH::BodyInterface* bodyInterface = nullptr;

    const JPH::uint maxBodies = 1024;
    const JPH::uint numBodyMutexes = 0;
    const JPH::uint maxBodyPairs = 1024;
    const JPH::uint maxContactConstraints = 1024;

    BroadPhaseLayerInterfaceImpl broadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseLayerFilter;
    ObjectLayerPairFilterImpl objectLayerPairFilter;
};





