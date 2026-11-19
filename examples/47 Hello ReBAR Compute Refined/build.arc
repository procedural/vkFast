//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require-config debug,release,release-fast
//\\rc rawbuild `gcc`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` main.c ../../vkfast.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm`
//\\rc rawbuild end
