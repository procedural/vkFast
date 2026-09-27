#if 0
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icx -c ../../../vkfast.c "../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
icpx -fsycl -fsycl-targets=spir64 -Xclang -fsycl-allow-func-ptr main.cpp *.o -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm -o a.out
XDG_SESSION_TYPE=x11 ./a.out
#endif

#include <iostream>
#include <vector>
#include <limits>

// NOTE(Constantine)(Sep 18, 2026): X11/X.h defines None, so we include sycl.hpp before vkFast.
#include <sycl/sycl.hpp>

#include "../../../vkfast.h"
#include "../../../extra/REII/vkfast_extra_reii.h"
#include "../../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLM
#include "../../../examples/Common/vkfast_examples_common.h"

using namespace glm;

// Source: https://www.shadertoy.com/view/lsX3DH

#define MOTIONBLUR
#define DEPTHOFFIELD

#define CUBEMAPSIZE 256

#define SAMPLES 8
#define PATHDEPTH 4
#define TARGETFPS 60.f

#define FOCUSDISTANCE 17.f
#define FOCUSBLUR 0.25f

#define RAYCASTSTEPS 20
#define RAYCASTSTEPSRECURSIVE 2

#define EPSILON 0.001f
#define MAXDISTANCE 180.f
#define GRIDSIZE 8.f
#define GRIDSIZESMALL 5.9f
#define MAXHEIGHT 30.f
#define SPEED 0.5f

//
// math functions
//

float hash( const float n ) {
  return fract(sin(n)*43758.54554213);
}
vec2 hash2( const float n ) {
  return fract(sin(vec2(n,n+1.))*vec2(43758.5453123));
}
vec2 hash2( const vec2 n ) {
  return fract(sin(vec2( n.x*n.y, n.x+n.y))*vec2(25.1459123,312.3490423));
}
vec3 hash3( const vec2 n ) {
  return fract(sin(vec3(n.x, n.y, n.x+n.y+2.0f))*vec3(36.5453123,43.1459123,11234.3490423));
}

//
// intersection functions
//

float intersectPlane( const vec3 ro, const vec3 rd, const float height) {
  if (rd.y==0.0) return 500.;
  float d = -(ro.y - height)/rd.y;
  if( d > 0. ) {
    return d;
  }
  return 500.;
}

float intersectUnitSphere ( const vec3 ro, const vec3 rd, const vec3 sph ) {
  vec3  ds = ro - sph;
  float bs = dot( rd, ds );
  float cs = dot( ds, ds ) - 1.0;
  float ts = bs*bs - cs;

  if( ts > 0.0 ) {
    ts = -bs - sqrt( ts );
    if( ts > 0. ) {
      return ts;
    }
  }
  return 500.;
}

//
// Scene
//

void getSphereOffset( const vec2 grid, vec2 & center ) {
  center = (hash2( grid+vec2(43.12,1.23) ) - vec2(0.5) )*(GRIDSIZESMALL);
}

void getMovingSpherePosition( const vec2 grid, const vec2 sphereOffset, vec3 & center, float & time ) {
  // falling?
  float s = 0.1+hash( grid.x*1.23114+5.342+74.324231*grid.y );
  float t = fract(14.*s + time/s*.3);

  float y =  s * MAXHEIGHT * abs( 4.*t*(1.-t) );
  vec2 offset = grid + sphereOffset;

  center = vec3( offset.x, y, offset.y ) + 0.5f*vec3( GRIDSIZE, 2.f, GRIDSIZE );
}

void getSpherePosition( const vec2 grid, const vec2 sphereOffset, vec3 & center ) {
  vec2 offset = grid + sphereOffset;
  center = vec3( offset.x, 0.f, offset.y ) + 0.5f*vec3( GRIDSIZE, 2.f, GRIDSIZE );
}

vec3 getSphereColor( const vec2 grid ) {
  vec3 col = hash3( grid+vec2(43.12*grid.y,12.23*grid.x) );
  return mix(col,col*col,.8);
}

vec3 getBackgroundColor( const vec3 ro, const vec3 rd ) {
  return 1.4f*mix(vec3(.5f),vec3(.7f,.9f,1), .5f+.5f*rd.y);
}

vec3 trace(const vec3 ro, const vec3 rd, vec3 & intersection, vec3 & normal, float & dist, int & material, const int steps, float & time) {
  dist = MAXDISTANCE;
  float distcheck;

  vec3 sphereCenter, col, normalcheck;

  material = 0;
  col = getBackgroundColor(ro, rd);

  if( (distcheck = intersectPlane( ro,  rd, 0.)) < MAXDISTANCE ) {
    dist = distcheck;
    material = 1;
    normal = vec3( 0., 1., 0. );
    col = vec3(.7);
  }

  // trace grid
  vec3 pos = floor(ro/GRIDSIZE)*GRIDSIZE;
  vec3 ri = 1.0f/rd;
  vec3 rs = sign(rd) * GRIDSIZE;
  vec3 dis = (pos-ro + 0.5f  * GRIDSIZE + rs*0.5f) * ri;
  vec3 mm = vec3(0.0);
  vec2 offset;

  for( int i=0; i<steps; i++ )	{
    if( material == 2 ||  distance( ro.xz(), pos.xz() ) > dist+GRIDSIZE ) break; {
      getSphereOffset( pos.xz(), offset );

      getMovingSpherePosition( pos.xz(), -offset, sphereCenter, time );
      if( (distcheck = intersectUnitSphere( ro, rd, sphereCenter )) < dist ) {
        dist = distcheck;
        normal = normalize((ro+rd*dist)-sphereCenter);
        col = getSphereColor(pos.xz());
        material = 2;
      }

      getSpherePosition( pos.xz(), offset, sphereCenter );
      if( (distcheck = intersectUnitSphere( ro, rd, sphereCenter )) < dist ) {
        dist = distcheck;
        normal = normalize((ro+rd*dist)-sphereCenter);
        col = getSphereColor(pos.xz()+vec2(1.,2.));
        material = 2;
      }
      mm = step(dis.xyz(), dis.zyx());
      dis += mm * rs * ri;
      pos += mm * rs;
    }
  }

  intersection = ro+rd*dist;

  return col;
}

vec3 cosWeightedRandomHemisphereDirection2( vec2 & rv2, const vec3 n ) {
  vec3  uu = normalize( cross( n, vec3(0.0,1.0,1.0) ) );
  vec3  vv = cross( uu, n );

  float ra = sqrt(rv2.y);
  float rx = ra*cos(6.2831*rv2.x);
  float ry = ra*sin(6.2831*rv2.x);
  float rz = sqrt( 1.0-rv2.y );
  vec3  rr = vec3( rx*uu + ry*vv + rz*n );

  return normalize( rr );
}

void mainImage(vec4 & fragColor, vec2 fragCoord, vec2 iResolution, vec2 iMouse, float iTime, float iFrame) {
  vec2 rv2;

  float time = iTime;
  vec2 q = fragCoord.xy()/iResolution.xy();
  vec2 p = -1.0f+2.0f*q;
  p.x *= iResolution.x/iResolution.y;

  vec3 col = vec3( 0. );

  // raytrace
  int material;
  vec3 normal, intersection;
  float dist;
  float seed = time+(p.x+iResolution.x*p.y)*1.51269341231;

  for( int j=0; j<SAMPLES + min(0.0f,iFrame); j++ ) {
    float fj = float(j);

    #ifdef MOTIONBLUR
    time = iTime + fj/(float(SAMPLES)*TARGETFPS);
    #endif

    rv2 = hash2( 24.4316544311*fj+time+seed );

    vec2 pt = p+rv2/(0.5f*iResolution.xy());

    // camera
    vec3 ro = vec3( cos( 0.232*time) * 10., 6.+3.*cos(0.3*time), GRIDSIZE*(time/SPEED) );
    vec3 ta = ro + vec3( -sin( 0.232*time) * 10., -2.0+cos(0.23*time), 10.0 );

    float roll = -0.15*sin(0.5*time);

    // camera tx
    vec3 cw = normalize( ta-ro );
    vec3 cp = vec3( sin(roll), cos(roll),0.0 );
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );

    #ifdef DEPTHOFFIELD
    // create ray with depth of field
    const float fov = 3.0;

    vec3 er = normalize( vec3( pt.xy(), fov ) );
    vec3 rd = er.x*cu + er.y*cv + er.z*cw;

    vec3 go = FOCUSBLUR*vec3( (rv2-vec2(0.5f))*2.f, 0.0f );
    vec3 gd = normalize( er*FOCUSDISTANCE - go );

    ro += go.x*cu + go.y*cv;
    rd += gd.x*cu + gd.y*cv;
    rd = normalize(rd);
    #else
    vec3 rd = normalize( pt.x*cu + pt.y*cv + 1.5*cw );
    #endif
    vec3 colsample = vec3( 1. );

    // first hit
    rv2 = hash2( (rv2.x*2.4543263+rv2.y)*(time+1.) );
    colsample *= trace(ro, rd, intersection, normal, dist, material, RAYCASTSTEPS, time);

    // bounces
    for( int i=0; i<(PATHDEPTH-1); i++ ) {
      if( material != 0 ) {
        rd = cosWeightedRandomHemisphereDirection2( rv2, normal );
        ro = intersection + EPSILON*rd;

        rv2 = hash2( (rv2.x*2.4543263+rv2.y)*(time+1.)+(float(i+1)*.23) );

        colsample *= trace(ro, rd, intersection, normal, dist, material, RAYCASTSTEPSRECURSIVE, time);
      }
    }
    colsample = sqrt(clamp(colsample, 0.f, 1.f));
    if( material == 0 ) {
      col += colsample;
    }
  }
  col  /= float(SAMPLES);

  fragColor = vec4( col,1.0);
}

#define WINDOW_WIDTH  384
#define WINDOW_HEIGHT 384

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "[vkFast] Sycl Ray Traced Motion Blur", 0, 0);
  #if defined(_WIN32)
  void * window_handle = (void *)glfwGetWin32Window(window);
  #elif defined(__linux__) && !defined(__ANDROID__)
  // NOTE(Constantine): this struct's layout is defined in redgpu_32.c file of REDGPU 2 SDK.
  struct X11WindowData {
    Display * display;
    Window    window;
    Atom      wmDeleteMessage;
  };
  struct X11WindowData windowData = {0};
  windowData.display = glfwGetX11Display();
  windowData.window = glfwGetX11Window(window);
  windowData.wmDeleteMessage = 0;
  REDGPU_2_EXPECTFL(windowData.display != NULL || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  REDGPU_2_EXPECTFL(windowData.window  != 0    || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  void * window_handle = &windowData;
  #endif

  gpu_handle_context_t ctx = vfContextInitNoDefaultAllocs(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] Sycl Ray Traced Motion Blur", WINDOW_WIDTH, WINDOW_HEIGHT, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  struct Pixels {
    unsigned char pixels[WINDOW_HEIGHT][WINDOW_WIDTH][4];
  };

  sycl::property_list sycl_queue_properties{sycl::property::queue::enable_profiling()};
  sycl::queue sycl_queue(sycl::gpu_selector_v, sycl_queue_properties);

  std::cout << "Running Sycl on device: " << sycl_queue.get_device().get_info<sycl::info::device::name>() << std::endl;

  // Create an output frame buffer using USM shared allocation
  struct Pixels * pixels = (struct Pixels *)sycl::malloc_shared(sizeof(struct Pixels), sycl_queue);

  VfeReBARMallocShared pixelsHandles = {};
  volatile struct Pixels * pix = (volatile struct Pixels *)vfeReBARMallocShared(ctx, sizeof(struct Pixels), &pixelsHandles);

  // Mouse state tracking
  double lastX = 0.0;
  double lastY = 0.0;
  int wasPressed = 0;

  vec2  iResolution = {WINDOW_WIDTH, WINDOW_HEIGHT};
  vec2  iMouse      = {0, 0};
  float iTime       = 0;
  float iFrame      = 0;

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    // Check if the Left Mouse Button is currently held down
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      double currentX = 0;
      double currentY = 0;
      glfwGetCursorPos(window, &currentX, &currentY);

      if (wasPressed) {
        // Calculate how far the mouse moved since the last frame
        double deltaX = currentX - lastX;
        double deltaY = currentY - lastY;

        // Convert pixel delta into coordinate scaling
        iMouse.x -= (float)(deltaX);
        iMouse.y -= (float)(deltaY);
      }

      // Save current positions for the next frame's comparison
      lastX = currentX;
      lastY = currentY;
      wasPressed = 1; // Mark that dragging is active
    } else {
      wasPressed = 0; // Reset state when button is released
    }

    iTime  += 0.01f;
    iFrame += 1;

    auto syclQueueSubmitEvent =sycl_queue.submit([&](sycl::handler& sycl_cgh) {
      sycl_cgh.parallel_for(sycl::range<2>(WINDOW_HEIGHT, WINDOW_WIDTH), [=](sycl::id<2> sycl_id) {
        int y = sycl_id[0];
        int x = sycl_id[1];

        float xf = (float)x;
        float yf = (float)y;

        vec4 color = {};
        vec2 fragCoord = {xf + 0.5f, (WINDOW_HEIGHT-yf) + 0.5f}; // https://registry.khronos.org/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml

        mainImage(color, fragCoord, iResolution, iMouse, iTime, iFrame);

        unsigned char r = (unsigned char)(color.r * 255.0f);
        unsigned char g = (unsigned char)(color.g * 255.0f);
        unsigned char b = (unsigned char)(color.b * 255.0f);
        unsigned char a = (unsigned char)(color.a * 255.0f);

        // NOTE(Constantine): pixels are in BGRA order.
        pixels->pixels[y][x][0] = b;
        pixels->pixels[y][x][1] = g;
        pixels->pixels[y][x][2] = r;
        pixels->pixels[y][x][3] = a;
      });
    });

    // Wait for execution to finish
    sycl_queue.wait();

    if (0) {
      auto end   = syclQueueSubmitEvent.get_profiling_info<sycl::info::event_profiling::command_end>();
      auto start = syclQueueSubmitEvent.get_profiling_info<sycl::info::event_profiling::command_start>();

      std::cout << "Sycl queue submit elapsed time: " << (end - start) / 1000.0 << " microseconds.\n";
    }

    // Copy pixels and draw
    memcpy((void *)&pix->pixels[0][0][0], (void *)&pixels->pixels[0][0][0], WINDOW_WIDTH * WINDOW_HEIGHT * 4);

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfAsyncDrawPixelsRaw(ctx, &pixelsHandles.storageRaw, NULL, 2, gpu_threads, array65536, FF, LL);

    glfwSwapBuffers(window);
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  // Cleanup
  sycl::free(pixels, sycl_queue);

  vfeReBARFreeShared(ctx, &pixelsHandles);

  vfGpuThreadDestroy(ctx, gpu_thread);

  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}
