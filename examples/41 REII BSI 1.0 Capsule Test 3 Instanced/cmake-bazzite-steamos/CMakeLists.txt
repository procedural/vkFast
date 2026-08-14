# For Bazzite/SteamOS only.
project(41_REII_BSI_1_0_Capsule_Test_3_Instanced)
cmake_minimum_required(VERSION 3.20)
add_executable(${PROJECT_NAME}
  ${CMAKE_SOURCE_DIR}/../main.c
  ${CMAKE_SOURCE_DIR}/../../../vkfast.c
  ${CMAKE_SOURCE_DIR}/../../../extra/Banzai/vkfast_extra_banzai.c
  ${CMAKE_SOURCE_DIR}/../../../extra/Banzai/vkfast_extra_banzai_pointer.c
  "${CMAKE_SOURCE_DIR}/../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c"
  "${CMAKE_SOURCE_DIR}/../../../extra/REII/vkfast_extra_reii.c"
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
  /home/linuxbrew/.linuxbrew/lib/libX11.so
  /home/linuxbrew/.linuxbrew/lib/libvulkan.so
  -lm
  #-lasan # sudo dnf install libasan && LD_PRELOAD=/usr/lib64/libasan.so.8 ./41_REII_BSI_1_0_Capsule_Test_3_Instanced
)
target_compile_options(${PROJECT_NAME} PRIVATE
  -g #-fsanitize=address
)
