#include "../../vkfast.h"
#include "../../vkfast_ids.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"
#include "../../extra/REII/vkfast_extra_reii.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI
#include "../Common/vkfast_examples_common.h"

#if defined(__linux__) && !defined(__ANDROID__)
#include "/home/linuxbrew/RedGpuSDK/radeonrays.h"
#include "/home/linuxbrew/RedGpuSDK/radeonrays_redgpu.h"
#include "/home/linuxbrew/RedGpuSDK/misc/examples/sponza_ray_tracing/mesh_data_positions.h" // NOTE(Constantine): On Linux, needs `ulimit -s unlimited`, currently.
#include "/home/linuxbrew/RedGpuSDK/misc/examples/sponza_ray_tracing/mesh_data_indices.h"
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../extra/RadeonRays SDK 4.1/test/test_vk/stb_image_write.h"

typedef enum RedHelperMemoryType {
  RED_HELPER_MEMORY_TYPE_VRAM     = 0,
  RED_HELPER_MEMORY_TYPE_UPLOAD   = 1,
  RED_HELPER_MEMORY_TYPE_READBACK = 2,
} RedHelperMemoryType;

REDGPU_DECLSPEC unsigned REDGPU_API redHelperGetMemoryTypeIndex(const RedGpuInfo * gpuInfo, RedHelperMemoryType memoryType, unsigned memoryTypesSupported) {
  const unsigned        memoryTypesCount = gpuInfo->memoryTypesCount;
  const RedMemoryType * memoryTypes      = gpuInfo->memoryTypes;

  unsigned memoryTypeIsSupported[32];
  memoryTypeIsSupported[0]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? 0 : 1;
  memoryTypeIsSupported[1]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? 0 : 1;
  memoryTypeIsSupported[2]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? 0 : 1;
  memoryTypeIsSupported[3]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[4]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[5]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[6]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[7]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[8]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[9]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[10] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[11] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[12] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[13] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[14] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[15] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[16] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[17] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[18] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[19] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[20] = (memoryTypesSupported & REDGPU_B32(0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[21] = (memoryTypesSupported & REDGPU_B32(0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[22] = (memoryTypesSupported & REDGPU_B32(0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[23] = (memoryTypesSupported & REDGPU_B32(0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[24] = (memoryTypesSupported & REDGPU_B32(0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[25] = (memoryTypesSupported & REDGPU_B32(0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[26] = (memoryTypesSupported & REDGPU_B32(0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[27] = (memoryTypesSupported & REDGPU_B32(0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[28] = (memoryTypesSupported & REDGPU_B32(0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[29] = (memoryTypesSupported & REDGPU_B32(0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[30] = (memoryTypesSupported & REDGPU_B32(0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[31] = (memoryTypesSupported & REDGPU_B32(1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;

  if (memoryType == RED_HELPER_MEMORY_TYPE_VRAM) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isGpuVram == 1 && memoryTypeIsSupported[i] == 1) {
        return i;
      }
    }
  } else if (memoryType == RED_HELPER_MEMORY_TYPE_UPLOAD) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 0 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
  } else if (memoryType == RED_HELPER_MEMORY_TYPE_READBACK) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
  }

  return -1;
}

RRError rrCheckCallError(const char * file, int line, RRError error, const char * call) {
  if (error != RR_SUCCESS) {
    printf("[REDGPU RadeonRays][%s][%d][RR_ERROR]: %d == %s", file, line, error, call);
    fflush(stdout);
  }
  return error;
}

#define RR_CHECK(CALL) rrCheckCallError(__FILE__, __LINE__, CALL, #CALL)

int main() {
#if defined(__MINGW32__)
  SetProcessDPIAware();
#elif defined(_WIN32)
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  #define WINDOW_W 1024
  #define WINDOW_H 1024

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(WINDOW_W, WINDOW_H, "[vkFast] RT 3D Mesh Sponza GPU RR 4.1", 0, 0);
#if defined(_WIN32)
  void * window_handle = (void *)glfwGetWin32Window(window);
#elif defined(__linux__) && !defined(__ANDROID__)
  // NOTE(Constantine): this struct's layout is defined in redgpu_32.c file of REDGPU 2 SDK.
  struct X11WindowData {
    Display * display;
    Window    window;
    Atom      wmDeleteMessage;
  };
  struct X11WindowData windowData = {0};
  windowData.display = glfwGetX11Display();
  windowData.window = glfwGetX11Window(window);
  windowData.wmDeleteMessage = 0;
  REDGPU_2_EXPECTFL(windowData.display != NULL || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  REDGPU_2_EXPECTFL(windowData.window  != 0    || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  void * window_handle = &windowData;
#endif

  gpu_internal_memory_allocation_sizes_t memory_allocation_sizes = {0};
  memory_allocation_sizes.bytes_count_for_memory_storages_type_gpu_only         = 0;
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_upload       = 0;
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_readback     = 0;
  memory_allocation_sizes.bytes_count_for_memory_present_pixels_type_cpu_upload = VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_PRESENT_PIXELS_CPU_UPLOAD_288MB;
  gpu_context_optional_parameters_t optional_parameters = {0};
  optional_parameters.internal_memory_allocation_sizes = &memory_allocation_sizes;

  // NOTE(Constantine): You can also define REDGPU_COMPILE_SWITCH_DEBUG to see extra errors.
  gpu_handle_context_t ctx = vfContextInit(1, &optional_parameters, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] RT 3D Mesh Sponza GPU RR 4.1", WINDOW_W, WINDOW_H, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)ctx;
  RedContext redcontext = vkfast->context;

  RRContext context = NULL;
  RR_CHECK(rrCreateContextVk(RR_API_VERSION, redcontext->gpus[0].gpu, redcontext->gpus[0].gpuDevice, redcontext->gpus[0].queues[0], redcontext->gpus[0].queuesFamilyIndex[0], &context));

  RedArray vertex_buffer_cpu = {0};
  RedArray vertex_buffer_gpu = {0};
  RedArray index_buffer_cpu  = {0};
  RedArray index_buffer_gpu  = {0};
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "vertex_buffer_cpu", RED_ARRAY_TYPE_ARRAY_RO, sizeof(mesh_data_positions), 0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &vertex_buffer_cpu, NULL, __FILE__, __LINE__, NULL);
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "vertex_buffer_gpu", RED_ARRAY_TYPE_ARRAY_RW, sizeof(mesh_data_positions), 0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &vertex_buffer_gpu, NULL, __FILE__, __LINE__, NULL);
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "index_buffer_cpu",  RED_ARRAY_TYPE_ARRAY_RO, sizeof(mesh_data_indices),   0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &index_buffer_cpu,  NULL, __FILE__, __LINE__, NULL);
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "index_buffer_gpu",  RED_ARRAY_TYPE_ARRAY_RW, sizeof(mesh_data_indices),   0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &index_buffer_gpu,  NULL, __FILE__, __LINE__, NULL);

  RedHandleMemory vertex_buffer_memory_cpu = {0};
  RedHandleMemory vertex_buffer_memory_gpu = {0};
  RedHandleMemory index_buffer_memory_cpu  = {0};
  RedHandleMemory index_buffer_memory_gpu  = {0};
  void * vertex_buffer_memory_cpu_pointer  = 0;
  void * index_buffer_memory_cpu_pointer   = 0;
  redMemoryAllocateMappable(redcontext, redcontext->gpus[0].gpu, "vertex_buffer_memory_cpu", 0, vertex_buffer_cpu.handle, vertex_buffer_cpu.memoryBytesCount, redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_UPLOAD, vertex_buffer_cpu.memoryTypesSupported),             0, &vertex_buffer_memory_cpu, NULL, __FILE__, __LINE__, NULL);
  redMemoryAllocate        (redcontext, redcontext->gpus[0].gpu, "vertex_buffer_memory_gpu",                              vertex_buffer_gpu.memoryBytesCount, redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_VRAM,   vertex_buffer_gpu.memoryTypesSupported), NULL, NULL, 0, &vertex_buffer_memory_gpu, NULL, __FILE__, __LINE__, NULL);
  redMemoryAllocateMappable(redcontext, redcontext->gpus[0].gpu, "index_buffer_memory_cpu",  0, index_buffer_cpu.handle,  index_buffer_cpu.memoryBytesCount,  redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_UPLOAD, index_buffer_cpu.memoryTypesSupported),              0, &index_buffer_memory_cpu,  NULL, __FILE__, __LINE__, NULL);
  redMemoryAllocate        (redcontext, redcontext->gpus[0].gpu, "index_buffer_memory_gpu",                               index_buffer_gpu.memoryBytesCount,  redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_VRAM,   index_buffer_gpu.memoryTypesSupported),  NULL, NULL, 0, &index_buffer_memory_gpu,  NULL, __FILE__, __LINE__, NULL);
  redMemoryMap(redcontext, redcontext->gpus[0].gpu, vertex_buffer_memory_cpu, 0, sizeof(mesh_data_positions), &vertex_buffer_memory_cpu_pointer, NULL, __FILE__, __LINE__, NULL);
  redMemoryMap(redcontext, redcontext->gpus[0].gpu, index_buffer_memory_cpu,  0, sizeof(mesh_data_indices),   &index_buffer_memory_cpu_pointer,  NULL, __FILE__, __LINE__, NULL);

  {
    RedMemoryArray memoryArrays[4] = {0};

    memoryArrays[0].setTo1000157000  = 1000157000;
    memoryArrays[0].setTo0           = 0;
    memoryArrays[0].array            = vertex_buffer_cpu.handle;
    memoryArrays[0].memory           = vertex_buffer_memory_cpu;
    memoryArrays[0].memoryBytesFirst = 0;

    memoryArrays[1].setTo1000157000  = 1000157000;
    memoryArrays[1].setTo0           = 0;
    memoryArrays[1].array            = vertex_buffer_gpu.handle;
    memoryArrays[1].memory           = vertex_buffer_memory_gpu;
    memoryArrays[1].memoryBytesFirst = 0;

    memoryArrays[2].setTo1000157000  = 1000157000;
    memoryArrays[2].setTo0           = 0;
    memoryArrays[2].array            = index_buffer_cpu.handle;
    memoryArrays[2].memory           = index_buffer_memory_cpu;
    memoryArrays[2].memoryBytesFirst = 0;

    memoryArrays[3].setTo1000157000  = 1000157000;
    memoryArrays[3].setTo0           = 0;
    memoryArrays[3].array            = index_buffer_gpu.handle;
    memoryArrays[3].memory           = index_buffer_memory_gpu;
    memoryArrays[3].memoryBytesFirst = 0;

    redMemorySet(redcontext, redcontext->gpus[0].gpu, countof(memoryArrays), memoryArrays, 0, NULL, NULL, __FILE__, __LINE__, NULL);
  }

  memcpy(vertex_buffer_memory_cpu_pointer, &mesh_data_positions[0], sizeof(mesh_data_positions));
  memcpy(index_buffer_memory_cpu_pointer,  &mesh_data_indices[0],   sizeof(mesh_data_indices));

  {
    RedCalls calls_upload = {0};
    redCreateCallsReusable(redcontext, redcontext->gpus[0].gpu, "calls_upload", redcontext->gpus[0].queuesFamilyIndex[0], &calls_upload, NULL, __FILE__, __LINE__, NULL);
    RedCallProceduresAndAddresses calls_pa = {0};
    redGetCallProceduresAndAddresses(redcontext, redcontext->gpus[0].gpu, &calls_pa, NULL, __FILE__, __LINE__, NULL);
    redCallsSet(redcontext, redcontext->gpus[0].gpu, calls_upload.handle, calls_upload.memory, calls_upload.reusable, NULL, __FILE__, __LINE__, NULL);
    {
      RedCopyArrayRange copyArrayRange = {0};
      copyArrayRange.arrayRBytesFirst = 0;
      copyArrayRange.arrayWBytesFirst = 0;
      copyArrayRange.bytesCount       = sizeof(mesh_data_positions);
      calls_pa.redCallCopyArrayToArray(calls_upload.handle, vertex_buffer_cpu.handle, vertex_buffer_gpu.handle, 1, &copyArrayRange);
    }
    {
      RedCopyArrayRange copyArrayRange = {0};
      copyArrayRange.arrayRBytesFirst = 0;
      copyArrayRange.arrayWBytesFirst = 0;
      copyArrayRange.bytesCount       = sizeof(mesh_data_indices);
      calls_pa.redCallCopyArrayToArray(calls_upload.handle, index_buffer_cpu.handle, index_buffer_gpu.handle, 1, &copyArrayRange);
    }
    redCallsEnd(redcontext, redcontext->gpus[0].gpu, calls_upload.handle, calls_upload.memory, NULL, __FILE__, __LINE__, NULL);
    {
      RedHandleCpuSignal cpu_signal = NULL;
      redCreateCpuSignal(redcontext, redcontext->gpus[0].gpu, NULL, 0, &cpu_signal, NULL, __FILE__, __LINE__, NULL);
      RedGpuTimeline timeline = {0};
      timeline.setTo4                            = 4;
      timeline.setTo0                            = 0;
      timeline.waitForAndUnsignalGpuSignalsCount = 0;
      timeline.waitForAndUnsignalGpuSignals      = NULL;
      timeline.setTo65536                        = NULL;
      timeline.callsCount                        = 1;
      timeline.calls                             = &calls_upload.handle;
      timeline.signalGpuSignalsCount             = 0;
      timeline.signalGpuSignals                  = NULL;
      redQueueSubmit(redcontext, redcontext->gpus[0].gpu, redcontext->gpus[0].queues[0], 1, &timeline, cpu_signal, NULL, __FILE__, __LINE__, NULL);
      redCpuSignalWait(redcontext, redcontext->gpus[0].gpu, 1, &cpu_signal, 1, NULL, __FILE__, __LINE__, NULL);
      redDestroyCpuSignal(redcontext, redcontext->gpus[0].gpu, cpu_signal, __FILE__, __LINE__, NULL);
    }
    redDestroyCalls(redcontext, redcontext->gpus[0].gpu, calls_upload.handle, calls_upload.memory, __FILE__, __LINE__, NULL);
  }

  RRDevicePtr vertex_ptr = NULL;
  RRDevicePtr index_ptr  = NULL;
  RR_CHECK(rrGetDevicePtrFromVkBuffer(context, vertex_buffer_gpu.handle, 0, &vertex_ptr));
  RR_CHECK(rrGetDevicePtrFromVkBuffer(context, index_buffer_gpu.handle,  0, &index_ptr));

  uint32_t triangle_count = (uint32_t)(sizeof(mesh_data_indices)/sizeof(mesh_data_indices[0])) / 3;

  RRGeometryBuildInput    geometry_build_input                = {0};
  RRTriangleMeshPrimitive mesh                                = {0};
  geometry_build_input.triangle_mesh_primitives               = &mesh;
  geometry_build_input.primitive_type                         = RR_PRIMITIVE_TYPE_TRIANGLE_MESH;
  geometry_build_input.triangle_mesh_primitives->vertices     = vertex_ptr;
  geometry_build_input.triangle_mesh_primitives->vertex_count = (uint32_t)((sizeof(mesh_data_positions)/sizeof(mesh_data_positions[0])) / 3);

  geometry_build_input.triangle_mesh_primitives->vertex_stride    = 3 * sizeof(float);
  geometry_build_input.triangle_mesh_primitives->triangle_indices = index_ptr;
  geometry_build_input.triangle_mesh_primitives->triangle_count   = triangle_count;
  geometry_build_input.triangle_mesh_primitives->index_type       = RR_INDEX_TYPE_UINT32;
  geometry_build_input.primitive_count                            = 1u;

  RRBuildOptions options;
  options.build_flags = RR_BUILD_FLAG_BITS_PREFER_FAST_BUILD;

  RRMemoryRequirements geometry_reqs;
  RR_CHECK(rrGetGeometryBuildMemoryRequirements(context, &geometry_build_input, &options, &geometry_reqs));

  RedArray scratch  = {0};
  RedArray geometry = {0};
  RedHandleMemory scratch_memory  = {0};
  RedHandleMemory geometry_memory = {0};
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "scratch",  RED_ARRAY_TYPE_ARRAY_RW, geometry_reqs.temporary_build_buffer_size, 0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &scratch,  NULL, __FILE__, __LINE__, NULL);
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "geometry", RED_ARRAY_TYPE_ARRAY_RW, geometry_reqs.result_buffer_size,          0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &geometry, NULL, __FILE__, __LINE__, NULL);
  redMemoryAllocate(redcontext, redcontext->gpus[0].gpu, "scratch_memory",  scratch.memoryBytesCount,  redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_VRAM, scratch.memoryTypesSupported),  NULL, NULL, 0, &scratch_memory,  NULL, __FILE__, __LINE__, NULL);
  redMemoryAllocate(redcontext, redcontext->gpus[0].gpu, "geometry_memory", geometry.memoryBytesCount, redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_VRAM, geometry.memoryTypesSupported), NULL, NULL, 0, &geometry_memory, NULL, __FILE__, __LINE__, NULL);
  {
    RedMemoryArray memoryArrays[2] = {0};

    memoryArrays[0].setTo1000157000  = 1000157000;
    memoryArrays[0].setTo0           = 0;
    memoryArrays[0].array            = scratch.handle;
    memoryArrays[0].memory           = scratch_memory;
    memoryArrays[0].memoryBytesFirst = 0;

    memoryArrays[1].setTo1000157000  = 1000157000;
    memoryArrays[1].setTo0           = 0;
    memoryArrays[1].array            = geometry.handle;
    memoryArrays[1].memory           = geometry_memory;
    memoryArrays[1].memoryBytesFirst = 0;

    redMemorySet(redcontext, redcontext->gpus[0].gpu, countof(memoryArrays), memoryArrays, 0, NULL, NULL, __FILE__, __LINE__, NULL);
  }

  RRDevicePtr geometry_ptr = NULL;
  RR_CHECK(rrGetDevicePtrFromVkBuffer(context, geometry.handle, 0, &geometry_ptr));

  RRDevicePtr scratch_ptr = NULL;
  RR_CHECK(rrGetDevicePtrFromVkBuffer(context, scratch.handle, 0, &scratch_ptr));

  RedCalls cmd_buf = {0};
  redCreateCallsReusable(redcontext, redcontext->gpus[0].gpu, "cmd_buf", redcontext->gpus[0].queuesFamilyIndex[0], &cmd_buf, NULL, __FILE__, __LINE__, NULL);

  RRCommandStream command_stream = NULL;
  RR_CHECK(rrGetCommandStreamFromVkCommandBuffer(context, cmd_buf.handle, &command_stream));

  redCallsSet(redcontext, redcontext->gpus[0].gpu, cmd_buf.handle, cmd_buf.memory, cmd_buf.reusable, NULL, __FILE__, __LINE__, NULL);
  RR_CHECK(rrCmdBuildGeometry(context, RR_BUILD_OPERATION_BUILD, &geometry_build_input, &options, scratch_ptr, geometry_ptr, command_stream));

  RREvent wait_event = NULL;
  RR_CHECK(rrSumbitCommandStream(context, command_stream, NULL, &wait_event));
  RR_CHECK(rrWaitEvent(context, wait_event));
  RR_CHECK(rrReleaseEvent(context, wait_event));
  RR_CHECK(rrReleaseExternalCommandStream(context, command_stream));

  RRRay rays[WINDOW_W * WINDOW_H] = {0};
  RRHit hits[WINDOW_W * WINDOW_H] = {0};

  for (int y = 0; y < WINDOW_H; y += 1)
  {
    for (int x = 0; x < WINDOW_W; x += 1)
    {
      int i = WINDOW_W * y + x;

      rays[i].origin[0] = 0.f;
      rays[i].origin[1] = 15.f;
      rays[i].origin[2] = 0.f;

      rays[i].direction[0] = -1.f;
      rays[i].direction[1] = -1.f + (2.f / WINDOW_W) * y;
      rays[i].direction[2] = -1.f + (2.f / WINDOW_H) * x;

      rays[i].min_t = 0.001f;
      rays[i].max_t = 100000.f;
    }
  }

  RedArray rays_buffer_cpu = {0};
  RedArray rays_buffer_gpu = {0};
  RedArray hits_buffer_gpu = {0};
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "rays_buffer_cpu", RED_ARRAY_TYPE_ARRAY_RO, sizeof(rays), 0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &rays_buffer_cpu, NULL, __FILE__, __LINE__, NULL);
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "rays_buffer_gpu", RED_ARRAY_TYPE_ARRAY_RW, sizeof(rays), 0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &rays_buffer_gpu, NULL, __FILE__, __LINE__, NULL);
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "hits_buffer_gpu", RED_ARRAY_TYPE_ARRAY_RW, sizeof(hits), 0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &hits_buffer_gpu, NULL, __FILE__, __LINE__, NULL);

  RedHandleMemory rays_buffer_memory_cpu = {0};
  RedHandleMemory rays_buffer_memory_gpu = {0};
  RedHandleMemory hits_buffer_memory_gpu = {0};
  void * rays_buffer_memory_cpu_pointer  = 0;
  redMemoryAllocateMappable(redcontext, redcontext->gpus[0].gpu, "rays_buffer_memory_cpu", 0, rays_buffer_cpu.handle, rays_buffer_cpu.memoryBytesCount, redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_UPLOAD, rays_buffer_cpu.memoryTypesSupported),             0, &rays_buffer_memory_cpu, NULL, __FILE__, __LINE__, NULL);
  redMemoryAllocate        (redcontext, redcontext->gpus[0].gpu, "rays_buffer_memory_gpu",                            rays_buffer_gpu.memoryBytesCount, redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_VRAM,   rays_buffer_gpu.memoryTypesSupported), NULL, NULL, 0, &rays_buffer_memory_gpu, NULL, __FILE__, __LINE__, NULL);
  redMemoryAllocate        (redcontext, redcontext->gpus[0].gpu, "hits_buffer_memory_gpu",                            hits_buffer_gpu.memoryBytesCount, redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_VRAM,   hits_buffer_gpu.memoryTypesSupported), NULL, NULL, 0, &hits_buffer_memory_gpu, NULL, __FILE__, __LINE__, NULL);
  redMemoryMap(redcontext, redcontext->gpus[0].gpu, rays_buffer_memory_cpu, 0, sizeof(rays), &rays_buffer_memory_cpu_pointer, NULL, __FILE__, __LINE__, NULL);

  {
    RedMemoryArray memoryArrays[3] = {0};

    memoryArrays[0].setTo1000157000  = 1000157000;
    memoryArrays[0].setTo0           = 0;
    memoryArrays[0].array            = rays_buffer_cpu.handle;
    memoryArrays[0].memory           = rays_buffer_memory_cpu;
    memoryArrays[0].memoryBytesFirst = 0;

    memoryArrays[1].setTo1000157000  = 1000157000;
    memoryArrays[1].setTo0           = 0;
    memoryArrays[1].array            = rays_buffer_gpu.handle;
    memoryArrays[1].memory           = rays_buffer_memory_gpu;
    memoryArrays[1].memoryBytesFirst = 0;

    memoryArrays[2].setTo1000157000  = 1000157000;
    memoryArrays[2].setTo0           = 0;
    memoryArrays[2].array            = hits_buffer_gpu.handle;
    memoryArrays[2].memory           = hits_buffer_memory_gpu;
    memoryArrays[2].memoryBytesFirst = 0;

    redMemorySet(redcontext, redcontext->gpus[0].gpu, countof(memoryArrays), memoryArrays, 0, NULL, NULL, __FILE__, __LINE__, NULL);
  }

  memcpy(rays_buffer_memory_cpu_pointer, &rays[0], sizeof(rays));

  {
    RedCalls calls_upload = {0};
    redCreateCallsReusable(redcontext, redcontext->gpus[0].gpu, "calls_upload", redcontext->gpus[0].queuesFamilyIndex[0], &calls_upload, NULL, __FILE__, __LINE__, NULL);
    RedCallProceduresAndAddresses calls_pa = {0};
    redGetCallProceduresAndAddresses(redcontext, redcontext->gpus[0].gpu, &calls_pa, NULL, __FILE__, __LINE__, NULL);
    redCallsSet(redcontext, redcontext->gpus[0].gpu, calls_upload.handle, calls_upload.memory, calls_upload.reusable, NULL, __FILE__, __LINE__, NULL);
    {
      RedCopyArrayRange copyArrayRange = {0};
      copyArrayRange.arrayRBytesFirst = 0;
      copyArrayRange.arrayWBytesFirst = 0;
      copyArrayRange.bytesCount       = sizeof(rays);
      calls_pa.redCallCopyArrayToArray(calls_upload.handle, rays_buffer_cpu.handle, rays_buffer_gpu.handle, 1, &copyArrayRange);
    }
    redCallsEnd(redcontext, redcontext->gpus[0].gpu, calls_upload.handle, calls_upload.memory, NULL, __FILE__, __LINE__, NULL);
    {
      RedHandleCpuSignal cpu_signal = NULL;
      redCreateCpuSignal(redcontext, redcontext->gpus[0].gpu, "cpu_signal", 0, &cpu_signal, NULL, __FILE__, __LINE__, NULL);
      RedGpuTimeline timeline = {0};
      timeline.setTo4                            = 4;
      timeline.setTo0                            = 0;
      timeline.waitForAndUnsignalGpuSignalsCount = 0;
      timeline.waitForAndUnsignalGpuSignals      = NULL;
      timeline.setTo65536                        = NULL;
      timeline.callsCount                        = 1;
      timeline.calls                             = &calls_upload.handle;
      timeline.signalGpuSignalsCount             = 0;
      timeline.signalGpuSignals                  = NULL;
      redQueueSubmit(redcontext, redcontext->gpus[0].gpu, redcontext->gpus[0].queues[0], 1, &timeline, cpu_signal, NULL, __FILE__, __LINE__, NULL);
      redCpuSignalWait(redcontext, redcontext->gpus[0].gpu, 1, &cpu_signal, 1, NULL, __FILE__, __LINE__, NULL);
      redDestroyCpuSignal(redcontext, redcontext->gpus[0].gpu, cpu_signal, __FILE__, __LINE__, NULL);
    }
    redDestroyCalls(redcontext, redcontext->gpus[0].gpu, calls_upload.handle, calls_upload.memory, __FILE__, __LINE__, NULL);
  }

  RRDevicePtr rays_ptr = NULL;
  RR_CHECK(rrGetDevicePtrFromVkBuffer(context, rays_buffer_gpu.handle, 0, &rays_ptr));

  RRDevicePtr hits_ptr = NULL;
  RR_CHECK(rrGetDevicePtrFromVkBuffer(context, hits_buffer_gpu.handle, 0, &hits_ptr));

  RedCalls trace_cmd_buf = {0};
  redCreateCallsReusable(redcontext, redcontext->gpus[0].gpu, "trace_cmd_buf", redcontext->gpus[0].queuesFamilyIndex[0], &trace_cmd_buf, NULL, __FILE__, __LINE__, NULL);

  RRCommandStream trace_command_stream = NULL;
  RR_CHECK(rrGetCommandStreamFromVkCommandBuffer(context, trace_cmd_buf.handle, &trace_command_stream));

  size_t scratch_size = 0;
  rrGetTraceMemoryRequirements(context, WINDOW_W * WINDOW_H, &scratch_size);
  RedArray        scratch_buffer_gpu = {0};
  RedHandleMemory scratch_buffer_memory_gpu = {0};
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "scratch_buffer_gpu", RED_ARRAY_TYPE_ARRAY_RW, scratch_size, 0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &scratch_buffer_gpu, NULL, __FILE__, __LINE__, NULL);
  redMemoryAllocate(redcontext, redcontext->gpus[0].gpu, "scratch_buffer_memory_gpu", scratch_buffer_gpu.memoryBytesCount, redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_VRAM, scratch_buffer_gpu.memoryTypesSupported), NULL, NULL, 0, &scratch_buffer_memory_gpu, NULL, __FILE__, __LINE__, NULL);
  {
    RedMemoryArray memoryArrays[1] = {0};

    memoryArrays[0].setTo1000157000  = 1000157000;
    memoryArrays[0].setTo0           = 0;
    memoryArrays[0].array            = scratch_buffer_gpu.handle;
    memoryArrays[0].memory           = scratch_buffer_memory_gpu;
    memoryArrays[0].memoryBytesFirst = 0;

    redMemorySet(redcontext, redcontext->gpus[0].gpu, countof(memoryArrays), memoryArrays, 0, NULL, NULL, __FILE__, __LINE__, NULL);
  }
  RRDevicePtr scratch_trace_ptr = NULL;
  RR_CHECK(rrGetDevicePtrFromVkBuffer(context, scratch_buffer_gpu.handle, 0, &scratch_trace_ptr));

  redCallsSet(redcontext, redcontext->gpus[0].gpu, trace_cmd_buf.handle, trace_cmd_buf.memory, trace_cmd_buf.reusable, NULL, __FILE__, __LINE__, NULL);
  RR_CHECK(rrCmdIntersect(context, geometry_ptr, RR_INTERSECT_QUERY_CLOSEST, rays_ptr, WINDOW_W * WINDOW_H, NULL, RR_INTERSECT_QUERY_OUTPUT_FULL_HIT, hits_ptr, scratch_trace_ptr, trace_command_stream));

  RR_CHECK(rrSumbitCommandStream(context, trace_command_stream, NULL, &wait_event));
  RR_CHECK(rrWaitEvent(context, wait_event));
  RR_CHECK(rrReleaseEvent(context, wait_event));
  RR_CHECK(rrReleaseExternalCommandStream(context, trace_command_stream));

  RedArray        staging_hits_buffer = {0};
  RedHandleMemory staging_hits_memory = {0};
  redCreateArray(redcontext, redcontext->gpus[0].gpu, "staging_hits_buffer", RED_ARRAY_TYPE_ARRAY_RO, hits_buffer_gpu.memoryBytesCount, 0, 0, redcontext->gpus[0].queuesFamilyIndex[0], 0, &staging_hits_buffer, NULL, __FILE__, __LINE__, NULL);
  redMemoryAllocateMappable(redcontext, redcontext->gpus[0].gpu, "staging_hits_memory", 0, staging_hits_buffer.handle, staging_hits_buffer.memoryBytesCount, redHelperGetMemoryTypeIndex(&redcontext->gpus[0], RED_HELPER_MEMORY_TYPE_READBACK, staging_hits_buffer.memoryTypesSupported), 0, &staging_hits_memory, NULL, __FILE__, __LINE__, NULL);
  {
    RedMemoryArray memoryArrays[1] = {0};

    memoryArrays[0].setTo1000157000  = 1000157000;
    memoryArrays[0].setTo0           = 0;
    memoryArrays[0].array            = staging_hits_buffer.handle;
    memoryArrays[0].memory           = staging_hits_memory;
    memoryArrays[0].memoryBytesFirst = 0;

    redMemorySet(redcontext, redcontext->gpus[0].gpu, countof(memoryArrays), memoryArrays, 0, NULL, NULL, __FILE__, __LINE__, NULL);
  }

  RedCalls copy_command_buffer = {0};
  redCreateCalls(redcontext, redcontext->gpus[0].gpu, "copy_command_buffer", redcontext->gpus[0].queuesFamilyIndex[0], &copy_command_buffer, NULL, __FILE__, __LINE__, NULL);

  RedCallProceduresAndAddresses calls_pa = {0};
  redGetCallProceduresAndAddresses(redcontext, redcontext->gpus[0].gpu, &calls_pa, NULL, __FILE__, __LINE__, NULL);

  redCallsSet(redcontext, redcontext->gpus[0].gpu, copy_command_buffer.handle, copy_command_buffer.memory, copy_command_buffer.reusable, NULL, __FILE__, __LINE__, NULL);

  RedCopyArrayRange copyRange = {0};
  copyRange.arrayRBytesFirst  = 0;
  copyRange.arrayWBytesFirst  = 0;
  copyRange.bytesCount        = hits_buffer_gpu.memoryBytesCount;
  calls_pa.redCallCopyArrayToArray(copy_command_buffer.handle, hits_buffer_gpu.handle, staging_hits_buffer.handle, 1, &copyRange);

  RedUsageArray usage = {0};
  usage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
  usage.oldAccessStages        = RED_ACCESS_STAGE_BITFLAG_COPY;
  usage.newAccessStages        = RED_ACCESS_STAGE_BITFLAG_CPU;
  usage.oldAccess              = RED_ACCESS_BITFLAG_COPY_W;
  usage.newAccess              = RED_ACCESS_BITFLAG_CPU_RW;
  usage.queueFamilyIndexSource = -1;
  usage.queueFamilyIndexTarget = -1;
  usage.array                  = staging_hits_buffer.handle;
  usage.arrayBytesFirst        = 0;
  usage.arrayBytesCount        = -1;
  redCallUsageAliasOrderBarrier(calls_pa.redCallUsageAliasOrderBarrier, copy_command_buffer.handle, redcontext, 1, &usage, 0, NULL, 0, NULL, 0, NULL, 0);

  redCallsEnd(redcontext, redcontext->gpus[0].gpu, copy_command_buffer.handle, copy_command_buffer.memory, NULL, __FILE__, __LINE__, NULL);

  {
    RedHandleCpuSignal cpu_signal = NULL;
    redCreateCpuSignal(redcontext, redcontext->gpus[0].gpu, "cpu_signal", 0, &cpu_signal, NULL, __FILE__, __LINE__, NULL);
    RedGpuTimeline timeline = {0};
    timeline.setTo4                            = 4;
    timeline.setTo0                            = 0;
    timeline.waitForAndUnsignalGpuSignalsCount = 0;
    timeline.waitForAndUnsignalGpuSignals      = NULL;
    timeline.setTo65536                        = NULL;
    timeline.callsCount                        = 1;
    timeline.calls                             = &copy_command_buffer.handle;
    timeline.signalGpuSignalsCount             = 0;
    timeline.signalGpuSignals                  = NULL;
    redQueueSubmit(redcontext, redcontext->gpus[0].gpu, redcontext->gpus[0].queues[0], 1, &timeline, cpu_signal, NULL, __FILE__, __LINE__, NULL);
    redCpuSignalWait(redcontext, redcontext->gpus[0].gpu, 1, &cpu_signal, 1, NULL, __FILE__, __LINE__, NULL);
    redDestroyCpuSignal(redcontext, redcontext->gpus[0].gpu, cpu_signal, __FILE__, __LINE__, NULL);
  }

  RRHit * mapped_ptr = NULL;
  redMemoryMap(redcontext, redcontext->gpus[0].gpu, staging_hits_memory, 0, hits_buffer_gpu.memoryBytesCount, (void **)&mapped_ptr, NULL, __FILE__, __LINE__, NULL);

  uint32_t data[WINDOW_W * WINDOW_H] = {0};

  for (int y = 0; y < WINDOW_H; y += 1)
  {
    for (int x = 0; x < WINDOW_W; x += 1)
    {
      int wi = WINDOW_W * (WINDOW_H - 1 - y) + x;
      int i  = WINDOW_W * y + x;

      if (mapped_ptr[i].inst_id != ~0u)
      {
        data[wi] = 0xff000000 | ((uint32_t)(mapped_ptr[i].uv[0] * 255) << 8) | ((uint32_t)(mapped_ptr[i].uv[1] * 255) << 16);
      } else
      {
        data[wi] = 0xff101010;
      }
    }
  }

  stbi_write_jpg("redgpu_radeonrays_test_sponza_geom_isect.jpg", WINDOW_W, WINDOW_H, 4, &data[0], 120);

  RR_CHECK(rrDestroyContext(context));

  redDestroyCalls(redcontext, redcontext->gpus[0].gpu, copy_command_buffer.handle, copy_command_buffer.memory, __FILE__, __LINE__, NULL);
  redDestroyCalls(redcontext, redcontext->gpus[0].gpu, trace_cmd_buf.handle, trace_cmd_buf.memory, __FILE__, __LINE__, NULL);
  redDestroyCalls(redcontext, redcontext->gpus[0].gpu, cmd_buf.handle, cmd_buf.memory, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, geometry.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, scratch.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, index_buffer_gpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, index_buffer_cpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, vertex_buffer_gpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, vertex_buffer_cpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, staging_hits_buffer.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, scratch_buffer_gpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, rays_buffer_cpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, rays_buffer_gpu.handle, __FILE__, __LINE__, NULL);
  redDestroyArray(redcontext, redcontext->gpus[0].gpu, hits_buffer_gpu.handle, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, geometry_memory, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, scratch_memory, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, index_buffer_memory_gpu, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, index_buffer_memory_cpu, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, vertex_buffer_memory_gpu, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, vertex_buffer_memory_cpu, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, staging_hits_memory, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, scratch_buffer_memory_gpu, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, hits_buffer_memory_gpu, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, rays_buffer_memory_gpu, __FILE__, __LINE__, NULL);
  redMemoryFree(redcontext, redcontext->gpus[0].gpu, rays_buffer_memory_cpu, __FILE__, __LINE__, NULL);

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();
  
    int os_window_w = 0;
    int os_window_h = 0;
    glfwGetWindowSize(window, &os_window_w, &os_window_h);

    if (vfWindowIsMinimized(ctx) || os_window_w == 0 || os_window_h == 0) {
      continue;
    }

  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}
