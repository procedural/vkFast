cmake_minimum_required(VERSION 3.18)

project(RayGPUPresents LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../../../extra/Ray ${CMAKE_CURRENT_BINARY_DIR}/Ray)

add_executable(${PROJECT_NAME}
  ../main.cpp
  ../../../vkfast.c
  ../../../extra/Banzai/vkfast_extra_banzai.c
  ../../../extra/Banzai/vkfast_extra_banzai_pointer.c
  "../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c"
  C:/RedGpuSDK/redgpu.c
  C:/RedGpuSDK/redgpu_2.c
  C:/RedGpuSDK/redgpu_32.c
)

target_link_libraries(${PROJECT_NAME} Ray)
