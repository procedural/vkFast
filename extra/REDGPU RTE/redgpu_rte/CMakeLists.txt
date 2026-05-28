cmake_minimum_required(VERSION 3.5)
project(redgpu_rte)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories(
  "C:/VulkanSDK/1.4.341.1/Include"
  "nvpro_core"
  "nvpro_core/nvp"
  "nvpro_core/third_party/tinygltf"
  "nvpro_core/third_party/imgui"
  "nvpro_core/third_party/vma/include"
)

add_library(redgpu_rte SHARED redgpu_rte.cpp)

target_compile_options(redgpu_rte PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/bigobj>)
