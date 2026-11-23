//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require-config debug,release,release-fast
//\\rc rawbuild require-config glfw3,sdl3
//\\rc rawbuild `g++ -shared -fPIC -fvisibility=hidden`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` "../../extra/Dear ImGui 2016/imgui_megafile.cpp" -o libimgui.so`
//\\rc rawbuild next_command
//\\rc rawbuild `gcc`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild sdl3 ` -DVKFAST_EXAMPLE_29_ENABLE_GLFW3_TO_SDL3`
//\\rc rawbuild ` main.c ../Common/font_droid_sans_mono.c ../../vkfast.c ../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/`
//\\rc rawbuild glfw3 ` /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a`
//\\rc rawbuild sdl3 ` /home/linuxbrew/.linuxbrew/lib/libSDL3.so`
//\\rc rawbuild ` /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so libimgui.so -lm`
//\\rc rawbuild end

//\\rc rawbuild begin clang-windows-64-bit
//\\rc rawbuild require-config debug,release,release-fast
//\\rc rawbuild `clang++ -shared -fvisibility=hidden`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` "../../extra/Dear ImGui 2016/imgui_megafile.cpp" -o libimgui.so`
//\\rc rawbuild next_command
//\\rc rawbuild `clang`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` main.c ../Common/font_droid_sans_mono.c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c ../../extra/REII/vkfast_extra_reii.c libimgui.so ../Common/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a -lgdi32`
//\\rc rawbuild end

//\\rc rawbuild begin mingw-clang-termux-64-bit
//\\rc rawbuild `x86_64-w64-mingw32-clang++ -DVKFAST_INCLUDE_TERMUX_PATHS -shared -static-libgcc -static-libstdc++ -fPIC -fvisibility=hidden "../../extra/Dear ImGui 2016/imgui_megafile.cpp" -o imgui.dll`
//\\rc rawbuild next_command
//\\rc rawbuild `x86_64-w64-mingw32-clang -DVKFAST_INCLUDE_TERMUX_PATHS main.c ../../vkfast.c /data/data/com.termux/files/home/RedGpuSDK/redgpu.c /data/data/com.termux/files/home/RedGpuSDK/redgpu_2.c /data/data/com.termux/files/home/RedGpuSDK/redgpu_32.c ../../extra/REII/vkfast_extra_reii.c ../Common/font_droid_sans_mono.c ../Common/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a -luser32 -lshell32 -lgdi32 imgui.dll`
//\\rc rawbuild next_command
//\\rc rawbuild `cp a.exe /storage/emulated/0/Download/`
//\\rc rawbuild next_command
//\\rc rawbuild `cp imgui.dll /storage/emulated/0/Download/`
//\\rc rawbuild next_command
//\\rc rawbuild `cp ../../extra/Dear\ ImGui\ 2016/NotoSans.ttf /storage/emulated/0/Download/`
//\\rc rawbuild end
