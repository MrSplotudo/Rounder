#include "physics_world.h"
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>


void PhysicsWorld::create() {
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    tempAllocator = new JPH::TempAllocatorImpl(10* 1024 * 1024);
    jobSystemPool = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
    physicsSystem = new JPH::PhysicsSystem();
}

void PhysicsWorld::update(float deltaTime) {

}

void PhysicsWorld::shutdown() {
    delete physicsSystem;
    delete jobSystemPool;
    delete tempAllocator;
}

