// NOTE(Constantine):
//
// Include the following compile flags:
// -shared
// -o redgpu_rte.dll
// -fPIC
// -std=c++20
// -I.
// -IC:/VulkanSDK/1.4.341.1/Include
// -Invpro_core
// -Invpro_core/nvp
// -Invpro_core/third_party/tinygltf
// -Invpro_core/third_party/imgui
// -Invpro_core/third_party/vma/include
// framework/glfw3.dll
// C:/Windows/System32/vulkan-1.dll
// -lgdi32
// -lcomdlg32
// -Wl,--image-base
// -Wl,0x10000000
//
// In nvpro_core/nvh/misc.hpp
// replace:
// size_t charsNeeded = static_cast<size_t>(vsnprintf(str.data(), str.size(), msg, list)); // charsNeeded doesn't count \0
// with:
// size_t charsNeeded = static_cast<size_t>(vsnprintf((char *)str.data(), str.size(), msg, list)); // charsNeeded doesn't count \0
//
// In nvpro_core/nvvk/appwindowprofiler_vk.cpp
// In nvpro_core/nvp/nvpsystem.cpp
// In nvpro_core/nvp/nvpsystem_win32.cpp
// In nvpro_core/nvp/nvpwindow.cpp
// In nvpro_core/nvvkhl/appbase_vk.hpp
// In nvpro_core/nvvkhl/appbase_vkpp.hpp
// In nvpro_core/nvvkhl/application.cpp
// In nvpro_core/imgui/imgui_helper.cpp
// In nvpro_core/third_party/imgui/backends/imgui_impl_glfw.cpp
// replace all:
// #include <GLFW/glfw3.h>
// #include <GLFW/glfw3native.h>
// with:
// #include "framework/glfw3.h"
// #include "framework/glfw3native.h"
//
// In nvpro_core/nvh/parametertools.cpp
// comment the whole function:
// static bool endsWith(std::string const& s, std::string const& end)
//
// In nvpro_core/third_party/tinygltf/json.hpp
// replace all:
// (std::snprintf)
// with:
// (snprintf)
//
// In nvpro_core/imgui/backends/imgui_vk_extra.cpp
// rename all:
// check_vk_result
// to:
// assert_check_vk_result
//
#define REDGPU_RTE_DLL_EXPORT_IMPORT __declspec(dllexport)
#define PROJECT_NAME "REDGPU RTE"
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "C:/VulkanSDK/1.4.341.1/Lib/vulkan-1.lib") // For CMake
#pragma comment(lib, "../framework/glfw3dll.lib") // For CMake
#define NOMINMAX
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include "nvpro_core/nvvk/appwindowprofiler_vk.cpp"
#include "nvpro_core/nvvk/buffersuballocator_vk.cpp"
#include "nvpro_core/nvvk/commands_vk.cpp"
#include "nvpro_core/nvvk/context_vk.cpp"
#include "nvpro_core/nvvk/debug_util_vk.cpp"
#include "nvpro_core/nvvk/descriptorsets_vk.cpp"
#include "nvpro_core/nvvk/dynamicrendering_vk.cpp"
#include "nvpro_core/nvvk/error_vk.cpp"
#include "nvpro_core/nvvk/extensions_vk.cpp"
#include "nvpro_core/nvvk/gizmos_vk.cpp"
#include "nvpro_core/nvvk/images_vk.cpp"
#include "nvpro_core/nvvk/memallocator_dedicated_vk.cpp"
#include "nvpro_core/nvvk/memallocator_vk.cpp"
#include "nvpro_core/nvvk/memorymanagement_vk.cpp"
#include "nvpro_core/nvvk/memorymanagement_vkgl.cpp"
#include "nvpro_core/nvvk/nsight_aftermath_vk.cpp"
#include "nvpro_core/nvvk/pipeline_vk.cpp"
#include "nvpro_core/nvvk/profiler_vk.cpp"
#include "nvpro_core/nvvk/raytraceKHR_vk.cpp"
#include "nvpro_core/nvvk/raytraceNV_vk.cpp"
#include "nvpro_core/nvvk/renderpasses_vk.cpp"
#include "nvpro_core/nvvk/resourceallocator_vk.cpp"
#include "nvpro_core/nvvk/samplers_vk.cpp"
#include "nvpro_core/nvvk/sbtwrapper_vk.cpp"
#include "nvpro_core/nvvk/shadermodulemanager_vk.cpp"
#include "nvpro_core/nvvk/stagingmemorymanager_vk.cpp"
#include "nvpro_core/nvvk/swapchain_vk.cpp"
#include "nvpro_core/nvh/appwindowprofiler.cpp"
#include "nvpro_core/nvh/bitarray.cpp"
#include "nvpro_core/nvh/cameramanipulator.cpp"
#include "nvpro_core/nvh/filemapping.cpp"
#include "nvpro_core/nvh/gltfscene.cpp"
#include "nvpro_core/nvh/nvprint.cpp"
#include "nvpro_core/nvh/parametertools.cpp"
#include "nvpro_core/nvh/primitives.cpp"
#include "nvpro_core/nvh/profiler.cpp"
#include "nvpro_core/nvh/shaderfilemanager.cpp"
#include "nvpro_core/nvp/nvpfilesystem.cpp"
#include "nvpro_core/nvp/nvpsystem.cpp"
#if defined(_WIN32)
#include "nvpro_core/nvp/nvpsystem_win32.cpp"
#elif defined(__linux__)
#include "nvpro_core/nvp/nvpsystem_linux.cpp"
#endif
#include "nvpro_core/nvp/nvpwindow.cpp"
#include "nvpro_core/nvp/perproject_globals.cpp"
#include "nvpro_core/nvvkhl/appbase_vk.cpp"
#include "nvpro_core/nvvkhl/appbase_vkpp.cpp"
#include "nvpro_core/nvvkhl/application.cpp"
#include "nvpro_core/nvvkhl/gbuffer.cpp"
#include "nvpro_core/nvvkhl/gltf_scene_rtx.cpp"
#include "nvpro_core/nvvkhl/gltf_scene_vk.cpp"
#include "nvpro_core/nvvkhl/hdr_env.cpp"
//#include "nvpro_core/nvvkhl/hdr_env_dome.cpp"
//#include "nvpro_core/nvvkhl/sky.cpp"
//#include "nvpro_core/nvvkhl/tonemap_postprocess.cpp"
#include "nvpro_core/third_party/imgui/imgui.cpp"
#include "nvpro_core/third_party/imgui/imgui_demo.cpp"
#include "nvpro_core/third_party/imgui/imgui_draw.cpp"
#include "nvpro_core/third_party/imgui/imgui_tables.cpp"
#include "nvpro_core/third_party/imgui/imgui_widgets.cpp"
#include "nvpro_core/third_party/imgui/backends/imgui_impl_glfw.cpp"
#include "nvpro_core/imgui/imgui_camera_widget.cpp"
#include "nvpro_core/imgui/imgui_helper.cpp"
#include "nvpro_core/imgui/imgui_orient.cpp"
#include "nvpro_core/imgui/backends/imgui_impl_vulkan.cpp"
#include "nvpro_core/imgui/backends/imgui_vk_extra.cpp"
#define STB_IMAGE_IMPLEMENTATION
#include "nvpro_core/third_party/stb/stb_image.h"
#ifdef __MINGW64__
#include "nvpro_core/third_party/vma/include/vk_mem_alloc.cpp"
#endif
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE; // NOTE(Constantine): https://www.reddit.com/r/vulkan/comments/j9e6of/comment/g8km4fr/
