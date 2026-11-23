//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require-config debug,release,release-fast
//\\rc rawbuild `gcc -c`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild release-fast ` -march=native`
//\\rc rawbuild ` ../../vkfast.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/`
//\\rc rawbuild next_command
//\\rc rawbuild `g++`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild release-fast ` -march=native`
//\\rc rawbuild ` main.cpp *.o -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm`
//\\rc rawbuild end

//\\rc rawbuild begin clang-windows-64-bit
//\\rc rawbuild require-config debug,release,release-fast
//\\rc rawbuild `clang -c`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild release-fast ` -march=native`
//\\rc rawbuild ` ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c`
//\\rc rawbuild next_command
//\\rc rawbuild `clang++`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild release-fast ` -march=native`
//\\rc rawbuild ` main.cpp *.o -Wl,--stack,64000000 ../Common/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a -lgdi32`
//\\rc rawbuild end

//\\rc rawbuild begin cl-windows-64-bit
//\\rc rawbuild `cl /EHsc main.cpp ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c /arch:AVX2 /openmp /link /stack:64000000`
//\\rc rawbuild end
