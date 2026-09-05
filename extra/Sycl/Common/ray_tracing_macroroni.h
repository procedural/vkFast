#pragma once

#if 0
Ray Tracing Macroroni, a header-only C++ library by Constantine Tarasenkov (iamvfx@gmail.com), 2026.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
#endif

#if !defined(RTM_ENABLE_EMBREE_GPU) && !defined(RTM_ENABLE_EMBREE_CPU)
  #error define RTM_ENABLE_EMBREE_GPU or *_CPU
#endif
#if defined(RTM_ENABLE_EMBREE_GPU) && defined(RTM_ENABLE_EMBREE_CPU)
  #error define either RTM_ENABLE_EMBREE_GPU or *_CPU
#endif

// Includes

#ifdef RTM_ENABLE_EMBREE_CPU
  #include <tbb/parallel_for.h>
  #include <tbb/blocked_range2d.h>
#endif

#ifdef RTM_ENABLE_EMBREE_GPU
  #include <sycl/sycl.hpp>
#endif

#if defined(RTM_ENABLE_EMBREE_GPU) || defined(RTM_ENABLE_EMBREE_CPU)
  #include <embree4/rtcore.h>
#endif

// Macros

#ifdef RTM_ENABLE_EMBREE_CPU

  #define RTM_CREATE_EMBREE_DEVICE_AND_SYCL_QUEUE() \
    RTCDevice rtm_rtc_device = rtcNewDevice(NULL);

  #define rtmSyclQueueGetDeviceName(SYCL_QUEUE) std::string("CPU TBB")

  #define rtmMallocShared(X, Y) malloc(X)

  #define rtmFree(X, Y) free(X)

  #define RTM_TBB_SYCL_PARALLEL_FOR_XY_BEGIN(RANGE_WIDTH, RANGE_HEIGHT, SYCL_QUEUE) \
    tbb::parallel_for( \
      tbb::blocked_range2d<int>(0, (RANGE_HEIGHT), 0, (RANGE_WIDTH)), [&](const tbb::blocked_range2d<int>& rtm_tbb_range) { \
        for (int y = rtm_tbb_range.rows().begin(); y != rtm_tbb_range.rows().end(); ++y) { \
          for (int x = rtm_tbb_range.cols().begin(); x != rtm_tbb_range.cols().end(); ++x) {

  #define RTM_TBB_SYCL_PARALLEL_FOR_XY_END() \
          } \
        } \
      } \
    );

  #define rtmSyclQueueWait(SYCL_QUEUE)

#endif

#ifdef RTM_ENABLE_EMBREE_GPU

  #define RTM_CREATE_EMBREE_DEVICE_AND_SYCL_QUEUE() \
    sycl::queue rtm_sycl_queue(sycl::gpu_selector_v); \
    RTCDevice rtm_rtc_device = rtcNewSYCLDevice(rtm_sycl_queue.get_context(), NULL);

  #define rtmSyclQueueGetDeviceName(SYCL_QUEUE) (SYCL_QUEUE).get_device().get_info<sycl::info::device::name>()

  #define rtmMallocShared sycl::malloc_shared

  #define rtmFree sycl::free

  #define RTM_TBB_SYCL_PARALLEL_FOR_XY_BEGIN(RANGE_WIDTH, RANGE_HEIGHT, SYCL_QUEUE) \
    (SYCL_QUEUE).submit([&](sycl::handler& rtm_sycl_cgh) { \
      rtm_sycl_cgh.parallel_for(sycl::range<2>((RANGE_HEIGHT), (RANGE_WIDTH)), [=](sycl::id<2> rtm_sycl_id) { \
        int y = rtm_sycl_id[0]; \
        int x = rtm_sycl_id[1];

  #define RTM_TBB_SYCL_PARALLEL_FOR_XY_END() \
      }); \
    });

  #define rtmSyclQueueWait(SYCL_QUEUE) (SYCL_QUEUE).wait()

#endif
