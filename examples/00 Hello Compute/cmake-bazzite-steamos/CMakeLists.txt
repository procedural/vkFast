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
)
