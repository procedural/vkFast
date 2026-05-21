#include "../../vkfast.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_DO_NOT_LINK_GLFW3
#include "../Common/vkfast_examples_common.h"
#pragma comment(lib, "../../../Common/glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib") // NOTE(Constantine): Path relative to example's cmake/build/ folder.

#include "../../extra/Ray/Ray.h"

#include <fstream> // For std::ofstream
void WriteTGA(const Ray::color_rgba_t * data, int pitch, int w, int h, int bpp, const char * name);

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

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  GLFWwindow * window = glfwCreateWindow(700, 700, "[vkFast] Ray Denoising", NULL, NULL);
  void * window_handle = (void *)glfwGetWin32Window(window);

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] Ray Denoising", 700, 700, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  // Ray

  const int IMG_W = 256, IMG_H = 256;
  const int SAMPLE_COUNT = 256;

  // Initial frame resolution, can be changed later
  Ray::settings_t s;
  s.w = IMG_W;
  s.h = IMG_H;

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

  // Render image
  {
    // Create region contex for frame, setup to use whole frame
    auto region = Ray::RegionContext{{0, 0, IMG_W, IMG_H}};
    for (int i = 0; i < SAMPLE_COUNT; i++) {
      // Each call performs one iteration, blocks until finished
      renderer->RenderScene(*scene, region);
    }

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
  // Done.

  // Get rendered image pixels in 32-bit floating point RGBA format
  const Ray::color_data_rgba_t pixels = renderer->get_pixels_ref();

  // Save image
  WriteTGA(pixels.ptr, pixels.pitch, IMG_W, IMG_H, 3, "denoising.tga");
  // Image saved as denoising.tga

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    int os_window_w = 0;
    int os_window_h = 0;
    glfwGetWindowSize(window, &os_window_w, &os_window_h);

    if (vfWindowIsMinimized(ctx) || os_window_w == 0 || os_window_h == 0) {
      continue;
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

    // Clear pixels:
    for (int y = 0; y < window_h; y += 1) {
      for (int x = 0; x < window_w; x += 1) {
        pixels[y * window_w * 4 + x * 4 + 0] = 0;
        pixels[y * window_w * 4 + x * 4 + 1] = 0;
        pixels[y * window_w * 4 + x * 4 + 2] = 0;
        pixels[y * window_w * 4 + x * 4 + 3] = 0;
      }
    }
    // Draw pixels:
    for (int y = 0; y < window_h; y += 1) {
      for (int x = 0; x < window_w; x += 1) {
        // NOTE(Constantine):
        // The automatic DPI scaling is disabled with SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE), but
        // with DPI scaling, 1920x1080 screen resolution with a desktop scaling of, say, 1.25 is equal to 1536x864 (draw coords: 1535x863).
        if (y == 0) {
          pixels[y * window_w * 4 + x * 4 + 0] = 255;
          pixels[y * window_w * 4 + x * 4 + 1] = 0;
          pixels[y * window_w * 4 + x * 4 + 2] = 0;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        } else if (x == 0) {
          pixels[y * window_w * 4 + x * 4 + 0] = 0;
          pixels[y * window_w * 4 + x * 4 + 1] = 255;
          pixels[y * window_w * 4 + x * 4 + 2] = 0;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        } else if (y == mouse_y) {
          pixels[y * window_w * 4 + x * 4 + 0] = 0;
          pixels[y * window_w * 4 + x * 4 + 1] = 0;
          pixels[y * window_w * 4 + x * 4 + 2] = 255;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        } else if (x == mouse_x) {
          pixels[y * window_w * 4 + x * 4 + 0] = 255;
          pixels[y * window_w * 4 + x * 4 + 1] = 255;
          pixels[y * window_w * 4 + x * 4 + 2] = 255;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        }
      }
    }

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfDrawPixels(ctx, pixels, NULL, 2, gpu_threads, array65536, FF, LL);

    red32MemoryFree(pixels);
    pixels = NULL;
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
  
  delete scene;
  delete renderer;

  vfContextDeinit(ctx, FF, LL);
  vfExit(0);
}

void WriteTGA(const Ray::color_rgba_t * data, int pitch, const int w, const int h, const int bpp, const char * name) {
  #define float_to_byte(val) \
    (((val) <= 0.0f) ? 0 : (((val) > (1.0f - 0.5f / 255.0f)) ? 255 : uint8_t((255.0f * (val)) + 0.5f)))

  if (!pitch) {
    pitch = w;
  }

  std::ofstream file(name, std::ios::binary);

  unsigned char header[18] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  header[12] = w & 0xFF;
  header[13] = (w >> 8) & 0xFF;
  header[14] = (h)&0xFF;
  header[15] = (h >> 8) & 0xFF;
  header[16] = bpp * 8;
  header[17] |= (1 << 5); // set origin to upper left corner

  file.write((char *)&header[0], sizeof(header));

  auto out_data = std::make_unique<uint8_t[]>(size_t(w) * h * bpp);
  for (int j = 0; j < h; ++j) {
    for (int i = 0; i < w; ++i) {
      out_data[(j * w + i) * bpp + 0] = float_to_byte(data[j * pitch + i].v[2]);
      out_data[(j * w + i) * bpp + 1] = float_to_byte(data[j * pitch + i].v[1]);
      out_data[(j * w + i) * bpp + 2] = float_to_byte(data[j * pitch + i].v[0]);
      if (bpp == 4) {
        out_data[i * 4 + 3] = float_to_byte(data[j * pitch + i].v[3]);
      }
    }
  }

  file.write((const char *)&out_data[0], size_t(w) * h * bpp);

  static const char footer[26] =
    "\0\0\0\0"         // no extension area
    "\0\0\0\0"         // no developer directory
    "TRUEVISION-XFILE" // yep, this is a TGA file
    ".";
  file.write(footer, sizeof(footer));

  #undef float_to_byte
}
