//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require-config debug,release,release-fast
//\\rc rawbuild `gcc`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` main.c ../../vkfast.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm`
//\\rc rawbuild end

#include "../../vkfast.h"
#include "../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#include "../Common/vkfast_examples_common.h"

int main() {
  gpu_handle_context_t ctx = vfContextInitNoDefaultAllocs(1, NULL, FF, LL);

  RedMemoryBudget budget = {};
  vfeReBARGetMemoryBudget(ctx, &budget);
  REDGPU_2_EXPECTFL(
    budget.memoryHeapsBudget[0] >= 10ULL * 1024*1024*1024 ||
    !"This example is hardcoded for either a ReBAR-capable dGPU with 12+ GB of VRAM or any iGPU, and we need to guarantee at least 10 GB of free GPU memory available." ||
    !"If your GPU meets this requirement, try to close your web browsers or other apps that can consume GPU memory and re-run this example program again."
  );
  REDGPU_2_EXPECTFL(vfeReBARGetMaxMallocSharedSize(ctx) >= (4ULL * 1024*1024*1024 - 1));

  // ReBAR Test
  {
    VfeReBARMallocShared part1_4GB_array = {};
    VfeReBARMallocShared part2_4GB_array = {};
    VfeReBARMallocShared part3_2GB_array = {};

    void * part1_4GB = vfeReBARMallocShared(ctx, 4ULL * 1024*1024*1024 - 64, &part1_4GB_array);
    void * part2_4GB = vfeReBARMallocShared(ctx, 4ULL * 1024*1024*1024 - 64, &part2_4GB_array);
    void * part3_2GB = vfeReBARMallocShared(ctx, 2ULL * 1024*1024*1024 - 64, &part3_2GB_array);

    REDGPU_2_EXPECTFL(part1_4GB != NULL);
    REDGPU_2_EXPECTFL(part2_4GB != NULL);
    REDGPU_2_EXPECTFL(part3_2GB != NULL);

    vfeReBARFreeShared(ctx, &part1_4GB_array);
    vfeReBARFreeShared(ctx, &part2_4GB_array);
    vfeReBARFreeShared(ctx, &part3_2GB_array);
    part1_4GB = NULL;
    part2_4GB = NULL;
    part3_2GB = NULL;
  }

  vfContextDeinit(ctx, FF, LL);
}
