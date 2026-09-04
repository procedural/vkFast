#if 0
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icpx -fsycl -fsycl-targets=spir64 -Xclang -fsycl-allow-func-ptr -I/home/constantine/Desktop/embree/include -L/home/constantine/Desktop/embree/build/ main.cpp -lembree4_sycl -lembree4 -o sycl_embree_demo
LD_LIBRARY_PATH=/home/constantine/Desktop/embree/build/:$LD_LIBRARY_PATH ./sycl_embree_demo
#endif

#include <iostream>
#include <sycl/sycl.hpp>
#include <embree4/rtcore.h>
#include <embree4/rtcore_device.h>

int main() {
    // 1. Initialize SYCL Queue targeting a GPU (or default selector)
    sycl::queue queue(sycl::gpu_selector_v);
    std::cout << "Running on: " << queue.get_device().get_info<sycl::info::device::name>() << std::endl;

    // 2. Initialize Embree Device matching the SYCL context
    // In Embree 4, passing the SYCL context allows sharing USM memory and executing device kernels.
    RTCDevice device = rtcNewSYCLDevice(queue.get_context(), nullptr);
    if (!device) {
        std::cerr << "Failed to create Embree device\n";
        return 1;
    }
    RTCScene scene = rtcNewScene(device);

    // 3. Create a simple triangle geometry on the host
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    float* vertices = (float*) rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX, 0,
                                                       RTC_FORMAT_FLOAT3, 3 * sizeof(float), 3);
    // Triangle corner points
    vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f;
    vertices[3] = 1.0f; vertices[4] = 0.0f; vertices[5] = 0.0f;
    vertices[6] = 0.0f; vertices[7] = 1.0f; vertices[8] = 0.0f;

    unsigned int* indices = (unsigned int*) rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX, 0,
                                                                    RTC_FORMAT_UINT3, 3 * sizeof(unsigned int), 1);
    indices[0] = 0; indices[1] = 1; indices[2] = 2;

    rtcCommitGeometry(geom);
    rtcAttachGeometry(scene, geom);
    rtcReleaseGeometry(geom);

    // Commit the scene structure (this constructs the BVH acceleration structure)
    rtcCommitScene(scene);

    // 4. Allocate Universal Shared Memory (USM) for the result so the GPU can write to it
    bool* hit_result = sycl::malloc_shared<bool>(1, queue);
    *hit_result = false;

    // Get the device-side traversable handle of the scene for the SYCL kernel
    // (Replaces: RTCDeviceScene device_scene = rtcGetDeviceScene(scene);)
    RTCTraversable traversable_scene = rtcGetSceneTraversable(scene);

    // 5. Launch a single-threaded SYCL kernel to trace the ray on the GPU
    queue.submit([&](sycl::handler& cgh) {
        cgh.single_task([=]() {
            // Initialize arguments and features flags for optimal GPU code gen
            RTCIntersectArguments args;
            rtcInitIntersectArguments(&args);
            args.feature_mask = RTC_FEATURE_FLAG_TRIANGLE;

            // Define a ray aiming directly through the center of our triangle
            RTCRayHit rayhit;
            rayhit.ray.org_x = 0.25f;
            rayhit.ray.org_y = 0.25f;
            rayhit.ray.org_z = -1.0f;

            rayhit.ray.dir_x = 0.0f;
            rayhit.ray.dir_y = 0.0f;
            rayhit.ray.dir_z = 1.0f;

            rayhit.ray.tnear = 0.0f;
            rayhit.ray.tfar = std::numeric_limits<float>::infinity();
            rayhit.ray.mask = -1;
            rayhit.ray.flags = 0;

            rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
            rayhit.hit.primID = RTC_INVALID_GEOMETRY_ID;

            // Perform intersection using the modern traversable-specific functions
            // (Replaces: rtcIntersect1(device_scene, &rayhit, &args);)
            rtcTraversableIntersect1(traversable_scene, &rayhit, &args);

            // Record if a valid geometry hit occurred
            if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
                *hit_result = true;
            }
        });
    }).wait(); // Wait for the GPU kernel execution to finish

    // 6. Print results and cleanup memory
    std::cout << "Ray Intersection Result: " << (*hit_result ? "HIT" : "MISS") << std::endl;

    sycl::free(hit_result, queue);
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);

    return 0;
}
