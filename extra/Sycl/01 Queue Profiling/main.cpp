// su
// source /opt/intel/oneapi/setvars.sh intel64
// sycl-ls
// icpx -fsycl -g3 -gline-tables-only -fdebug-info-for-profiling -O0 main.cpp -o a.out

// Source:
// https://github.khronos.org/SYCL_Reference/iface/event.html#example-1

#include <sycl/sycl.hpp>

#include <cstdlib>
#include <cstring>

int main() {
  sycl::property_list properties{sycl::property::queue::enable_profiling()};
  auto q = sycl::queue(sycl::default_selector_v, properties);

  std::cout
      << "  Platform: "
      << q.get_device().get_platform().get_info<sycl::info::platform::name>()
      << std::endl;

  std::cout
      << "  Running on: "
      << q.get_device().get_info<sycl::info::device::name>()
      << std::endl;

  std::cout
      << "  Queue profiling supported: "
      << q.get_device().has(sycl::aspect::queue_profiling)
      << std::endl;

  const int num_ints = 1024 * 1024;
  const size_t num_bytes = num_ints * sizeof(int);
  const int alignment = 8;

  // Alloc memory on host
  auto src = std::aligned_alloc(alignment, num_bytes);
  std::memset(src, 1, num_bytes);

  // Alloc memory on device
  auto dst = sycl::malloc_device<int>(num_ints, q);
  q.memset(dst, 0, num_bytes).wait();

  // Copy from host to device
  auto event = q.memcpy(dst, src, num_bytes);
  event.wait();

  auto end   = event.get_profiling_info<sycl::info::event_profiling::command_end>();
  auto start = event.get_profiling_info<sycl::info::event_profiling::command_start>();

  std::cout << "Elapsed time: " << (end - start) / 1.0e9 << " seconds\n";

  uint64_t submit_time = event.get_profiling_info<sycl::info::event_profiling::command_submit>();
  std::cout << "Queue overhead: " << (start - submit_time) / 1000.0 << " microseconds.\n";

  sycl::free(dst, q);
}
