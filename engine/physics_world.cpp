#include "physics_world.h"
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyID.h>


void PhysicsWorld::create() {
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    tempAllocator = new JPH::TempAllocatorImpl(10* 1024 * 1024);
    jobSystemPool = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
    physicsSystem = new JPH::PhysicsSystem();

    physicsSystem->Init(maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints, broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);

    physicsSystem->SetGravity(JPH::Vec3(0.0f, -9.81f, 0.0f));

    bodyInterface = &physicsSystem->GetBodyInterface();
}

void PhysicsWorld::update(float deltaTime) {
    physicsSystem->Update(
        deltaTime,
        1,
        tempAllocator,
        jobSystemPool);
}

void PhysicsWorld::shutdown() {
    delete physicsSystem;
    delete jobSystemPool;
    delete tempAllocator;
}

JPH::BodyID PhysicsWorld::createBody(JPH::RVec3 position, JPH::Vec3 halfExtents, JPH::EMotionType motionType, JPH::ObjectLayer layer) {
    JPH::BoxShapeSettings shapeSettings(halfExtents);
    JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
    JPH::ShapeRefC shape = shapeResult.Get();

    JPH::BodyCreationSettings bodySettings(
        shape,
        position,
        JPH::Quat::sIdentity(),
        motionType,
        layer);

    JPH::BodyID bodyID = bodyInterface->CreateAndAddBody(bodySettings, JPH::EActivation::Activate);

    return bodyID;
}

JPH::Vec3 PhysicsWorld::getPosition(JPH::BodyID bodyID) {
    return bodyInterface->GetPosition(bodyID);
}

JPH::Quat PhysicsWorld::getRotation(JPH::BodyID bodyID) {
    return bodyInterface->GetRotation(bodyID);
}

