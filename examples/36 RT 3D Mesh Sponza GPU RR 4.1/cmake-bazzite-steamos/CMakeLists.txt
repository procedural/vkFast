# For Bazzite/SteamOS only.
project(36_RT_3D_Mesh_Sponza_GPU_RR_4_1)
cmake_minimum_required(VERSION 3.20)
add_executable(${PROJECT_NAME}
  ${CMAKE_SOURCE_DIR}/../main.c
  ${CMAKE_SOURCE_DIR}/../../../vkfast.c
  ${CMAKE_SOURCE_DIR}/../../../extra/Banzai/vkfast_extra_banzai.c
  ${CMAKE_SOURCE_DIR}/../../../extra/Banzai/vkfast_extra_banzai_pointer.c
  "${CMAKE_SOURCE_DIR}/../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c"
  /home/linuxbrew/RedGpuSDK/redgpu.c
  /home/linuxbrew/RedGpuSDK/redgpu_2.c
  /home/linuxbrew/RedGpuSDK/redgpu_32.c
)
include_directories(
  /home/linuxbrew/.linuxbrew/include/
  /home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/
  /var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
)
target_link_libraries(${PROJECT_NAME}
  /home/linuxbrew/.linuxbrew/Cellar/glfw/3.4/lib/libglfw3.a
  "${CMAKE_SOURCE_DIR}/../../../extra/RadeonRays SDK 4.1/build/bin/libradeonrays.so"
  /home/linuxbrew/.linuxbrew/lib/libX11.so
  /home/linuxbrew/.linuxbrew/lib/libvulkan.so
  -lm
  #-lasan # sudo dnf install libasan && LD_PRELOAD=/usr/lib64/libasan.so.8 ./36_RT_3D_Mesh_Sponza_GPU_RR_4_1
)
target_compile_options(${PROJECT_NAME} PRIVATE
  -O0 -g #-fsanitize=address
)
