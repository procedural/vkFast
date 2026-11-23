//\\rc rawbuild begin clang-windows-64-bit
//\\rc rawbuild require-config debug,release,release-fast

  //\\rc rawbuild `clang -c`
  //\\rc rawbuild debug ` -g -O0`
  //\\rc rawbuild release,release-fast ` -O2`
  //\\rc rawbuild ` ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c`

  //\\rc rawbuild next_command
  //\\rc rawbuild `clang++ -c`
  //\\rc rawbuild debug ` -g -O0`
  //\\rc rawbuild release,release-fast ` -O2`
  //\\rc rawbuild ` -DRR_STATIC_LIBRARY -I"..\../extra/Modified Vulkan/include/" "../../extra/Modified RadeonRays 2.0/*.cpp"`

  //\\rc rawbuild next_command
  //\\rc rawbuild `clang++`
  //\\rc rawbuild debug ` -g -O0`
  //\\rc rawbuild release,release-fast ` -O2`
  //\\rc rawbuild ` -DRR_STATIC_LIBRARY main.cpp *.o -I"..\../extra/Modified Vulkan/include/" ../Common/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a -lgdi32 -lvulkan-1`

//\\rc rawbuild end
