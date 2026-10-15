// SPDX-FileCopyrightText: Copyright (c) 2026 thomthehound
// SPDX-License-Identifier: Apache-2.0
#ifndef PX_PORTABLE_GPU_BROADPHASE_PROVIDER_H
#define PX_PORTABLE_GPU_BROADPHASE_PROVIDER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Fixed C layout: selection and the factory crossing are backend-neutral.
// Factory-produced objects implement the ORIGINAL PhysX Bp::BroadPhase and
// Bp::AABBManagerBase contracts, and therefore must be compiled against the
// same PhysX version as the host. This does not claim a stable binary ABI
// for those internal C++ objects across unrelated PhysX releases.
#define PX_PORTABLE_GPU_BP_PROVIDER_ABI_V1 1u

typedef struct PxPortableGpuBroadPhaseProviderV1 {
    uint32_t abi_version;
    uint32_t struct_size;
    void* user_data;

    // Evaluated once by PxPhysics::createScene, before scene construction.
    // scene_desc is a borrowed const physx::PxSceneDesc*.
    // A false result rejects the scene; it MUST NOT select a CPU fallback.
    uint32_t (*supports_scene)(void* user_data, const void* scene_desc);

    // These callbacks are called by the ORIGINAL Sc::Scene factory path.
    // The provider owns GPU resources; the returned objects own their
    // PhysX-facing lifetimes and implement the original virtual methods.
    // Non-null return values are required after successful supports_scene.
    // scene_desc: const physx::PxSceneDesc*
    // return: physx::Bp::BroadPhase* (released by PhysX)
    void* (*create_broadphase)(void* user_data, const void* scene_desc,
                               uint64_t context_id);

    // broadphase: physx::Bp::BroadPhase*
    // bounds: physx::Bp::BoundsArray*
    // distances: physx::Cm::PinnableArray<physx::PxReal>*
    // host_allocator: physx::Cm::VirtualAllocatorCallback*
    // return: physx::Bp::AABBManagerBase* (destroyed by PhysX)
    void* (*create_aabb_manager)(void* user_data, const void* scene_desc,
                                void* broadphase, void* bounds,
                                void* distances, void* host_allocator,
                                uint64_t context_id);
} PxPortableGpuBroadPhaseProviderV1;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // PX_PORTABLE_GPU_BROADPHASE_PROVIDER_H
