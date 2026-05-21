#include "../../vkfast.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_DO_NOT_LINK_GLFW3
#include "../Common/vkfast_examples_common.h"
#pragma comment(lib, "../../../Common/glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib") // NOTE(Constantine): Path relative to example's cmake/build/ folder.

#include "../../extra/Ray/Ray.h"

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
#ifdef __MINGW32__
  SetProcessDPIAware();
#else
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

  const int window_w = 700;
  const int window_h = 700;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] Ray Denoising", NULL, NULL);
  void * window_handle = (void *)glfwGetWin32Window(window);

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] Ray Denoising", 700, 700, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  // Ray

  // Initial frame resolution, can be changed later
  Ray::settings_t s;
  s.w = window_w;
  s.h = window_h;

  // Additional Ray::eRendererType parameter can be passed (Vulkan GPU renderer created by default)
  Ray::RendererBase * renderer = Ray::CreateRenderer(s, &Ray::g_stdout_log);

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

    // To free
    unsigned char * pixels = (unsigned char *)red32MemoryCalloc(4 * window_h * window_w);
    REDGPU_2_EXPECTFL(pixels != NULL);

    double mouse_x = 0;
    double mouse_y = 0;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

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

    // Get rendered image pixels in 32-bit floating point RGBA format
    const Ray::color_data_rgba_t raypixels = renderer->get_pixels_ref();

    #define float_to_byte(val) \
      (((val) <= 0.0f) ? 0 : (((val) > (1.0f - 0.5f / 255.0f)) ? 255 : uint8_t((255.0f * (val)) + 0.5f)))

    // Draw pixels:
    for (int y = 0; y < window_h; y += 1) {
      for (int x = 0; x < window_w; x += 1) {
        float r = raypixels.ptr[y * raypixels.pitch + x].v[0];
        float g = raypixels.ptr[y * raypixels.pitch + x].v[1];
        float b = raypixels.ptr[y * raypixels.pitch + x].v[2];
        pixels[y * window_w * 4 + x * 4 + 0] = float_to_byte(b);
        pixels[y * window_w * 4 + x * 4 + 1] = float_to_byte(g);
        pixels[y * window_w * 4 + x * 4 + 2] = float_to_byte(r);
        pixels[y * window_w * 4 + x * 4 + 3] = 255;
      }
    }

    #undef float_to_byte

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfDrawPixels(ctx, pixels, NULL, 2, gpu_threads, array65536, FF, LL);

    red32MemoryFree(pixels);
    pixels = NULL;

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
  
  delete scene;
  delete renderer;

  vfContextDeinit(ctx, FF, LL);
  vfExit(0);
}
