#include "../../vkfast.h"
#include "../../vkfast_ex.h"
#include "../../vkfast_ids.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"
#include "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_DO_NOT_LINK_GLFW3
#include "../Common/vkfast_examples_common.h"
#pragma comment(lib, "../../../Common/glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib") // NOTE(Constantine): Path relative to example's cmake/build/ folder.

#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"

#include "../../extra/Ray/Ray.h"

extern void * VKFAST_EXTRA_RAY_HACK_VK_INSTANCE;
extern void * VKFAST_EXTRA_RAY_HACK_VK_PHYSICAL_DEVICE;
extern void * VKFAST_EXTRA_RAY_HACK_VK_DEVICE;
extern void * VKFAST_EXTRA_RAY_HACK_QUEUE_SUBMIT_VK_SEMAPHORE;

std::vector<uint8_t> GenerateCheckerboard(const int res, const int square_size) {
  std::vector<uint8_t> ret(4 * res * res);

  for (int i = 0; i < res; i++) {
    for (int j = 0; j < res; j++) {
      const int index = i * res + j;
      const int square_x = j / square_size;
      const int square_y = i / square_size;

      if ((square_x + square_y) % 2 == 0) {
        ret[4 * index + 0] = 10;
        ret[4 * index + 1] = 10;
        ret[4 * index + 2] = 10;
        ret[4 * index + 3] = 255;
      } else {
        ret[4 * index + 0] = 250;
        ret[4 * index + 1] = 250;
        ret[4 * index + 2] = 250;
        ret[4 * index + 3] = 255;
      }
    }
  }

  return ret;
}

int main() {
#if defined(__MINGW32__)
  SetProcessDPIAware();
#elif defined(_WIN32)
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  const int window_w = 700;
  const int window_h = 700;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] Ray GPU Presents", NULL, NULL);
  void * window_handle = (void *)glfwGetWin32Window(window);

  // Ray

  // Initial frame resolution, can be changed later
  Ray::settings_t s;
  s.w = window_w;
  s.h = window_h;

  // Additional Ray::eRendererType parameter can be passed (Vulkan GPU renderer created by default)
  Ray::RendererBase * renderer = Ray::CreateRenderer(s, &Ray::g_stdout_log);

  gpu_internal_memory_allocation_sizes_t memory_allocation_sizes = {0};
  memory_allocation_sizes.bytes_count_for_memory_storages_type_gpu_only         = (1024/*mb*/ * 1024 * 1024) - 64; // NOTE(Constantine)(Mar 20, 2026): '- 64' added for Intel iGPUs which can allocate not 1073741824, but 1073741820 max, lol.
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_upload       = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_readback     = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_present_pixels_type_cpu_upload = 0;
  gpu_context_optional_parameters_t optional_parameters = {0};
  optional_parameters.internal_memory_allocation_sizes = &memory_allocation_sizes;

  gpu_context_ex2_parameters_t ex2_parameters = {0};
  ex2_parameters.external_VkInstance       = (uint64_t)VKFAST_EXTRA_RAY_HACK_VK_INSTANCE;
  ex2_parameters.external_VkPhysicalDevice = (uint64_t)VKFAST_EXTRA_RAY_HACK_VK_PHYSICAL_DEVICE;
  ex2_parameters.external_VkDevice         = (uint64_t)VKFAST_EXTRA_RAY_HACK_VK_DEVICE;
  ex2_parameters.exposeOnlyOneGpu          = 1;
  ex2_parameters.exposeOnlyOneQueue        = 1;
        
  gpu_handle_context_t ctx = vfContextInitEx2(1, 0, &optional_parameters, &ex2_parameters, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] Ray GPU Presents", 700, 700, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[3] = {65536, 65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  gpu_storage_t storage_gpu_only     = {0};
  gpu_storage_t storage_cpu_upload   = {0};
  gpu_storage_t storage_cpu_readback = {0};
  vfeBanzaiStoragesCreate(ctx, &storage_gpu_only, &storage_cpu_upload, &storage_cpu_readback, FF, LL);

  uint64_t storage_gpu_only_mem_offset = 0;
  uint64_t storage_cpu_upload_mem_offset = 0;
  uint64_t storage_cpu_readback_mem_offset = 0;

  gpu_extra_cpu_gpu_array gpu_pixels = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_PRESENT_PIXELS_CPU_UPLOAD_288MB,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  // Ray

  // Each renderer has its own storage implementation (RAM, GPU-RAM),
  // so renderer itself should create scene object
  Ray::SceneBase * scene = renderer->CreateScene();

  // Setup environment
  Ray::environment_desc_t env_desc;
  env_desc.env_col[0] = 0.0f;
  env_desc.env_col[1] = 0.0f;
  env_desc.env_col[2] = 0.0f;
  scene->SetEnvironment(env_desc);

  // Add checker texture
  const std::vector<uint8_t> tex_data = GenerateCheckerboard(128, 16);
  Ray::tex_desc_t tex_desc;
  tex_desc.format = Ray::eTextureFormat::RGBA8888;
  tex_desc.w = 128;
  tex_desc.h = 128;
  tex_desc.generate_mipmaps = false;
  tex_desc.data = tex_data;

  Ray::TextureHandle checker_tex = scene->AddTexture(tex_desc);

  // Add principled material
  Ray::principled_mat_desc_t mat_desc1;
  mat_desc1.base_texture = checker_tex;
  mat_desc1.roughness = 0.25f;
  mat_desc1.roughness_texture = checker_tex;
  const Ray::MaterialHandle mat0 = scene->AddMaterial(mat_desc1);

  Ray::shading_node_desc_t mat_desc2;
  mat_desc2.type = Ray::eShadingNode::Diffuse;
  mat_desc2.base_color[0] = 0.5f;
  mat_desc2.base_color[1] = 0.5f;
  mat_desc2.base_color[2] = 0.5f;
  const Ray::MaterialHandle mat1 = scene->AddMaterial(mat_desc2);

  mat_desc2.base_color[0] = 0.5f;
  mat_desc2.base_color[1] = 0.0f;
  mat_desc2.base_color[2] = 0.0f;
  const Ray::MaterialHandle mat2 = scene->AddMaterial(mat_desc2);

  mat_desc2.base_color[0] = 0.0f;
  mat_desc2.base_color[1] = 0.5f;
  mat_desc2.base_color[2] = 0.0f;
  const Ray::MaterialHandle mat3 = scene->AddMaterial(mat_desc2);

  // Add emissive material
  Ray::shading_node_desc_t mat_desc3;
  mat_desc3.type = Ray::eShadingNode::Emissive;
  mat_desc3.strength = 100.0f;
  mat_desc3.importance_sample = true; // Use NEE for this lightsource
  const Ray::MaterialHandle mat4 = scene->AddMaterial(mat_desc3);

  // Setup test mesh
  // position(3 floats), normal(3 floats), tex_coord(2 floats)
  const float attrs[] = {
    // floor
    0.0f, 0.0f, -0.5592f,           0.0f, 1.0f, 0.0f,                   1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,               0.0f, 1.0f, 0.0f,                   1.0f, 0.0f,
    -0.5528f, 0.0f, 0.0f,           0.0f, 1.0f, 0.0f,                   0.0f, 0.0f,
    -0.5496f, 0.0f, -0.5592f,       0.0f, 1.0f, 0.0f,                   0.0f, 1.0f,
    // back wall
    0.0f, 0.0f, -0.5592f,           0.0f, 0.0f, 1.0f,                   0.0f, 0.0f,
    -0.5496f, 0.0f, -0.5592f,       0.0f, 0.0f, 1.0f,                   0.0f, 0.0f,
    -0.556f, 0.5488f, -0.5592f,     0.0f, 0.0f, 1.0f,                   0.0f, 0.0f,
    0.0f, 0.5488f, -0.5592f,        0.0f, 0.0f, 1.0f,                   0.0f, 0.0f,
    // ceiling
    -0.556f, 0.5488f, -0.5592f,     0.0f, -1.0f, 0.0f,                  0.0f, 0.0f,
    0.0f, 0.5488f, -0.5592f,        0.0f, -1.0f, 0.0f,                  0.0f, 0.0f,
    0.0f, 0.5488f, 0.0f,            0.0f, -1.0f, 0.0f,                  0.0f, 0.0f,
    -0.556f, 0.5488f, 0.0f,         0.0f, -1.0f, 0.0f,                  0.0f, 0.0f,
    // left wall
    -0.5528f, 0.0f, 0.0f,           1.0f, 0.0f, 0.0f,                   0.0f, 0.0f,
    -0.5496f, 0.0f, -0.5592f,       1.0f, 0.0f, 0.0f,                   0.0f, 0.0f,
    -0.556f, 0.5488f, 0.0f,         1.0f, 0.0f, 0.0f,                   0.0f, 0.0f,
    -0.556f, 0.5488f, -0.5592f,     1.0f, 0.0f, 0.0f,                   0.0f, 0.0f,
    // right wall
    0.0f, 0.0f, -0.5592f,           -1.0f, 0.0f, 0.0f,                  0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,               -1.0f, 0.0f, 0.0f,                  0.0f, 0.0f,
    0.0f, 0.5488f, -0.5592f,        -1.0f, 0.0f, 0.0f,                  0.0f, 0.0f,
    0.0f, 0.5488f, 0.0f,            -1.0f, 0.0f, 0.0f,                  0.0f, 0.0f,
    // light
    -0.213f, 0.5478f, -0.227f,      0.0f, -1.0f, 0.0f,                  0.0f, 0.0f,
    -0.343f, 0.5478f, -0.227f,      0.0f, -1.0f, 0.0f,                  0.0f, 0.0f,
    -0.343f, 0.5478f, -0.332f,      0.0f, -1.0f, 0.0f,                  0.0f, 0.0f,
    -0.213f, 0.5478f, -0.332f,      0.0f, -1.0f, 0.0f,                  0.0f, 0.0f,
    // short block
    -0.240464f, 0.0f, -0.271646f,   0.285951942f, 0.0f, -0.958243966f,  0.0f, 0.0f,
    -0.240464f, 0.165f, -0.271646f, 0.285951942f, 0.0f, -0.958243966f,  0.0f, 0.0f,
    -0.082354f, 0.165f, -0.224464f, 0.285951942f, 0.0f, -0.958243966f,  0.0f, 0.0f,
    -0.082354f, 0.0f, -0.224464f,   0.285951942f, 0.0f, -0.958243966f,  0.0f, 0.0f,
    -0.240464f, 0.0, -0.271646f,    -0.958243966f, 0.0f, -0.285951942f, 0.0f, 0.0f,
    -0.240464f, 0.165f, -0.271646f, -0.958243966f, 0.0f, -0.285951942f, 0.0f, 0.0f,
    -0.287646f, 0.165f, -0.113536f, -0.958243966f, 0.0f, -0.285951942f, 0.0f, 0.0f,
    -0.287646f, 0.0f, -0.113536f,   -0.958243966f, 0.0f, -0.285951942f, 0.0f, 0.0f,
    -0.082354f, 0.0f, -0.224464f,   0.958243966f, 0.0f, 0.285951942f,   0.0f, 0.0f,
    -0.082354f, 0.165f, -0.224464f, 0.958243966f, 0.0f, 0.285951942f,   0.0f, 0.0f,
    -0.129536f, 0.165f, -0.066354f, 0.958243966f, 0.0f, 0.285951942f,   0.0f, 0.0f,
    -0.129536f, 0.0f, -0.066354f,   0.958243966f, 0.0f, 0.285951942f,   0.0f, 0.0f,
    -0.287646f, 0.0f, -0.113536f,   -0.285951942f, 0.0f, 0.958243966f,  0.0f, 0.0f,
    -0.287646f, 0.165f, -0.113536f, -0.285951942f, 0.0f, 0.958243966f,  0.0f, 0.0f,
    -0.129536f, 0.165f, -0.066354f, -0.285951942f, 0.0f, 0.958243966f,  0.0f, 0.0f,
    -0.129536f, 0.0f, -0.066354f,   -0.285951942f, 0.0f, 0.958243966f,  0.0f, 0.0f,
    -0.240464f, 0.165f, -0.271646f, 0.0f, 1.0f, 0.0f,                   0.0f, 0.0f,
    -0.082354f, 0.165f, -0.224464f, 0.0f, 1.0f, 0.0f,                   0.0f, 0.0f,
    -0.129536f, 0.165f, -0.066354f, 0.0f, 1.0f, 0.0f,                   0.0f, 0.0f,
    -0.287646f, 0.165f, -0.113536f, 0.0f, 1.0f, 0.0f,                   0.0f, 0.0f,
    // tall block
    -0.471239f, 0.0f, -0.405353f,   -0.296278358f, 0.0f, -0.955101609f, 0.0f, 0.0f,
    -0.471239f, 0.33f, -0.405353f,  -0.296278358f, 0.0f, -0.955101609f, 0.0f, 0.0f,
    -0.313647f, 0.33f, -0.454239f,  -0.296278358f, 0.0f, -0.955101609f, 0.0f, 0.0f,
    -0.313647f, 0.0f, -0.454239f,   -0.296278358f, 0.0f, -0.955101609f, 0.0f, 0.0f,
    -0.264761f, 0.0f, -0.296647f,   0.955101609f, 0.0f, -0.296278358f,  0.0f, 0.0f,
    -0.264761f, 0.33f, -0.296647f,  0.955101609f, 0.0f, -0.296278358f,  0.0f, 0.0f,
    -0.313647f, 0.33f, -0.454239f,  0.955101609f, 0.0f, -0.296278358f,  0.0f, 0.0f,
    -0.313647f, 0.0f, -0.454239f,   0.955101609f, 0.0f, -0.296278358f,  0.0f, 0.0f,
    -0.471239f, 0.0f, -0.405353f,   -0.955101609f, 0.0f, 0.296278358f,  0.0f, 0.0f,
    -0.471239f, 0.33f, -0.405353f,  -0.955101609f, 0.0f, 0.296278358f,  0.0f, 0.0f,
    -0.422353f, 0.33f, -0.247761f,  -0.955101609f, 0.0f, 0.296278358f,  0.0f, 0.0f,
    -0.422353f, 0.0f, -0.247761f,   -0.955101609f, 0.0f, 0.296278358f,  0.0f, 0.0f,
    -0.422353f, 0.0f, -0.247761f,   0.296278358f, 0.0f, 0.955101609f,   0.0f, 0.0f,
    -0.422353f, 0.33f, -0.247761f,  0.296278358f, 0.0f, 0.955101609f,   0.0f, 0.0f,
    -0.264761f, 0.33f, -0.296647f,  0.296278358f, 0.0f, 0.955101609f,   0.0f, 0.0f,
    -0.264761f, 0.0f, -0.296647f,   0.296278358f, 0.0f, 0.955101609f,   0.0f, 0.0f,
    -0.471239f, 0.33f, -0.405353f,  0.0f, 1.0f, 0.0f,                   0.0f, 0.0f,
    -0.313647f, 0.33f, -0.454239f,  0.0f, 1.0f, 0.0f,                   0.0f, 0.0f,
    -0.264761f, 0.33f, -0.296647f,  0.0f, 1.0f, 0.0f,                   0.0f, 0.0f,
    -0.422353f, 0.33f, -0.247761f,  0.0f, 1.0f, 0.0f,                   0.0f, 0.0f,
  };
  const uint32_t indices[] = {
    0, 2, 1, 0, 3, 2,
    4, 6, 5, 4, 7, 6,
    8, 9, 10, 8, 10, 11,
    12, 13, 14, 13, 15, 14,
    16, 17, 18, 18, 17, 19,
    20, 21, 22, 20, 22, 23,
    24, 25, 26, 24, 26, 27,
    28, 30, 29, 28, 31, 30,
    32, 33, 34, 32, 34, 35,
    36, 38, 37, 36, 39, 38,
    40, 42, 41, 40, 43, 42,
    44, 45, 46, 44, 46, 47,
    48, 50, 49, 48, 51, 50,
    52, 54, 53, 52, 55, 54,
    56, 58, 57, 56, 59, 58,
    60, 62, 61, 60, 63, 62,
  };

  Ray::mesh_desc_t mesh_desc;
  mesh_desc.prim_type = Ray::ePrimType::TriangleList;
  mesh_desc.vtx_positions = {attrs, 0, 8};
  mesh_desc.vtx_normals = {attrs, 3, 8};
  mesh_desc.vtx_uvs = {attrs, 6, 8};
  mesh_desc.vtx_indices = indices;

  // Setup material groups
  const Ray::mat_group_desc_t groups[] = {
    {mat0, 0, 6},
    {mat1, 6, 12},
    {mat2, 19, 6},
    {mat3, 25, 6},
    {mat4, Ray::InvalidMaterialHandle, 31, 6},
    {mat1, 37, 60},
  };
  mesh_desc.groups = groups;

  Ray::MeshHandle mesh1 = scene->AddMesh(mesh_desc);

  // Instantiate mesh
  const float xform[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
  scene->AddMeshInstance(mesh1, xform);

  // Add camera
  const float view_origin[] = {-0.278f, 0.273f, 0.8f};
  const float view_dir[] = {0.0f, 0.0f, -1.0f};

  Ray::camera_desc_t cam_desc;
  cam_desc.type = Ray::eCamType::Persp;
  memcpy(&cam_desc.origin[0], &view_origin[0], 3 * sizeof(float));
  memcpy(&cam_desc.fwd[0], &view_dir[0], 3 * sizeof(float));
  cam_desc.fov = 39.1463f;

  const Ray::CameraHandle cam = scene->AddCamera(cam_desc);
  scene->set_current_cam(cam);

  scene->Finalize();

  const bool EnableHighQualityDenoising = true;

  // Render region
  auto region = Ray::RegionContext{{0, 0, window_w, window_h}};

  // Camera

  ReiiVec4 camera_pos  = {-0.278f, 0.273f, 0.8f};
  ReiiVec4 camera_quat = {0, 1, 0, 0}; // Rotated 180 degrees in Y axis
  int camera_is_enabled = 0;
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

  uint64_t batch = 0;

  RedStructDeclarationMember pp_slots[2] = {0};
  uint64_t cs = 0;
  uint64_t pp = 0;
  {
    #include "ray_copy_pixels.cs.h"
    gpu_program_info_t cs_info = {0};
    cs_info.program_binary_bytes_count = sizeof(g_main);
    cs_info.program_binary             = g_main;
    cs = vfProgramCreateFromBinaryCompute(ctx, &cs_info, FF, LL);

    pp_slots[0].slot            = 0;
    pp_slots[0].type            = RED_STRUCT_MEMBER_TYPE_TEXTURE_RW;
    pp_slots[0].count           = 1;
    pp_slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;

    pp_slots[1].slot            = 1;
    pp_slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    pp_slots[1].count           = 1;
    pp_slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
    gpu_program_pipeline_compute_info_t pp_info = {0};
    pp_info.compute_program       = cs;
    pp_info.variables_slot        = 2;
    pp_info.variables_bytes_count = 2 * sizeof(float);
    pp_info.struct_members_count  = countof(pp_slots);
    pp_info.struct_members        = pp_slots;
    pp = vfProgramPipelineCreateCompute(ctx, &pp_info, FF, LL);
  }

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    int os_window_w = 0;
    int os_window_h = 0;
    glfwGetWindowSize(window, &os_window_w, &os_window_h);

    if (vfWindowIsMinimized(ctx) || os_window_w == 0 || os_window_h == 0) {
      continue;
    }

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    int mouse_right_mouse_button_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);

    // NOTE(Constantine):
    // Camera quaternion rotation and translation.
    const float mouse_move_sensitivity = 0.0035f;
    const float camera_move_speed      = 0.01f;
    float camera_side_vec[3] = {1, 0, 0};
    float   camera_up_vec[3] = {0, 1, 0};
    float  camera_dir_vec[3] = {0, 0, 1};
    float camera_move_vec_normalized[3] = {0, 0, 0};
    float mouse_move_x = mouse_move_sensitivity * (float)(mouse_x_prev - mouse_x); // NOTE(Constantine): Inverted than usual.
    float mouse_move_y = mouse_move_sensitivity * (float)(mouse_y - mouse_y_prev);
    int camera_is_disabled = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;

    quatRotateVec3Fast(camera_side_vec, camera_side_vec, &camera_quat.x);
    quatRotateVec3Fast(  camera_up_vec,   camera_up_vec, &camera_quat.x);
    quatRotateVec3Fast( camera_dir_vec,  camera_dir_vec, &camera_quat.x);

    if (mouse_right_mouse_button_state == GLFW_PRESS && mouse_right_mouse_button_state != mouse_right_mouse_button_state_prev) {
      camera_is_enabled = !camera_is_enabled;
      glfwSetInputMode(window, GLFW_CURSOR, camera_is_enabled == 1 ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    } else if (camera_is_enabled == 1) {
      float key_f = glfwGetKey(window, GLFW_KEY_W);
      float key_b = glfwGetKey(window, GLFW_KEY_S);

      float key_r = glfwGetKey(window, GLFW_KEY_D);
      float key_l = glfwGetKey(window, GLFW_KEY_A);

      float key_u = glfwGetKey(window, GLFW_KEY_E);
      float key_d = glfwGetKey(window, GLFW_KEY_Q);

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
      vec3Mulf(side_vec, camera_side_vec, key_l - key_r); // NOTE(Constantine): Inverted than usual.
      vec3Mulf(  up_vec,   camera_up_vec, key_u - key_d);
      vec3Mulf( dir_vec,  camera_dir_vec, key_f - key_b);

      vec3Add(camera_move_vec_normalized, camera_move_vec_normalized, side_vec);
      vec3Add(camera_move_vec_normalized, camera_move_vec_normalized,   up_vec);
      vec3Add(camera_move_vec_normalized, camera_move_vec_normalized,  dir_vec);

      float move_vec_len = sqrtf(
        camera_move_vec_normalized[0] * camera_move_vec_normalized[0] +
        camera_move_vec_normalized[1] * camera_move_vec_normalized[1] +
        camera_move_vec_normalized[2] * camera_move_vec_normalized[2]
      );
      if (move_vec_len != 0) {
        camera_move_vec_normalized[0] /= move_vec_len;
        camera_move_vec_normalized[1] /= move_vec_len;
        camera_move_vec_normalized[2] /= move_vec_len;
      }

      vec3Mulf(camera_move_vec_normalized, camera_move_vec_normalized, camera_move_speed);

      vec3Add(&camera_pos.x, &camera_pos.x, camera_move_vec_normalized);
    }

    // Now let's draw pixels

    int window_w = 0;
    int window_h = 0;
    vfWindowGetSize(ctx, &window_w, &window_h);

    cam_desc.type = Ray::eCamType::Persp;
    cam_desc.origin[0] = camera_pos.x;
    cam_desc.origin[1] = camera_pos.y;
    cam_desc.origin[2] = camera_pos.z;
    cam_desc.fwd[0] = camera_dir_vec[0];
    cam_desc.fwd[1] = camera_dir_vec[1];
    cam_desc.fwd[2] = camera_dir_vec[2];
    cam_desc.fov = 39.1463f;
    scene->SetCamera(cam, cam_desc);

    if (camera_is_disabled == 1) {
      Ray::color_rgba_t clearColor = {0};
      renderer->Clear(clearColor);
      region.Clear();
      for (int i = 0; i < 4; i += 1) {
        // NOTE(Constantine): Pre-render at least 4 samples for the initial image not to look too dark.
        renderer->RenderScene(*scene, region);
      }
    }
    renderer->RenderScene(*scene, region);

    if (0) {
      // Denoising...
      if (EnableHighQualityDenoising) {
        // Initialize neural denoiser
        const Ray::unet_filter_properties_t unet_props = renderer->InitUNetFilter(true);
        for (int pass = 0; pass < unet_props.pass_count; ++pass) {
          renderer->DenoiseImage(pass, region);
        }
      } else {
        // Run simple NLM filter
        renderer->DenoiseImage(region);
      }
    }

    Ray::GpuImage gpu_image = renderer->get_native_raw_final_pixels();

    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 1;
    bindings_info.max_texture_rw_binds_count  = 1;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    {
      vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)ctx;
      RedHandleCalls callsHandle = vfBatchGetRawHandle(ctx, batch, FF, LL);

      RedCallProceduresAndAddresses addresses = {0};
      np(redGetCallProceduresAndAddresses,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "outCallProceduresAndAddresses", &addresses,
        "outStatuses", NULL,
        "optionalFile", FF,
        "optionalLine", LL,
        "optionalUserData", NULL
      );
      RedUsageImage imageUsage = {0};
      imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
      imageUsage.oldAccessStages        = RED_ACCESS_STAGE_BITFLAG_COPY;
      imageUsage.newAccessStages        = RED_ACCESS_STAGE_BITFLAG_COMPUTE;
      imageUsage.oldAccess              = RED_ACCESS_BITFLAG_COPY_W;
      imageUsage.newAccess              = RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R | RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W;
      imageUsage.oldState               = (RedState)0x00000007/*VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL*/;
      imageUsage.newState               = RED_STATE_USABLE;
      imageUsage.queueFamilyIndexSource = -1;
      imageUsage.queueFamilyIndexTarget = -1;
      imageUsage.image                  = (RedHandleImage)gpu_image.vk_image;
      imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
      imageUsage.imageLevelsFirst       = 0;
      imageUsage.imageLevelsCount       = -1;
      imageUsage.imageLayersFirst       = 0;
      imageUsage.imageLayersCount       = -1;
      Red2UsageImageTempCallStruct imageUsageTempStruct = {0};
      np(red2CallUsageAliasOrderBarrier,
        "address", addresses.redCallUsageAliasOrderBarrier,
        "calls", callsHandle,
        "context", vkfast->context,
        "arrayUsagesCount", 0,
        "arrayUsages", NULL,
        "arrayTempCallStructs", NULL,
        "imageUsagesCount", 1,
        "imageUsages", &imageUsage,
        "imageTempCallStructs", &imageUsageTempStruct,
        "aliasesCount", 0,
        "aliases", NULL,
        "ordersCount", 0,
        "orders", NULL,
        "dependencyByRegion", 0
      );
    }
    vfBatchBindProgramPipelineCompute(ctx, batch, pp, FF, LL);
    vfBatchBindNewBindingsSet(ctx, batch, countof(pp_slots), pp_slots, FF, LL);
    RedStructMemberTexture texture = {0};
    texture.sampler = NULL;
    texture.texture = (RedHandleTexture)gpu_image.vk_image_view;
    texture.setTo1  = 1;
    vfBatchBindTextureRWEx(ctx, batch, 0, 1, &texture, FF, LL);
    vfBatchBindStorageRaw(ctx, batch, 1, 1, &gpu_pixels.gpu, FF, LL);
    vfBatchBindNewBindingsEnd(ctx, batch, FF, LL);
    float window_wh[2] = {(float)window_w, (float)window_h};
    vfBatchBindVariablesCopy(ctx, batch, 0, sizeof(window_wh), window_wh, FF, LL);
    vfBatchCompute(ctx, batch, (window_w / 8) + 1, (window_h / 8) + 1, 1, FF, LL);
    vfBatchEnd(ctx, batch, FF, LL);

    gpu_thread_t gpu_thread_ray = (gpu_thread_t)VKFAST_EXTRA_RAY_HACK_QUEUE_SUBMIT_VK_SEMAPHORE;
    gpu_thread_t gpu_threads[3] = {gpu_thread, gpu_thread_ray, 0};

    {
      RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
      uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 2, gpu_threads, array65536, FF, LL);
      vfAsyncWaitToFinish(ctx, wait, FF, LL);
    }

    vfAsyncDrawPixelsRaw(ctx, &gpu_pixels.gpu, NULL, 3, gpu_threads, array65536, FF, LL);

    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    {
      vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)ctx;
      RedHandleCalls callsHandle = vfBatchGetRawHandle(ctx, batch, FF, LL);

      RedCallProceduresAndAddresses addresses = {0};
      np(redGetCallProceduresAndAddresses,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "outCallProceduresAndAddresses", &addresses,
        "outStatuses", NULL,
        "optionalFile", FF,
        "optionalLine", LL,
        "optionalUserData", NULL
      );
      RedUsageImage imageUsage = {0};
      imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
      imageUsage.oldAccessStages        = 0;
      imageUsage.newAccessStages        = RED_ACCESS_STAGE_BITFLAG_COPY;
      imageUsage.oldAccess              = 0;
      imageUsage.newAccess              = RED_ACCESS_BITFLAG_COPY_W;
      imageUsage.oldState               = RED_STATE_USABLE;
      imageUsage.newState               = (RedState)0x00000007/*VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL*/;
      imageUsage.queueFamilyIndexSource = -1;
      imageUsage.queueFamilyIndexTarget = -1;
      imageUsage.image                  = (RedHandleImage)gpu_image.vk_image;
      imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
      imageUsage.imageLevelsFirst       = 0;
      imageUsage.imageLevelsCount       = -1;
      imageUsage.imageLayersFirst       = 0;
      imageUsage.imageLayersCount       = -1;
      Red2UsageImageTempCallStruct imageUsageTempStruct = {0};
      np(red2CallUsageAliasOrderBarrier,
        "address", addresses.redCallUsageAliasOrderBarrier,
        "calls", callsHandle,
        "context", vkfast->context,
        "arrayUsagesCount", 0,
        "arrayUsages", NULL,
        "arrayTempCallStructs", NULL,
        "imageUsagesCount", 1,
        "imageUsages", &imageUsage,
        "imageTempCallStructs", &imageUsageTempStruct,
        "aliasesCount", 0,
        "aliases", NULL,
        "ordersCount", 0,
        "orders", NULL,
        "dependencyByRegion", 0
      );
    }
    vfBatchEnd(ctx, batch, FF, LL);

    {
      RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
      uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 2, gpu_threads, array65536, FF, LL);
      vfAsyncWaitToFinish(ctx, wait, FF, LL);
    }

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
  
  uint64_t ids[] = {
    batch,
    pp,
    cs
  };
  vfIdDestroy(countof(ids), ids, FF, LL);

  delete scene;
  delete renderer;

  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}
