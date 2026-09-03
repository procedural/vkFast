// su
// source /opt/intel/oneapi/setvars.sh intel64
// sycl-ls
// icpx -fsycl -g3 -gline-tables-only -fdebug-info-for-profiling -O0 main.cpp -o a.out

#include <sycl/sycl.hpp>
#include <sycl/ext/intel/esimd.hpp>
#include <iostream>

using namespace sycl;
namespace esimd = sycl::ext::intel::esimd;

int main() {
  // 1. Setup the GPU queue
  queue q{gpu_selector_v};
  std::cout << "Running on: " << q.get_device().get_info<info::device::name>() << "\n";

  // 2. Define data size (8 elements for 1 single SIMD vector operation)
  constexpr int VL = 8;

  // 3. Allocate Unified Shared Memory (USM) visible to both Host and GPU
  int* a = malloc_shared<int>(VL, q);
  int* b = malloc_shared<int>(VL, q);
  int* c = malloc_shared<int>(VL, q);

  // Initialize input data
  for (int i = 0; i < VL; ++i) {
    a[i] = i;
    b[i] = i * 2;
    c[i] = 0;
  }

  // 4. Submit the ESIMD kernel to the queue
  q.submit([&](handler& cgh) {
    // The ESIMD kernel must be launched inside a single_task or basic parallel_for
    cgh.single_task<class EsimdHello>([=]() [[intel::sycl_explicit_simd]] {
      // Explicitly load 8 integers into a SIMD vector register
      esimd::simd<int, VL> va;
      va.copy_from(a);

      esimd::simd<int, VL> vb;
      vb.copy_from(b);

      sycl::ext::oneapi::experimental::printf("vb[7]: %d\n", (int)vb[7]);

      // Perform explicit SIMD parallel addition
      esimd::simd<int, VL> vc = va + vb;

      // Store the SIMD result register back to USM memory
      vc.copy_to(c);
    });
  }).wait(); // Wait for execution to finish

  // 5. Verify and Print the results
  std::cout << "Result calculations: \n";
  for (int i = 0; i < VL; ++i) {
      std::cout << a[i] << " + " << b[i] << " = " << c[i] << "\n";
  }

  // Free allocated memory
  free(a, q);
  free(b, q);
  free(c, q);

  return 0;
}

#if 0
// Output:

Running on: Intel(R) Arc(TM) B580 Graphics
vb[7]: 14
Result calculations:
0 + 0 = 0
1 + 2 = 3
2 + 4 = 6
3 + 6 = 9
4 + 8 = 12
5 + 10 = 15
6 + 12 = 18
7 + 14 = 21
#endif
