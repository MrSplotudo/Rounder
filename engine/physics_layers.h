#pragma once
#include <Jolt/Jolt.h>
#include "Jolt/Physics/Collision/ObjectLayer.h"
#include "Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h"

namespace Layers {
static constexpr JPH::ObjectLayer STATIC = 0;
static constexpr JPH::ObjectLayer DYNAMIC = 1;
static constexpr JPH::uint NUM_LAYERS = 2;
}

namespace BroadPhaseLayers {
static constexpr JPH::BroadPhaseLayer STATIC(0);
static constexpr JPH::BroadPhaseLayer DYNAMIC(1);
static constexpr JPH::uint NUM_LAYERS(2);
}



class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer object1, JPH::ObjectLayer object2) const override {
        if (object1 == Layers::STATIC && object2 == Layers::STATIC) {
            return false;
        }
        return true;
    }
};

class BroadPhaseLayerInterfaceImpl : public JPH::BroadPhaseLayerInterface {
public:
    JPH::uint GetNumBroadPhaseLayers() const override {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override {
        if (layer == Layers::STATIC) {
            return BroadPhaseLayers::STATIC;
        }
        return BroadPhaseLayers::DYNAMIC;
    }

    #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override {
        if (inLayer == BroadPhaseLayers::STATIC) return "STATIC";
        return "DYNAMIC";
    }
    #endif
};

class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer layer1, JPH::BroadPhaseLayer layer2) const override {
        if (layer1 == Layers::STATIC && layer2 == BroadPhaseLayers::STATIC) {
            return false;
        }
        return true;
    }
};