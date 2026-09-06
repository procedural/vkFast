// su
// source /opt/intel/oneapi/setvars.sh intel64
// sycl-ls
// icpx -fsycl -g3 -gline-tables-only -fdebug-info-for-profiling -O0 main.cpp -o a.out

#include <sycl/sycl.hpp>
#include <sycl/ext/intel/esimd.hpp>
#include <iostream>
#include <vector>

using namespace sycl;
namespace esimd = sycl::ext::intel::esimd;

int main() {
    constexpr size_t N = 1024;
    // Chunk size processed per hardware thread (must align to hardware native widths, e.g., 16 or 32)
    constexpr size_t SIMD_WIDTH = 32;

    queue q{sycl::gpu_selector_v};

    // 1. Allocate Unified Shared Memory (USM) for hardware access
    float* input  = malloc_shared<float>(N, q);
    float* output = malloc_shared<float>(N, q);

    // Initialize data
    for (size_t i = 0; i < N; i++) input[i] = static_cast<float>(i);
    std::fill(output, output + N, 0.0f);

    // Filter coefficients: [0.25, 0.5, 0.25]
    float f_left = 0.25f, f_mid = 0.5f, f_right = 0.25f;

    // Total threads needed
    size_t num_threads = N / SIMD_WIDTH;

    // 2. Launch ESIMD Kernel
    q.submit([&](handler& h) {
        h.parallel_for<class Conv1DKernel>(
            range<1>(num_threads),
            [=](item<1> it) [[intel::sycl_explicit_simd]] { // Critical attribute

                size_t tid = it.get_id(0);
                size_t offset = tid * SIMD_WIDTH;

                // Handle boundaries safely by using vector loads and shifts
                // We load 32 elements natively into registers
                esimd::simd<float, SIMD_WIDTH> current_chunk;
                current_chunk.copy_from(input + offset);

                // Non-trivial requirement: Access overlapping data boundaries
                esimd::simd<float, SIMD_WIDTH> left_chunk;
                if (offset == 0) {
                    // Left boundary condition (Pad with 0)
                    left_chunk.select<SIMD_WIDTH - 1, 1>(1) = current_chunk.select<SIMD_WIDTH - 1, 1>(0);
                    left_chunk[0] = 0.0f;
                } else {
                    // Load misaligned elements safely via offset scalar shifts
                    left_chunk.copy_from(input + offset - 1);
                }

                esimd::simd<float, SIMD_WIDTH> right_chunk;
                if (offset + SIMD_WIDTH >= N) {
                    // Right boundary condition (Pad with 0)
                    right_chunk.select<SIMD_WIDTH - 1, 1>(0) = current_chunk.select<SIMD_WIDTH - 1, 1>(1);
                    right_chunk[SIMD_WIDTH - 1] = 0.0f;
                } else {
                    right_chunk.copy_from(input + offset + 1);
                }

                // 3. Fused FMA (Multiply-Accumulate) executed purely inside the hardware register file
                esimd::simd<float, SIMD_WIDTH> result =
                    (left_chunk * f_left) + (current_chunk * f_mid) + (right_chunk * f_right);

                // 4. Block store write back out to main memory
                result.copy_to(output + offset);
            });
    }).wait();

    // Verify a random inner index (e.g., index 5 -> expected: 4*0.25 + 5*0.5 + 6*0.25 = 5.0)
    std::cout << "Output[5]: " << output[5] << " (Expected: 5.0)" << std::endl;

    free(input, q);
    free(output, q);
    return 0;
}
