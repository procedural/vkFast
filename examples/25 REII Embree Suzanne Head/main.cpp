#if 0
gcc -c ../../vkfast.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
g++ main.cpp *.o -I"../../extra/REII Embree 1.1.0/include/renderer/" /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm "../../extra/REII Embree 1.1.0/embree/build/librenderer.a" "../../extra/REII Embree 1.1.0/embree/build/librtcore.a" "../../extra/REII Embree 1.1.0/embree/build/libsys.a" "../../extra/REII Embree 1.1.0/embree/build/libimage.a"
exit
#endif

#define M_PI 3.14159265358979323846

#include "../../vkfast.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"
#include "../../extra/REII/vkfast_extra_reii.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI
#include "../Common/vkfast_examples_common.h"

// Embree 1.1.0
#undef min
#undef max
#if defined(_WIN32) && !defined(__MINGW32__)
#include <immintrin.h>
#endif
#if defined(__linux__) || defined(__MINGW32__)
#include <x86intrin.h>
#endif
#include "../../extra/REII Embree 1.1.0/include/sys/platform.h"
#include "../../extra/REII Embree 1.1.0/include/sys/filename.h"
#include "../../extra/REII Embree 1.1.0/include/sys/ref.h"
#include "../../extra/REII Embree 1.1.0/include/lexers/streamfilters.h"
#include "../../extra/REII Embree 1.1.0/include/lexers/parsestream.h"
#include "../../extra/REII Embree 1.1.0/include/loaders/loaders.h"
#include "../../extra/REII Embree 1.1.0/include/math/stlmath.h"
#include "../../extra/REII Embree 1.1.0/include/math/vec2.h"
#include "../../extra/REII Embree 1.1.0/include/math/vec3.h"
#include "../../extra/REII Embree 1.1.0/include/math/vec4.h"
#include "../../extra/REII Embree 1.1.0/include/math/col3.h"
#include "../../extra/REII Embree 1.1.0/include/math/affinespace.h"
#include "../../extra/REII Embree 1.1.0/include/image/image.h"
#include "../../extra/REII Embree 1.1.0/include/renderer/api/device.h"
#include "../../extra/REII Embree 1.1.0/include/renderer/api/handle.h"

// NOTE(Constantine): Paths relative to example's vs2019/ folder:
#ifdef _DEBUG
#pragma comment(lib, "../../../extra/REII Embree 1.1.0/embree/x64/Debug/common.lib")
#pragma comment(lib, "../../../extra/REII Embree 1.1.0/embree/x64/Debug/renderer.lib")
#pragma comment(lib, "../../../extra/REII Embree 1.1.0/embree/x64/Debug/rtcore.lib")
#else
#pragma comment(lib, "../../../extra/REII Embree 1.1.0/embree/x64/Release/common.lib")
#pragma comment(lib, "../../../extra/REII Embree 1.1.0/embree/x64/Release/renderer.lib")
#pragma comment(lib, "../../../extra/REII Embree 1.1.0/embree/x64/Release/rtcore.lib")
#endif

const size_t stack_verticesCount = 2904;
const size_t stack_indicesCount  = 2904;

float stack_verticesPosition[2904*3] = {
  #include "../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
};


float stack_verticesNormal[] = {
  #include "../../extra/3D Mesh Suzanne Head/3d_mesh_normals_suzanne_head.h"
};

int stack_verticesIndex[2904] = {0};

int main() {
  for (int i = 0; i < stack_indicesCount; i += 1) {
    stack_verticesIndex[i] = i;
  }

#if defined(__MINGW32__)
  SetProcessDPIAware();
#elif defined(_WIN32)
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  #if defined(_WIN32)
  LARGE_INTEGER frequency = {0};
  REDGPU_2_EXPECTFL(QueryPerformanceFrequency(&frequency) == TRUE); // Query the frequency (ticks per second)
  #endif

  int window_w = 700;
  int window_h = 700;
  int embree_mode = 0;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] REII Embree Suzanne Head", 0, 0);
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
  memory_allocation_sizes.bytes_count_for_memory_storages_type_gpu_only         = (1024/*mb*/ * 1024 * 1024) - 64; // NOTE(Constantine)(Mar 20, 2026): '- 64' added for Intel iGPUs which can allocate not 1073741824, but 1073741820 max, lol.
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_upload       = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_readback     = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_present_pixels_type_cpu_upload = 0;
  gpu_context_optional_parameters_t optional_parameters = {0};
  optional_parameters.internal_memory_allocation_sizes = &memory_allocation_sizes;

  const int doDoubleGammaCorrection = 0;
  // NOTE(Constantine): You can also define REDGPU_COMPILE_SWITCH_DEBUG to see extra errors.
  gpu_handle_context_t ctx = vfContextInit(1, &optional_parameters, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] REII Embree Suzanne Head", window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  gpu_storage_t storage_gpu_only     = {0};
  gpu_storage_t storage_cpu_upload   = {0};
  gpu_storage_t storage_cpu_readback = {0};
  vfeBanzaiStoragesCreate(ctx, &storage_gpu_only, &storage_cpu_upload, &storage_cpu_readback, FF, LL);

  uint64_t storage_gpu_only_mem_offset = 0;
  uint64_t storage_cpu_upload_mem_offset = 0;
  uint64_t storage_cpu_readback_mem_offset = 0;

  gpu_extra_cpu_gpu_array pixels = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_PRESENT_PIXELS_CPU_UPLOAD_288MB,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  gpu_extra_cpu_gpu_array pos_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    64/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );
  gpu_extra_cpu_gpu_array col_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    64/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  #include "mesh.vs.h"
  #include "mesh.fs.h"
  gpu_program_info_t vp = {0};
  vp.program_binary_bytes_count = sizeof(g_main_vs);
  vp.program_binary             = g_main_vs;
  gpu_program_info_t fp = {0};
  fp.program_binary_bytes_count = sizeof(g_main_fs);
  fp.program_binary             = g_main_fs;
  char * vp_string = NULL;
  char * fp_string = NULL;

  ReiiMeshState mesh_state                                  = {0};
  mesh_state.rasterizationDepthClampEnable                  = 0;
  mesh_state.rasterizationCullMode                          = REII_CULL_MODE_NONE;
  mesh_state.rasterizationFrontFace                         = REII_FRONT_FACE_COUNTER_CLOCKWISE;
  mesh_state.rasterizationDepthBiasEnable                   = 0;
  mesh_state.rasterizationDepthBiasConstantFactor           = 0;
  mesh_state.rasterizationDepthBiasSlopeFactor              = 0;
  mesh_state.multisampleEnable                              = 1;
  mesh_state.multisampleAlphaToCoverageEnable               = 0;
  mesh_state.multisampleAlphaToOneEnable                    = 0;
  mesh_state.depthTestEnable                                = 1;
  mesh_state.depthTestDepthWriteEnable                      = 1;
  mesh_state.depthTestDepthCompareOp                        = REII_COMPARE_OP_GREATER_OR_EQUAL;
  mesh_state.stencilTestEnable                              = 0;
  mesh_state.stencilTestFrontStencilTestFailOp              = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestFrontStencilTestPassDepthTestPassOp = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestFrontStencilTestPassDepthTestFailOp = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestFrontCompareOp                      = REII_COMPARE_OP_NEVER;
  mesh_state.stencilTestBackStencilTestFailOp               = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestBackStencilTestPassDepthTestPassOp  = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestBackStencilTestPassDepthTestFailOp  = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestBackCompareOp                       = REII_COMPARE_OP_NEVER;
  mesh_state.stencilTestFrontAndBackCompareMask             = 0;
  mesh_state.stencilTestFrontAndBackWriteMask               = 0;
  mesh_state.stencilTestFrontAndBackReference               = 0;
  mesh_state.blendLogicOpEnable                             = 0;
  mesh_state.blendLogicOp                                   = REII_LOGIC_OP_CLEAR;
  mesh_state.blendConstants[0]                              = 0;
  mesh_state.blendConstants[1]                              = 0;
  mesh_state.blendConstants[2]                              = 0;
  mesh_state.blendConstants[3]                              = 0;
  mesh_state.outputColorWriteEnableR                        = 1;
  mesh_state.outputColorWriteEnableG                        = 1;
  mesh_state.outputColorWriteEnableB                        = 1;
  mesh_state.outputColorWriteEnableA                        = 1;
  mesh_state.outputColorBlendEnable                         = 0;
  mesh_state.outputColorBlendColorFactorSource              = REII_BLEND_FACTOR_ZERO;
  mesh_state.outputColorBlendColorFactorTarget              = REII_BLEND_FACTOR_ZERO;
  mesh_state.outputColorBlendColorOp                        = REII_BLEND_OP_ADD;
  mesh_state.outputColorBlendAlphaFactorSource              = REII_BLEND_FACTOR_ZERO;
  mesh_state.outputColorBlendAlphaFactorTarget              = REII_BLEND_FACTOR_ZERO;
  mesh_state.outputColorBlendAlphaOp                        = REII_BLEND_OP_ADD;
  mesh_state.codeVertex                                     = vp_string;
  mesh_state.codeFragment                                   = fp_string;
  mesh_state.extension                                      = NULL;
  mesh_state.programVertex                                  = vp;
  mesh_state.programFragment                                = fp;
  mesh_state.compileInfo.state_multisample_count            = RED_MULTISAMPLE_COUNT_BITFLAG_4;
  mesh_state.compileInfo.output_depth_stencil_enable        = 1;
  mesh_state.compileInfo.output_depth_stencil_format        = RED_FORMAT_DEPTH_32_FLOAT;
  mesh_state.compileInfo.output_color_format                = RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1;

  RedStructDeclarationMember slots[2] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  slots[1].slot            = 1;
  slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[1].count           = 1;
  slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;

  mesh_state.programPipelineInfo.variables_slot        = 2;
  mesh_state.programPipelineInfo.variables_bytes_count = (2 * sizeof(ReiiVec4)) + (2 * sizeof(float));
  mesh_state.programPipelineInfo.struct_members_count  = countof(slots);
  mesh_state.programPipelineInfo.struct_members        = slots;
  mesh_state.programPipelineInfoSamplersCount          = 0;

  reiiMeshStateCompile(ctx, &mesh_state);

  ReiiHandleTextureMemory outputDSTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA, (288/*mb*/ * 1024 * 1024), &outputDSTexMemory);
  ReiiHandleTexture houtputdstex = {0};
  ReiiHandleTexture * outputdstex = &houtputdstex;
  reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
  reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputdstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL, 1, &gpu_thread, array65536);

  ReiiHandleTextureMemory outputTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR, (288/*mb*/ * 1024 * 1024), &outputTexMemory);
  ReiiHandleTexture houtputtex = {0};
  ReiiHandleTexture * outputtex = &houtputtex;
  reiiCreateTextureFromTextureMemory(ctx, &outputTexMemory, REII_TEXTURE_BINDING_2D, &houtputtex);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputtex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

  ReiiHandleTextureMemory outputMSTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR_MSAA, (288/*mb*/ * 1024 * 1024), &outputMSTexMemory);
  ReiiHandleTexture houtputmstex = {0};
  ReiiHandleTexture * outputmstex = &houtputmstex;
  reiiCreateTextureFromTextureMemory(ctx, &outputMSTexMemory, REII_TEXTURE_BINDING_2D, &houtputmstex);
  reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputmstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

  uint64_t batch = 0;
  ReiiHandleCommandList hlist = {0};
  ReiiHandleCommandList * list = &hlist;
  Red2Output mutable_outputs_array[3]  = {0};
  list->mutable_outputs_array.items    = mutable_outputs_array;
  list->mutable_outputs_array.capacity = countof(mutable_outputs_array);
  list->dynamic_mesh_position          = pos_array;
  list->dynamic_mesh_color             = col_array;

  ReiiGammaCorrectColorTextureToTheInversePowerOf2StaticState gammaCorrectionStaticState = {0};

  ReiiVec4   camera_pos  = {0, 0, -5.f};
  ReiiVec4   camera_quat = {0, 0, 0, 1};
  ReiiBool32 camera_is_enabled = 1;
  if (camera_is_enabled == 1) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  glfwPollEvents();
  double mouse_x = 0;
  double mouse_y = 0;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  double mouse_x_prev = mouse_x;
  double mouse_y_prev = mouse_y;
  int    mouse_right_mouse_button_state_prev = 0;

  int previous_window_w = window_w;
  int previous_window_h = window_h;

  // Embree
  embree::Device * device;
  embree::Device::RTRenderer renderer;
  embree::Device::RTToneMapper tonemapper;
  embree::Device::RTFrameBuffer framebuffer;
  embree::Device::RTCamera camera;
  embree::Device::RTLight light;
  embree::Device::RTPrimitive lightprimitive;
  embree::Device::RTMaterial material;
  embree::Device::RTShape shape;
  embree::Device::RTData positions;
  embree::Device::RTData normals;
  embree::Device::RTData indices;
  embree::Device::RTPrimitive primitive;
  embree::Device::RTPrimitive primitives[2];
  embree::Device::RTScene scene;
  {
    const int numberOfThreads = 8;
    device = embree::Device::rtCreateDevice("default", numberOfThreads);

    renderer = device->rtNewRenderer("pathtracer");
    device->rtSetInt1(renderer, "maxDepth", 20);
    device->rtSetInt1(renderer, "sampler.spp", 1);
    device->rtSetFloat1(renderer, "clampRadianceIfMoreThan", 10.0);
    device->rtSetFloat1(renderer, "clampRadianceTo", 5.0);
    device->rtSetInt1(renderer, "printstats", 0);
    device->rtCommit(renderer);

    tonemapper = device->rtNewToneMapper("default");
    device->rtSetFloat1(tonemapper, "gamma", 1.0);
    device->rtCommit(tonemapper);

    framebuffer = device->rtNewFrameBuffer("RGB_FLOAT32", window_w, window_h, 1);

    camera = device->rtNewCamera("depthoffield");

    light = device->rtNewLight("trianglelight");
    device->rtSetFloat3(light, "v0", 0, 0, -10);
    device->rtSetFloat3(light, "v1", 0, 100.0, -10);
    device->rtSetFloat3(light, "v2", 100.0, 0, -10);
    device->rtSetFloat3(light, "L",  1.0, 1.0, 1.0);
    device->rtCommit(light);
    lightprimitive = device->rtNewLightPrimitive(light, NULL);

    material = device->rtNewMaterial("Matte");
    device->rtSetFloat3(material, "reflectance", 1.0, 1.0, 1.0);
    device->rtCommit(material);

    //material = device->rtNewMaterial("Velvet");
    //device->rtSetFloat3(material, "reflectance", 0.4, 0, 0);
    //device->rtSetFloat1(material, "backScattering", 0.5f);
    //device->rtSetFloat3(material, "horizonScatteringColor", 0.75, 0.1, 0.1);
    //device->rtSetFloat1(material, "horizonScatteringFallOff", 10.0f);
    //device->rtCommit(material);

    shape = device->rtNewShape("trianglemesh");
    positions = device->rtNewData("immutable", stack_verticesCount * (3 * 4), stack_verticesPosition);
    normals = device->rtNewData("immutable", stack_verticesCount * (3 * 4), stack_verticesNormal);
    indices = device->rtNewData("immutable", stack_indicesCount * 4, stack_verticesIndex);
    device->rtSetArray(shape, "positions", "float3", positions, stack_verticesCount, 3 * 4, 0);
    device->rtSetArray(shape, "normals", "float3", normals, stack_verticesCount, 3 * 4, 0);
    device->rtSetArray(shape, "indices", "int3", indices, stack_indicesCount / 3, 3 * 4, 0);
    device->rtCommit(shape);
    device->rtClear(shape);

    float * transform = NULL; // AffineSpace3f xform; transform = copyToArray(xform);
    primitive = device->rtNewShapePrimitive(shape, material, transform);

    primitives[0] = primitive;
    primitives[1] = lightprimitive;

    scene = device->rtNewScene(/*accel*/"default" " " /*tri*/"default", primitives, countof(primitives));
  }

  // Embree
  int   embree_frame      = 0;
  float embree_dir_vec[3] = {0, 0, 0};

  // Embree-related shader to convert RGB_FLOAT32 to BGRA8
  RedStructDeclarationMember pp_slots[2] = {0};
  uint64_t cs = 0;
  uint64_t pp = 0;
  {
    #include "embree_rgb_float32_to_bgra8.cs.h"
    gpu_program_info_t cs_info = {0};
    cs_info.program_binary_bytes_count = sizeof(g_main);
    cs_info.program_binary             = g_main;
    cs = vfProgramCreateFromBinaryCompute(ctx, &cs_info, FF, LL);

    pp_slots[0].slot            = 0;
    pp_slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    pp_slots[0].count           = 1;
    pp_slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;

    pp_slots[1].slot            = 1;
    pp_slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    pp_slots[1].count           = 1;
    pp_slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
    gpu_program_pipeline_info_t pp_info = {0};
    pp_info.variables_slot        = 2;
    pp_info.variables_bytes_count = 2 * sizeof(float);
    pp_info.struct_members_count  = countof(pp_slots);
    pp_info.struct_members        = pp_slots;
    pp = vfProgramPipelineCreateCompute(ctx, cs, &pp_info, NULL, FF, LL);
  }

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();
  
    int os_window_w = 0;
    int os_window_h = 0;
    glfwGetWindowSize(window, &os_window_w, &os_window_h);

    if (vfWindowIsMinimized(ctx) || os_window_w == 0 || os_window_h == 0) {
      continue;
    }

    #if defined(_WIN32)
    LARGE_INTEGER t_start = {0};
    QueryPerformanceCounter(&t_start);
    #endif

    {
      vfWindowGetSize(ctx, &window_w, &window_h);

      if (window_w != previous_window_w || window_h != previous_window_h) {
        // Recreate output textures then.

        vfAllQueuesWaitIdle(ctx, FF, LL);

        vfGpuThreadDestroy(ctx, gpu_thread);
        gpu_thread = NULL;
        vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

        reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputmstex);
        reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputtex);
        reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputdstex);

        reiiResetTextureMemory(ctx, &outputMSTexMemory);
        reiiResetTextureMemory(ctx, &outputTexMemory);
        reiiResetTextureMemory(ctx, &outputDSTexMemory);

        reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
        reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputdstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL, 1, &gpu_thread, array65536);

        reiiCreateTextureFromTextureMemory(ctx, &outputTexMemory, REII_TEXTURE_BINDING_2D, &houtputtex);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputtex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

        reiiCreateTextureFromTextureMemory(ctx, &outputMSTexMemory, REII_TEXTURE_BINDING_2D, &houtputmstex);
        reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputmstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

        device->rtDecRef(framebuffer);
        framebuffer = device->rtNewFrameBuffer("RGB_FLOAT32", window_w, window_h, 1);
      }

      previous_window_w = window_w;
      previous_window_h = window_h;
    }

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    int mouse_right_mouse_button_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);

    // NOTE(Constantine):
    // Camera quaternion rotation and translation.
    const float mouse_move_sensitivity = 0.0035f;
    const float camera_move_speed      = 0.1f;
    if (mouse_right_mouse_button_state == GLFW_PRESS && mouse_right_mouse_button_state != mouse_right_mouse_button_state_prev) {
      camera_is_enabled = !camera_is_enabled;
      glfwSetInputMode(window, GLFW_CURSOR, camera_is_enabled == 1 ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    } else if (camera_is_enabled == 1) {
      float mouse_move_x = (float)(mouse_x - mouse_x_prev) * mouse_move_sensitivity;
      float mouse_move_y = (float)(mouse_y - mouse_y_prev) * mouse_move_sensitivity;

      float key_f = glfwGetKey(window, GLFW_KEY_W);
      float key_b = glfwGetKey(window, GLFW_KEY_S);

      float key_r = glfwGetKey(window, GLFW_KEY_D);
      float key_l = glfwGetKey(window, GLFW_KEY_A);

      float key_u = glfwGetKey(window, GLFW_KEY_E);
      float key_d = glfwGetKey(window, GLFW_KEY_Q);

      // Embree
      if (mouse_move_x || mouse_move_y || key_f || key_b || key_r || key_l || key_u || key_d) {
        embree_frame = 0;
      }

      float rot_x[4];
      float rot_y[4];
  
      float axis_x[3] = {1, 0, 0};
      float axis_y[3] = {0, 1, 0};
      quatFromAxisAngle(rot_y, axis_y, mouse_move_x);
      quatFromAxisAngle(rot_x, axis_x, mouse_move_y);
  
      quatMul(&camera_quat.x, &camera_quat.x, rot_x);
      quatMul(&camera_quat.x, rot_y, &camera_quat.x);

      float side_vec[3] = {1, 0, 0};
      float   up_vec[3] = {0, 1, 0};
      float  dir_vec[3] = {0, 0, 1};
      quatRotateVec3Fast(side_vec, side_vec, &camera_quat.x);
      quatRotateVec3Fast(  up_vec,   up_vec, &camera_quat.x);
      quatRotateVec3Fast( dir_vec,  dir_vec, &camera_quat.x);

      embree_dir_vec[0] = dir_vec[0];
      embree_dir_vec[1] = dir_vec[1];
      embree_dir_vec[2] = dir_vec[2];

      vec3Mulf(side_vec, side_vec, key_r - key_l);
      vec3Mulf(  up_vec,   up_vec, key_u - key_d);
      vec3Mulf( dir_vec,  dir_vec, key_f - key_b);
  
      float move_vec_normalized[3] = {0, 0, 0};

      vec3Add(move_vec_normalized, move_vec_normalized, side_vec);
      vec3Add(move_vec_normalized, move_vec_normalized,   up_vec);
      vec3Add(move_vec_normalized, move_vec_normalized,  dir_vec);

      float move_vec_len = sqrtf(
        move_vec_normalized[0] * move_vec_normalized[0] +
        move_vec_normalized[1] * move_vec_normalized[1] +
        move_vec_normalized[2] * move_vec_normalized[2]
      );
      if (move_vec_len != 0) {
        move_vec_normalized[0] /= move_vec_len;
        move_vec_normalized[1] /= move_vec_len;
        move_vec_normalized[2] /= move_vec_len;
      }

      vec3Mulf(move_vec_normalized, move_vec_normalized, camera_move_speed);

      vec3Add(&camera_pos.x, &camera_pos.x, move_vec_normalized);
    }

    // FOV
    double fov_deg = 90.0;
    float fov_x = 0.f;
    float fov_y = 0.f;
    {
      double deg_to_rad = fov_deg * (M_PI / 180.0);
      double fov = 1.0 / tan(deg_to_rad / 2.0);
      double aspect_ratio = window_w / (double)window_h;
      fov_x = fov / aspect_ratio;
      fov_y = fov;
    }

    // Embree
    {
      embree::AffineSpace3f camera_matrix = embree::AffineSpace3f::lookAtPoint(
        /*pos*/    embree::Vec3f(camera_pos.x, camera_pos.y, camera_pos.z),
        /*target*/ embree::Vec3f(camera_pos.x+embree_dir_vec[0], camera_pos.y+embree_dir_vec[1], camera_pos.z+embree_dir_vec[2]),
        /*up*/     embree::Vec3f(0, 1, 0)
      );
      device->rtSetTransform(camera, "local2world", embree::copyToArray(camera_matrix));
      device->rtSetFloat1(camera, "angle", fov_deg);
      device->rtSetFloat1(camera, "aspectRatio", window_w / (float)window_h);
      device->rtSetFloat1(camera, "focalDistance", 10);
      device->rtSetFloat1(camera, "lensRadius", 0.001);
      device->rtCommit(camera);
    }

    if (camera_is_enabled == 1) {
      gpu_batch_info_t bindings_info = {0};
      bindings_info.max_new_bindings_sets_count = 2;
      bindings_info.max_storage_binds_count     = 2;
      bindings_info.max_texture_rw_binds_count  = 1;
      batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
      list->batch_id = batch;
      reiiCommandListReset(ctx, list);
      reiiCommandSetViewportEx(ctx, list, 0, 0, window_w, window_h, 0, 1);
      reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);
      reiiCommandClearTexture(ctx, list, outputdstex, outputmstex, outputmstex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f,0.f,0.05f,1.f);
      reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
      reiiCommandBindNewBindingsSet(ctx, list, countof(slots), slots);
      reiiCommandBindStorageRaw(ctx, list, 0, 1, &pos_array.gpu);
      reiiCommandBindStorageRaw(ctx, list, 1, 1, &col_array.gpu);
      reiiCommandBindNewBindingsEnd(ctx, list);
      reiiCommandBindVariablesCopy(ctx, list, 0 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_pos);
      reiiCommandBindVariablesCopy(ctx, list, 1 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_quat);
      float fov_xy[2] = {fov_x, fov_y};
      reiiCommandBindVariablesCopy(ctx, list, 2 * sizeof(ReiiVec4), sizeof(fov_xy), fov_xy);
      reiiCommandMeshSet(ctx, list);
      for (int i = 0; i < stack_verticesCount; i += 1) {
        reiiCommandMeshColor(ctx, list, i * 0.00025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          stack_verticesPosition[i * 3 + 0],
          stack_verticesPosition[i * 3 + 1],
          stack_verticesPosition[i * 3 + 2],
          1
        );
      }
      reiiCommandMeshEndWithTale64BytesAlign(ctx, list, outputdstex, outputmstex, outputmstex->texture);
      reiiCommandResolveMsaaColorTexture(ctx, list, outputmstex, outputtex);
      vfBatchBarrierMemory(ctx, batch, FF, LL);
      reiiCommandGammaCorrectColorTextureToTheInversePowerOf2(ctx, list, outputtex, doDoubleGammaCorrection, 1, &gammaCorrectionStaticState);
      vfBatchEnd(ctx, batch, FF, LL);

      RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
      uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
      vfAsyncWaitToFinish(ctx, wait, FF, LL);

      gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
      vfAsyncDrawImageRaw(ctx, outputtex->image.handle, NULL, 2, gpu_threads, array65536, FF, LL);
    } else {
      device->rtRenderFrame(renderer, camera, scene, tonemapper, framebuffer, embree_frame);
      embree_frame += 1;
      device->rtSwapBuffers(framebuffer);

      void * ptr = device->rtMapFrameBuffer(framebuffer);
      memcpy(pixels.cpu_ptr, ptr, sizeof(float) * 4 * window_w * window_h);
      device->rtUnmapFrameBuffer(framebuffer);
      ptr = NULL;

      gpu_batch_info_t bindings_info = {0};
      bindings_info.max_new_bindings_sets_count = 1;
      bindings_info.max_storage_binds_count     = 2;
      batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
      vfBatchBindProgramPipelineCompute(ctx, batch, pp, FF, LL);
      vfBatchBindNewBindingsSet(ctx, batch, countof(pp_slots), pp_slots, FF, LL);
      vfBatchBindStorageRaw(ctx, batch, 0, 1, &pixels.cpu, FF, LL);
      vfBatchBindStorageRaw(ctx, batch, 1, 1, &pixels.gpu, FF, LL);
      vfBatchBindNewBindingsEnd(ctx, batch, FF, LL);
      float window_wh[2] = {(float)window_w, (float)window_h};
      vfBatchBindVariablesCopy(ctx, batch, 0, sizeof(window_wh), window_wh, FF, LL);
      vfBatchCompute(ctx, batch, ((window_w * window_h) / 32) + 1, 1, 1, FF, LL);
      vfBatchEnd(ctx, batch, FF, LL);
      RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
      uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
      vfAsyncWaitToFinish(ctx, wait, FF, LL);

      gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
      vfAsyncDrawPixelsRaw(ctx, &pixels.gpu, NULL, 2, gpu_threads, array65536, FF, LL);
    }

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;

    #if defined(_WIN32)
    LARGE_INTEGER t_end = {0};
    QueryPerformanceCounter(&t_end);
    #endif

    #if defined(_WIN32)
    {
      LONGLONG elapsedTicks = t_end.QuadPart - t_start.QuadPart;
      LONGLONG nanoseconds = (elapsedTicks * 1000000000LL) / frequency.QuadPart;
      double milliseconds_fp = (double)(nanoseconds) / 1000000.0;
      //printf("Elapsed milliseconds: %f\n", milliseconds_fp);
    }
    #endif
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);

  device->rtDecRef(scene);
  device->rtDecRef(primitive);
  device->rtDecRef(indices);
  device->rtDecRef(normals);
  device->rtDecRef(positions);
  device->rtDecRef(shape);
  device->rtDecRef(material);
  device->rtDecRef(lightprimitive);
  device->rtDecRef(light);
  device->rtDecRef(camera);
  device->rtDecRef(framebuffer);
  device->rtDecRef(tonemapper);
  device->rtDecRef(renderer);
  delete device;
  device = NULL;

  vfIdDestroy(1, &gammaCorrectionStaticState.programPipeline, FF, LL);
  vfIdDestroy(1, &gammaCorrectionStaticState.programCompute, FF, LL);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_COMMAND_LIST, list);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputmstex);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputtex);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputdstex);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &outputMSTexMemory);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &outputTexMemory);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &outputDSTexMemory);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_MESH_STATE, &mesh_state);
  uint64_t ids[] = {
    batch,
    storage_gpu_only.id,
    storage_cpu_upload.id,
    storage_cpu_readback.id,
    pp,
    cs,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}
