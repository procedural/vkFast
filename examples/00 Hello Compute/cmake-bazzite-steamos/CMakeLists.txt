# For Bazzite/SteamOS only.
project(00_Hello_Compute)
cmake_minimum_required(VERSION 3.20)
add_executable(${PROJECT_NAME}
  ${CMAKE_SOURCE_DIR}/../main.c
  ${CMAKE_SOURCE_DIR}/../../../vkfast.c
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
  /home/linuxbrew/.linuxbrew/lib/libX11.so
  /home/linuxbrew/.linuxbrew/lib/libvulkan.so
  -lm
  #-lasan # sudo dnf install libasan && LD_PRELOAD=/usr/lib64/libasan.so.8 ./00_Hello_Compute
)
target_compile_options(${PROJECT_NAME} PRIVATE
  -O0 -g #-fsanitize=address
)

# NOTE(Constantine):
#
# Newer Vulkan SDK setup for REDGPU SDK's redgpu.cpp with '#define REDGPU_COMPILE_SWITCH 3' on Bazzite / SteamOS:
#
# $ file /home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/etc/vulkan/explicit_layer.d
# /home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/etc/vulkan/explicit_layer.d: cannot open `/home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/etc/vulkan/explicit_layer.d' (No such file or directory)
# $ file /home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/share/vulkan/explicit_layer.d/
# /home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/share/vulkan/explicit_layer.d/: directory
# $ mkdir -p /home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/etc/vulkan/
# $ ln -s /home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/share/vulkan/explicit_layer.d/ /home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/etc/vulkan/
# $ file /home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/etc/vulkan/explicit_layer.d/
# /home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/etc/vulkan/explicit_layer.d/: directory
