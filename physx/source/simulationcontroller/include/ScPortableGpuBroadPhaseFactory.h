// SPDX-FileCopyrightText: Copyright (c) 2008-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-FileCopyrightText: Copyright (c) 2026 thomthehound
// SPDX-License-Identifier: Apache-2.0

#ifndef SC_PORTABLE_GPU_BROADPHASE_FACTORY_H
#define SC_PORTABLE_GPU_BROADPHASE_FACTORY_H

#include "foundation/PxSimpleTypes.h"

namespace physx {
class PxSceneDesc;
namespace Cm {
class VirtualAllocatorCallback;
template<class T> class PinnableArray;
}
namespace Bp {
class BroadPhase;
class AABBManagerBase;
class BoundsArray;
}
namespace Sc {

// Scene-owned source integration seam for a portable implementation of
// PxgCudaBroadPhaseSap and PxgAABBManager. Both objects participate in the
// ORIGINAL Bp::BroadPhase/AABBManagerBase task and report interfaces.
// The provider is borrowed during scene construction; the broadphase and
// AABB manager are owned/destroyed by Sc::Scene exactly as in CUDA PhysX.
// Implementations must not create a second scene, frame driver or CPU ABP.
// Both factory methods MUST return a valid source-owned object or fail
// admission, never a null object allowing Sc::Scene to continue half built.
class PortableGpuBroadPhaseFactory {
public:
    virtual ~PortableGpuBroadPhaseFactory() {}

    // Called after scene-level GPU admission. Returns a source-derived
    // portable GPU broadphase allocated for the owning PhysX scene.
    virtual Bp::BroadPhase& createBroadPhase(
        const PxSceneDesc& desc, PxU64 contextID) = 0;

    // Called after Sc::Scene allocates its bounds and contact-distance
    // arrays, just as the CUDA factory does. This owner must process
    // original updateBPFirstPass/SecondPass/postBroadPhase tasks and
    // publish found/lost pairs into the scene's normal CPU dynamics path.
    virtual Bp::AABBManagerBase& createAABBManager(
        const PxSceneDesc& desc,
        Bp::BroadPhase& broadphase,
        Bp::BoundsArray& bounds,
        Cm::PinnableArray<PxReal>& contactDistances,
        Cm::VirtualAllocatorCallback& hostAllocator,
        PxU64 contextID) = 0;

protected:
    PortableGpuBroadPhaseFactory() {}
};

} // namespace Sc
} // namespace physx

#endif
