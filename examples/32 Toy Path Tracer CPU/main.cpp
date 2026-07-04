#if 0
gcc -c ../../vkfast.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
g++ -O2 -fopenmp main.cpp *.o -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.4/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so
exit
#endif

// NOTE(Constantine):
// Based on "Ray Tracing - Primitives" code by Reinder Nijhoff (@reindernijhoff), 2019, MIT License: https://www.shadertoy.com/view/tl23Rm

#define GLM_FORCE_SWIZZLE

#include "../../vkfast.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLM
#include "../Common/vkfast_examples_common.h"

extern "C" void glDrawPixels(int width, int height, unsigned GL_RGBA_0x1908, unsigned GL_UNSIGNED_BYTE_0x1401, void * pixels);

using namespace glm;
typedef unsigned uint;

float rand(float n) {
  return fract(sin(n) * 43758.5453123f);
}

typedef union UnionConverter {
  uint  Uint;
  float Float;
} UnionConverter;

uint asuint(float x) {
  UnionConverter c = {};
  c.Float = x;
  return c.Uint;
}

float inversesqrt(float x) {
  return 1.0 / sqrt(x);
}

uint floatBitsToUint(float x) {
  return asuint(x);
}

uvec2 floatBitsToUint(vec2 a) {
  uint x = asuint(a.x);
  uint y = asuint(a.y);
  return uvec2(x, y);
}

float mod(float x, float y) {
  // https://stackoverflow.com/questions/7610631/glsl-mod-vs-hlsl-fmod
  // https://discussions.unity.com/t/translating-a-glsl-shader-noise-algorithm-to-hlsl-cg/672575/3
  return x - y * floor(x / y);
}

float dot2(vec3 v) {
  return dot(v, v);
}

#define MAX_DIST 1e10

// Plane
float iPlane(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec3 planeNormal, float planeDist) {
  float a = dot(rd, planeNormal);
  float d = -(dot(ro, planeNormal) + planeDist) / a;
  if (a > 0. || d < distBound.x || d > distBound.y) {
    return MAX_DIST;
  } else {
    normal = planeNormal;
    return d;
  }
}

// Sphere: https://www.shadertoy.com/view/4d2XWV
float iSphere(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, float sphereRadius) {
  float b = dot(ro, rd);
  float c = dot(ro, ro) - sphereRadius * sphereRadius;
  float h = b * b - c;
  if (h < 0.) {
    return MAX_DIST;
  } else {
    h = sqrt(h);
    float d1 = -b - h;
    float d2 = -b + h;
    if (d1 >= distBound.x && d1 <= distBound.y) {
      normal = normalize(ro + rd * d1);
      return d1;
    } else if (d2 >= distBound.x && d2 <= distBound.y) {
      normal = normalize(ro + rd * d2);
      return d2;
    } else {
      return MAX_DIST;
    }
  }
}

// Box: https://www.shadertoy.com/view/ld23DV
float iBox(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec3 boxSize) {
  vec3 m = sign(rd) / max(abs(rd), 1e-8f);
  vec3 n = m * ro;
  vec3 k = abs(m) * boxSize;

  vec3 t1 = -n - k;
  vec3 t2 = -n + k;

  float tN = max(max(t1.x, t1.y), t1.z);
  float tF = min(min(t2.x, t2.y), t2.z);

  if (tN > tF || tF <= 0.) {
    return MAX_DIST;
  } else {
    if (tN >= distBound.x && tN <= distBound.y) {
      float Ax = step(t1.y, t1.x);
      float Ay = step(t1.z, t1.y);
      float Az = step(t1.x, t1.z);

      float Bx = step(t1.z, t1.x);
      float By = step(t1.x, t1.y);
      float Bz = step(t1.y, t1.z);

      normal = -sign(rd) * vec3(Ax, Ay, Az) * vec3(Bx, By, Bz);
      return tN;
    } else if (tF >= distBound.x && tF <= distBound.y) {
      float Cx = step(t1.y, t1.x);
      float Cy = step(t1.z, t1.y);
      float Cz = step(t1.x, t1.z);

      float Dx = step(t1.z, t1.x);
      float Dy = step(t1.x, t1.y);
      float Dz = step(t1.y, t1.z);

      normal = -sign(rd) * vec3(Cx, Cy, Cz) * vec3(Dx, Dy, Dz);
      return tF;
    } else {
      return MAX_DIST;
    }
  }
}

// Capped Cylinder: https://www.shadertoy.com/view/4lcSRn
float iCylinder(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec3 pa, vec3 pb, float ra) {
  vec3 ca = pb-pa;
  vec3 oc = ro-pa;

  float caca = dot(ca,ca);
  float card = dot(ca,rd);
  float caoc = dot(ca,oc);

  float a = caca - card*card;
  float b = caca*dot( oc, rd) - caoc*card;
  float c = caca*dot( oc, oc) - caoc*caoc - ra*ra*caca;
  float h = b*b - a*c;

  if (h < 0.) return MAX_DIST;

  h = sqrt(h);
  float d = (-b-h)/a;

  float y = caoc + d*card;
  if (y > 0. && y < caca && d >= distBound.x && d <= distBound.y) {
    normal = (oc+d*rd-ca*y/caca)/ra;
    return d;
  }

  d = ((y < 0. ? 0. : caca) - caoc)/card;

  if( abs(b+a*d) < h && d >= distBound.x && d <= distBound.y) {
    normal = normalize(ca*sign(y)/caca);
    return d;
  } else {
    return MAX_DIST;
  }
}

// Torus: https://www.shadertoy.com/view/4sBGDy
float iTorus(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec2 torus) {
  // bounding sphere
  vec3 tmpnormal;
  if (iSphere(ro, rd, distBound, tmpnormal, torus.y + torus.x) > distBound.y) {
    return MAX_DIST;
  }

  float po = 1.0;

  float Ra2 = torus.x * torus.x;
  float ra2 = torus.y * torus.y;

  float m = dot(ro, ro);
  float n = dot(ro, rd);

#if 1
  float k = (m + Ra2 - ra2) / 2.0;
  float k3 = n;
  vec2 rd_xy = rd.xy();
  vec2 ro_xy = ro.xy();
  float A = dot(rd_xy, rd_xy);
  float B = dot(rd_xy, ro_xy);
  float C = dot(ro_xy, ro_xy);
  float k2 = n * n - Ra2 * A + k;
  float k1 = n * k - Ra2 * B;
  float k0 = k * k - Ra2 * C;
#else
  float k = (m - Ra2 - ra2) / 2.0;
  float k3 = n;
  float k2 = n * n + Ra2 * rd.z * rd.z + k;
  float k1 = k * n + Ra2 * ro.z * rd.z;
  float k0 = k * k + Ra2 * ro.z * ro.z - Ra2 * ra2;
#endif

#if 1
  // prevent |c1| from being too close to zero
  if (abs(k3 * (k3 * k3 - k2) + k1) < 0.01) {
    po = -1.0;
    float tmp = k1; k1 = k3; k3 = tmp;
    k0 = 1.0 / k0;
    k1 = k1 * k0;
    k2 = k2 * k0;
    k3 = k3 * k0;
  }
#endif

  // reduced cubic
  float c2 = k2 * 2.0 - 3.0 * k3 * k3;
  float c1 = k3 * (k3 * k3 - k2) + k1;
  float c0 = k3 * (k3 * (c2 + 2.0 * k2) - 8.0 * k1) + 4.0 * k0;

  c2 /= 3.0;
  c1 *= 2.0;
  c0 /= 3.0;

  float Q = c2 * c2 + c0;
  float R = c2 * c2 * c2 - 3.0 * c2 * c0 + c1 * c1;

  float h = R * R - Q * Q * Q;
  float t = MAX_DIST;

  if (h >= 0.0) {
    // 2 intersections
    h = sqrt(h);

    float v = sign(R + h) * pow(abs(R + h), 1.0 / 3.0); // cube root
    float u = sign(R - h) * pow(abs(R - h), 1.0 / 3.0); // cube root

    vec2 s = vec2((v + u) + 4.0 * c2, (v - u) * sqrt(3.0));

    float y = sqrt(0.5 * (length(s) + s.x));
    float x = 0.5 * s.y / y;
    float r = 2.0 * c1 / (x * x + y * y);

    float t1 =  x - r - k3; t1 = (po < 0.0) ? 2.0 / t1 : t1;
    float t2 = -x - r - k3; t2 = (po < 0.0) ? 2.0 / t2 : t2;

    if (t1 >= distBound.x) t = t1;
    if (t2 >= distBound.x) t = min(t, t2);
  } else {
    // 4 intersections
    float sQ = sqrt(Q);
    float w = sQ * cos(acos(-R / (sQ * Q)) / 3.0);

    float d2 = -(w + c2); if (d2 < 0.0) return MAX_DIST;
    float d1 = sqrt(d2);

    float h1 = sqrt(w - 2.0 * c2 + c1 / d1);
    float h2 = sqrt(w - 2.0 * c2 - c1 / d1);
    float t1 = -d1 - h1 - k3; t1 = (po < 0.0) ? 2.0 / t1 : t1;
    float t2 = -d1 + h1 - k3; t2 = (po < 0.0) ? 2.0 / t2 : t2;
    float t3 =  d1 - h2 - k3; t3 = (po < 0.0) ? 2.0 / t3 : t3;
    float t4 =  d1 + h2 - k3; t4 = (po < 0.0) ? 2.0 / t4 : t4;

    if (t1 >= distBound.x) t = t1;
    if (t2 >= distBound.x) t = min(t, t2);
    if (t3 >= distBound.x) t = min(t, t3);
    if (t4 >= distBound.x) t = min(t, t4);
  }

  if (t >= distBound.x && t <= distBound.y) {
    vec3 pos = ro + rd * t;
    normal = normalize(pos * (dot(pos, pos) - torus.y * torus.y - torus.x * torus.x * vec3(1, 1, -1)));
    return t;
  } else {
    return MAX_DIST;
  }
}

// Capsule: https://www.shadertoy.com/view/Xt3SzX
float iCapsule(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec3 pa, vec3 pb, float r) {
  vec3 ba = pb - pa;
  vec3 oa = ro - pa;

  float baba = dot(ba, ba);
  float bard = dot(ba, rd);
  float baoa = dot(ba, oa);
  float rdoa = dot(rd, oa);
  float oaoa = dot(oa, oa);

  float a = baba        - bard * bard;
  float b = baba * rdoa - baoa * bard;
  float c = baba * oaoa - baoa * baoa - r * r * baba;
  float h = b * b - a * c;
  if (h >= 0.) {
    float t = (-b - sqrt(h)) / a;
    float d = MAX_DIST;

    float y = baoa + t * bard;

    // body
    if (y > 0. && y < baba) {
      d = t;
    } else {
      // caps
      vec3 oc = (y <= 0.) ? oa : ro - pb;
      b = dot(rd, oc);
      c = dot(oc, oc) - r * r;
      h = b * b - c;
      if (h > 0.0) {
        d = -b - sqrt(h);
      }
    }
    if (d >= distBound.x && d <= distBound.y) {
      vec3 pa2 = ro + rd * d - pa;
      float h = clamp(dot(pa2, ba) / dot(ba, ba), 0.0f, 1.0f);
      normal = (pa2 - h * ba) / r;
      return d;
    }
  }
  return MAX_DIST;
}

// Capped Cone: https://www.shadertoy.com/view/llcfRf
float iCone(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec3  pa, vec3  pb, float ra, float rb) {
  vec3 ba = pb - pa;
  vec3 oa = ro - pa;
  vec3 ob = ro - pb;

  float m0 = dot(ba, ba);
  float m1 = dot(oa, ba);
  float m2 = dot(ob, ba);
  float m3 = dot(rd, ba);

  // caps
  if (m1 < 0.) {
    if (dot2(oa * m3 - rd * m1) < (ra * ra * m3 * m3) ) {
      float d = -m1 / m3;
      if (d >= distBound.x && d <= distBound.y) {
        normal = -ba * inversesqrt(m0);
        return d;
      }
    }
  } else if (m2 > 0.) {
    if (dot2(ob * m3 - rd * m2) < (rb * rb * m3 * m3)) {
      float d = -m2 / m3;
      if (d >= distBound.x && d <= distBound.y) {
        normal = ba * inversesqrt(m0);
        return d;
      }
    }
  }

  // body
  float m4 = dot(rd, oa);
  float m5 = dot(oa, oa);
  float rr = ra - rb;
  float hy = m0 + rr * rr;

  float k2 = m0 * m0      - m3 * m3 * hy;
  float k1 = m0 * m0 * m4 - m1 * m3 * hy + m0 * ra * (rr * m3 * 1.0          );
  float k0 = m0 * m0 * m5 - m1 * m1 * hy + m0 * ra * (rr * m1 * 2.0 - m0 * ra);

  float h = k1 * k1 - k2 * k0;
  if (h < 0.) return MAX_DIST;

  float t = (-k1 - sqrt(h)) / k2;

  float y = m1 + t * m3;
  if (y > 0. && y < m0 && t >= distBound.x && t <= distBound.y) {
    normal = normalize(m0 * (m0 * (oa + t * rd) + rr * ba * ra) - ba * hy * y);
    return t;
  } else {
    return MAX_DIST;
  }
}

// Ellipsoid: https://www.shadertoy.com/view/MlsSzn
float iEllipsoid(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec3 rad) {
  vec3 ocn = ro / rad;
  vec3 rdn = rd / rad;

  float a = dot(rdn, rdn);
  float b = dot(ocn, rdn);
  float c = dot(ocn, ocn);
  float h = b * b - a * (c - 1.);

  if (h < 0.) {
    return MAX_DIST;
  }

  float d = (-b - sqrt(h)) / a;

  if (d < distBound.x || d > distBound.y) {
    return MAX_DIST;
  } else {
    normal = normalize((ro + d*rd)/rad);
    return d;
  }
}

// Rounded Cone: https://www.shadertoy.com/view/MlKfzm
float iRoundedCone(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec3  pa, vec3  pb, float ra, float rb) {
  vec3  ba = pb - pa;
  vec3  oa = ro - pa;
  vec3  ob = ro - pb;
  float rr = ra - rb;
  float m0 = dot(ba, ba);
  float m1 = dot(ba, oa);
  float m2 = dot(ba, rd);
  float m3 = dot(rd, oa);
  float m5 = dot(oa, oa);
  float m6 = dot(ob, rd);
  float m7 = dot(ob, ob);

  float d2 = m0 - rr * rr;

  float k2 = d2      - m2 * m2;
  float k1 = d2 * m3 - m1 * m2 + m2 * rr * ra;
  float k0 = d2 * m5 - m1 * m1 + m1 * rr * ra * 2. - m0 * ra * ra;

  float h = k1 * k1 - k0 * k2;
  if (h < 0.0) {
    return MAX_DIST;
  }

  float t = (-sqrt(h) - k1) / k2;

  float y = m1 - ra * rr + t * m2;
  if (y > 0.0 && y < d2) {
    if (t >= distBound.x && t <= distBound.y) {
      normal = normalize(d2 * (oa + t * rd) - ba * y);
      return t;
    } else {
      return MAX_DIST;
    }
  } else {
    float h1 = m3 * m3 - m5 + ra * ra;
    float h2 = m6 * m6 - m7 + rb * rb;

    if (max(h1, h2) < 0.0) {
      return MAX_DIST;
    }

    vec3 n = vec3(0);
    float r = MAX_DIST;

    if (h1 > 0.) {
      r = -m3 - sqrt(h1);
      n = (oa + r * rd) / ra;
    }
    if (h2 > 0.) {
      t = -m6 - sqrt(h2);
      if (t < r) {
        n = (ob + t * rd) / rb;
        r = t;
      }
    }
    if (r >= distBound.x && r <= distBound.y) {
      normal = n;
      return r;
    } else {
      return MAX_DIST;
    }
  }
}

// Triangle: https://www.shadertoy.com/view/MlGcDz
float iTriangle(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec3 v0, vec3 v1, vec3 v2) {
  vec3 v1v0 = v1 - v0;
  vec3 v2v0 = v2 - v0;
  vec3 rov0 = ro - v0;

  vec3  n = cross(v1v0, v2v0);
  vec3  q = cross(rov0, rd);
  float d = 1.0f / dot(rd, n);
  float u = d * dot(-q, v2v0);
  float v = d * dot( q, v1v0);
  float t = d * dot(-n, rov0);

  if (u < 0. || v < 0. || (u + v) > 1. || t < distBound.x || t > distBound.y) {
    return MAX_DIST;
  } else {
    normal = normalize(-n);
    return t;
  }
}

// Sphere4: https://www.shadertoy.com/view/3tj3DW
float iSphere4(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, float ra) {
  // -----------------------------
  // solve quartic equation
  // -----------------------------

  float r2 = ra * ra;

  vec3 d2 = rd * rd; vec3 d3 = d2 * rd;
  vec3 o2 = ro * ro; vec3 o3 = o2 * ro;

  float ka = 1.0f / dot(d2,d2);

  float k0 = ka *  dot(ro, d3);
  float k1 = ka *  dot(o2, d2);
  float k2 = ka *  dot(o3, rd);
  float k3 = ka * (dot(o2, o2) - r2 * r2);

  // -----------------------------
  // solve cubic
  // -----------------------------

  float c0 = k1 - k0  * k0;
  float c1 = k2 + 2.0 * k0 * (k0 * k0 - (3.0 / 2.0) * k1);
  float c2 = k3 - 3.0 * k0 * (k0 * (k0 * k0 - 2.0 * k1) + (4.0 / 3.0) * k2);

  float p = c0 * c0 * 3.0 + c2;
  float q = c0 * c0 * c0 - c0 * c2 + c1 * c1;
  float h = q * q - p * p * p * (1.0 / 27.0);

  // -----------------------------
  // skip the case of 3 real solutions for the cubic, which involves
  // 4 complex solutions for the quartic, since we know this objcet is
  // convex
  // -----------------------------
  if (h < 0.0) {
    return MAX_DIST;
  }

  // one real solution, two complex (conjugated)
  h = sqrt(h);

  float s = sign(q + h) * pow(abs(q + h), 1.0 / 3.0); // cuberoot
  float t = sign(q - h) * pow(abs(q - h), 1.0 / 3.0); // cuberoot

  vec2 v = vec2((s + t) + c0 * 4.0, (s - t) * sqrt(3.0)) * 0.5f;

  // -----------------------------
  // the quartic will have two real solutions and two complex solutions.
  // we only want the real ones
  // -----------------------------

  float r = length(v);
  float d = -abs(v.y) / sqrt(r + v.x) - c1 / r - k0;

  if (d >= distBound.x && d <= distBound.y) {
    vec3 pos = ro + rd * d;
    normal = normalize(pos * pos * pos);
    return d;
  } else {
    return MAX_DIST;
  }
}

// Goursat: https://www.shadertoy.com/view/3lj3DW
float cuberoot(float x) {
  return sign(x) * pow(abs(x), 1.0 / 3.0);
}

float iGoursat(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, float ra, float rb) {
  // hole: x4 + y4 + z4 - (r2^2)=C2=B7(x2 + y2 + z2) + r1^4 = 0;
  float ra2 = ra * ra;
  float rb2 = rb * rb;

  vec3 rd2 = rd * rd; vec3 rd3 = rd2 * rd;
  vec3 ro2 = ro * ro; vec3 ro3 = ro2 * ro;

  float ka = 1.0f / dot(rd2, rd2);

  float k3 = ka * (dot(ro,  rd3));
  float k2 = ka * (dot(ro2, rd2) - rb2 / 6.0f);
  float k1 = ka * (dot(ro3, rd ) - rb2 * dot(rd, ro) / 2.0f);
  float k0 = ka * (dot(ro2, ro2) + ra2 * ra2 - rb2 * dot(ro, ro));

  float c2 = k2 - k3 * (k3);
  float c1 = k1 + k3 * (2.0 * k3 * k3 - 3.0 * k2);
  float c0 = k0 + k3 * (k3 * (c2 + k2) * 3.0 - 4.0 * k1);

  c0 /= 3.0;

  float Q = c2 * c2 + c0;
  float R = c2 * c2 * c2 - 3.0 * c0 * c2 + c1 * c1;
  float h = R * R - Q * Q * Q;

  // 2 intersections
  if (h>0.0) {
    h = sqrt(h);

    float s = cuberoot(R + h);
    float u = cuberoot(R - h);

    float x = s + u + 4.0 * c2;
    float y = s - u;

    float k2 = x * x + y * y * 3.0;

    float k = sqrt(k2);

    float d = -0.5 * abs(y) * sqrt(6.0 / (k + x)) - 2.0 * c1 * (k + x) / (k2 + x * k) - k3;

    if (d >= distBound.x && d <= distBound.y) {
      vec3 pos = ro + rd * d;
      normal = normalize(4.0f * pos * pos * pos - 2.0f * pos * rb * rb);
      return d;
    } else {
      return MAX_DIST;
    }
  } else {
    // 4 intersections
    float sQ = sqrt(Q);
    float z = c2 - 2.0 * sQ * cos(acos(-R / (sQ * Q)) / 3.0);

    float d1 = z     - 3.0 * c2;
    float d2 = z * z - 3.0 * c0;

    if (abs(d1) < 1.0e-4) {
      if (d2 < 0.0) return MAX_DIST;
      d2 = sqrt(d2);
    } else {
      if (d1 < 0.0) return MAX_DIST;
      d1 = sqrt(d1 / 2.0);
      d2 = c1 / d1;
    }

    //----------------------------------

    float h1 = sqrt(d1 * d1 - z + d2);
    float h2 = sqrt(d1 * d1 - z - d2);
    float t1 = -d1 - h1 - k3;
    float t2 = -d1 + h1 - k3;
    float t3 =  d1 - h2 - k3;
    float t4 =  d1 + h2 - k3;

    if (t2 < 0.0 && t4 < 0.0) return MAX_DIST;

    float result = 1e20;
    if      (t1 > 0.0) result = t1;
    else if (t2 > 0.0) result = t2;
    if      (t3 > 0.0) result = min(result, t3);
    else if (t4 > 0.0) result = min(result, t4);

    if (result >= distBound.x && result <= distBound.y) {
      vec3 pos = ro + rd * result;
      normal = normalize(4.0f * pos * pos * pos - 2.0f * pos * rb * rb);
      return result;
    } else {
      return MAX_DIST;
    }
  }
}

// Rounded Box: https://www.shadertoy.com/view/WlSXRW
float iRoundedBox(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal, vec3 size, float rad) {
  // bounding box
  vec3 m = 1.0f / rd;
  vec3 n = m * ro;
  vec3 k = abs(m) * (size + rad);
  vec3 t1 = -n - k;
  vec3 t2 = -n + k;
  float tN = max(max(t1.x, t1.y), t1.z);
  float tF = min(min(t2.x, t2.y), t2.z);
  if (tN > tF || tF < 0.0) {
    return MAX_DIST;
  }
  float t = (tN >= distBound.x && tN <= distBound.y) ? tN : (tF >= distBound.x && tF <= distBound.y) ? tF : MAX_DIST;

  // convert to first octant
  vec3 pos = ro + t * rd;
  vec3 s = sign(pos);
  vec3 ros = ro * s;
  vec3 rds = rd * s;
  pos *= s;

  // faces
  pos -= size;
  pos = max(pos.xyz(), pos.yzx());
  if (min(min(pos.x, pos.y), pos.z) < 0.0) {
    if (t >= distBound.x && t <= distBound.y) {
      vec3 p = ro + rd * t;
      normal = sign(p) * normalize(max(abs(p) - size, 0.0f));
      return t;
    }
  }

  // some precomputation
  vec3 oc = ros - size;
  vec3 dd = rds * rds;
  vec3 oo = oc * oc;
  vec3 od = oc * rds;
  float ra2 = rad * rad;

  t = MAX_DIST;

  // corner
  {
    float b = od.x + od.y + od.z;
    float c = oo.x + oo.y + oo.z - ra2;
    float h = b * b - c;
    if (h > 0.0) t = -b - sqrt(h);
  }

  // edge X
  {
    float a = dd.y + dd.z;
    float b = od.y + od.z;
    float c = oo.y + oo.z - ra2;
    float h = b * b - a * c;
    if (h > 0.0) {
      h = (-b - sqrt(h)) / a;
      if (h >= distBound.x && h < t && abs(ros.x + rds.x * h) < size.x) t = h;
    }
  }
  // edge Y
  {
    float a = dd.z + dd.x;
    float b = od.z + od.x;
    float c = oo.z + oo.x - ra2;
    float h = b * b - a * c;
    if (h > 0.0) {
      h = (-b - sqrt(h)) / a;
      if (h >= distBound.x && h < t && abs(ros.y + rds.y * h) < size.y) t = h;
    }
  }
  // edge Z
  {
    float a = dd.x + dd.y;
    float b = od.x + od.y;
    float c = oo.x + oo.y - ra2;
    float h = b * b - a * c;
    if (h > 0.0) {
      h = (-b - sqrt(h)) / a;
      if (h >= distBound.x && h < t && abs(ros.z + rds.z * h) < size.z) t = h;
    }
  }

  if (t >= distBound.x && t <= distBound.y) {
    vec3 p = ro + rd * t;
    normal = sign(p) * normalize(max(abs(p) - size, 1e-16f));
    return t;
  } else {
    return MAX_DIST;
  }
}

//
// Hash functions by Nimitz:
// https://www.shadertoy.com/view/Xt3cDn
//

uint baseHash(uvec2 p) {
  uint Ax = 1103515245U * (p.x >> 1U);
  uint Ay = 1103515245U * (p.y >> 1U);
  uint Bx = p.y;
  uint By = p.x;
  p.x = Ax ^ Bx;
  p.y = Ay ^ By;
  uint h32 = 1103515245U * ((p.x) ^ (p.y >> 3U));
  return h32 ^ (h32 >> 16);
}

float hash1(float & seed) {
  uint n = baseHash(floatBitsToUint(vec2(seed += .1, seed += .1)));
  return float(n) / float(0xffffffffU);
}

vec2 hash2(float & seed) {
  uint n = baseHash(floatBitsToUint(vec2(seed += .1, seed += .1)));
  uvec2 rz = uvec2(n, n * 48271U);
  vec2 A;
  A.x = rz.x & 0x7fffffffU;
  A.y = rz.y & 0x7fffffffU;
  return A / float(0x7fffffff);
}

//
// Ray tracer helper functions
//

float FresnelSchlickRoughness(float cosTheta, float F0, float roughness) {
  return F0 + (max((1.0f - roughness), F0) - F0) * pow(abs(1.0f - cosTheta), 5.0f);
}

vec3 cosWeightedRandomHemisphereDirection(const vec3 n, float & seed) {
  vec2 r = hash2(seed);
  vec3  uu = normalize(cross(n, abs(n.y) > .5 ? vec3(1., 0., 0.) : vec3(0., 1., 0.)));
  vec3  vv = cross(uu, n);
  float ra = sqrt(r.y);
  float rx = ra*cos(6.28318530718 * r.x);
  float ry = ra*sin(6.28318530718 * r.x);
  float rz = sqrt(1. - r.y);
  vec3  rr = vec3(rx * uu + ry * vv + rz * n);
  return normalize(rr);
}

vec3 modifyDirectionWithRoughness(const vec3 normal, const vec3 n, const float roughness, float & seed) {
  vec2 r = hash2(seed);

  vec3  uu = normalize(cross(n, abs(n.y) > .5 ? vec3(1., 0., 0.) : vec3(0., 1., 0.)));
  vec3  vv = cross(uu, n);

  float a = roughness * roughness;

  float rz = sqrt(abs((1.0 - r.y) / clamp(1. + (a - 1.) * r.y, .00001, 1.)));
  float ra = sqrt(abs(1. - rz * rz));
  float rx = ra*cos(6.28318530718 * r.x);
  float ry = ra*sin(6.28318530718 * r.x);
  vec3  rr = vec3(rx * uu + ry * vv + rz * n);

  vec3 ret = normalize(rr);
  return dot(ret, normal) > 0.0f ? ret : n;
}

vec2 randomInUnitDisk(float & seed) {
  vec2 h = hash2(seed) * vec2(1, 6.28318530718);
  float phi = h.y;
  float r = sqrt(h.x);
  return r * vec2(sin(phi), cos(phi));
}

// NOTE(Constantine): RENDER PART BEGIN

//
// Scene description
//

vec3 rotateY(const vec3 p, const float t) {
  float co = cos(t);
  float si = sin(t);
  vec2 xz = mat2(co, si, -si, co) * p.xz();
  return vec3(xz.x, p.y, xz.y);
}

vec3 opU(vec3 d, float iResult, float mat) {
  return (iResult < d.y) ? vec3(d.x, iResult, mat) : d;
}

float iMesh(vec3 ro, vec3 rd, vec2 distBound, vec3 & normal) {
  const vec3 tri0 = vec3(-2. / 3. * 0.43301270189, 0,   0);
  const vec3 tri1 = vec3( 1. / 3. * 0.43301270189, 0,  .25);
  const vec3 tri2 = vec3( 1. / 3. * 0.43301270189, 0, -.25);
  const vec3 tri3 = vec3( 0, 0.41079191812, 0);

  vec2 d = distBound;
  d.y = min(d.y, iTriangle(ro, rd, d, normal, tri0, tri1, tri2));
  d.y = min(d.y, iTriangle(ro, rd, d, normal, tri0, tri3, tri1));
  d.y = min(d.y, iTriangle(ro, rd, d, normal, tri2, tri3, tri0));
  d.y = min(d.y, iTriangle(ro, rd, d, normal, tri1, tri3, tri2));

  return d.y < distBound.y ? d.y : MAX_DIST;
}

vec3 worldhit(vec3 ro, vec3 rd, vec2 dist, vec3 & normal) {
  vec3 tmp0, tmp1, d = vec3(dist, 0.);

  d = opU(d, iPlane      (ro,                  rd, d.xy(), normal, vec3(0,1,0), 0.), 1.);
  d = opU(d, iBox        (ro-vec3( 1,.250, 0), rd, d.xy(), normal, vec3(.25)), 2.);
  d = opU(d, iSphere     (ro-vec3( 0,.250, 0), rd, d.xy(), normal, .25), 3.);
  d = opU(d, iCylinder   (ro,                  rd, d.xy(), normal, vec3(2.1,.1,-2), vec3(1.9,.5,-1.9), .08 ), 4.);
  d = opU(d, iCylinder   (ro-vec3( 1,.100,-2), rd, d.xy(), normal, vec3(0,0,0), vec3(0,.4,0), .1 ), 5.);
  d = opU(d, iTorus      (ro-vec3( 0,.250, 1), rd, d.xy(), normal, vec2(.2,.05)), 6.);
  d = opU(d, iCapsule    (ro-vec3( 1,.000,-1), rd, d.xy(), normal, vec3(-.1,.1,-.1), vec3(.2,.4,.2), .1), 7.);
  d = opU(d, iCone       (ro-vec3( 2,.200, 0), rd, d.xy(), normal, vec3(.1,0,0), vec3(-.1,.3,.1), .15, .05), 8.);
  d = opU(d, iRoundedBox (ro-vec3( 0,.250,-2), rd, d.xy(), normal, vec3(.15,.125,.15), .045), 9.);
  d = opU(d, iGoursat    (ro-vec3( 1,.275, 1), rd, d.xy(), normal, .16, .2), 10.);
  d = opU(d, iEllipsoid  (ro-vec3(-1,.300, 0), rd, d.xy(), normal, vec3(.2,.25, .05)), 11.);
  d = opU(d, iRoundedCone(ro-vec3( 2,.200,-1), rd, d.xy(), normal, vec3(.1,0,0), vec3(-.1,.3,.1), 0.15, 0.05), 12.);
  d = opU(d, iRoundedCone(ro-vec3(-1,.200,-2), rd, d.xy(), normal, vec3(0,.3,0), vec3(0,0,0), .1, .2), 13.);
  d = opU(d, iMesh       (ro-vec3( 2,.090, 1), rd, d.xy(), normal), 14.);
  d = opU(d, iSphere4    (ro-vec3(-1,.275,-1), rd, d.xy(), normal, .225), 15.);

  tmp1 = opU(d, iBox(rotateY(ro-vec3(0,.25,-1), 0.78539816339), rotateY(rd, 0.78539816339), d.xy(), tmp0, vec3(.1,.2,.1)), 16.);
  if (tmp1.y < d.y) {
    d = tmp1;
    normal = rotateY(tmp0, -0.78539816339);
  }

  return d;
}

//
// Palette by Inigo Quilez:
// https://www.shadertoy.com/view/ll2GD3
//
vec3 pal(float t, vec3 a, vec3 b, vec3 c, vec3 d) {
  return a + b * cos(6.28318530718f * (c * t + d));
}

float checkerBoard(vec2 p) {
  return mod(floor(p.x) + floor(p.y), 2.);
}

vec3 getSkyColor(vec3 rd) {
  // vec3 col = mix(vec3(1), vec3(.5, .7, 1), .5 + .5 * rd.y);
  vec3 col = vec3(0);
  col.r = mix(1.0f, 0.5f, 0.5f + 0.5f * rd.y);
  col.g = mix(1.0f, 0.7f, 0.5f + 0.5f * rd.y);
  col.b = mix(1.0f, 1.0f, 0.5f + 0.5f * rd.y);

  float sun = clamp(dot(normalize(vec3(-0.4f, 0.7f, -0.6f)), rd), 0.0f, 1.0f);
  col += vec3(1.0f, 0.6f, 0.1f) * (pow(sun, 4.0f) + 10.0f * pow(sun, 32.0f));
  return col;
}

#define LAMBERTIAN 0.0f
#define METAL 1.0f
#define DIELECTRIC 2.0f

float gpuIndepentHash(float p) {
  p = fract(p * .1031);
  p *= p + 19.19;
  p *= p + p;
  return fract(p);
}

void getMaterialProperties(vec3 pos, float mat, vec3 & albedo, float & type, float & roughness) {
  albedo = pal(mat*.59996323 + .5, vec3(.5), vec3(.5), vec3(1), vec3(0, .1, .2));

  if (mat < 1.5) {
    albedo = vec3(.25 + .25 * checkerBoard(pos.xz() * 5.0f));
    roughness = .75 * albedo.x - .15;
    type = METAL;
  } else {
    type = floor(gpuIndepentHash(mat + .3) * 3.);
    roughness = (1. - type * .475) * gpuIndepentHash(mat);
  }
}

//
// Simple ray tracer
//

float schlick(float cosine, float r0) {
  return r0 + (1. - r0) * pow((1. - cosine), 5.);
}

vec3 render(vec3 ro, vec3 rd, float & seed) {
  vec3 albedo, normal, col = vec3(1.);
  float roughness, type;

  #define PATH_LENGTH 12
  for (int i = 0; i < PATH_LENGTH; i += 1) {
    vec3 res = worldhit(ro, rd, vec2(.0001, 100), normal);
    if (res.z > 0.) {
      ro += rd * res.y;

      getMaterialProperties(ro, res.z, albedo, type, roughness);

      if (type < LAMBERTIAN + 0.5f) { // Added/hacked a reflection term
        float F = FresnelSchlickRoughness(max(0.0f, -dot(normal, rd)), 0.04f, roughness);
        if (F > hash1(seed)) {
          rd = modifyDirectionWithRoughness(normal, reflect(rd, normal), roughness, seed);
        } else {
          col *= albedo;
          rd = cosWeightedRandomHemisphereDirection(normal, seed);
        }
      } else if (type < METAL + 0.5f) {
        col *= albedo;
        rd = modifyDirectionWithRoughness(normal, reflect(rd,normal), roughness, seed);
      } else { // DIELECTRIC
        vec3 normalOut, refracted;
        float ni_over_nt, cosine, reflectProb = 1.;
        if (dot(rd, normal) > 0.0f) {
          normalOut = -normal;
          ni_over_nt = 1.4;
          cosine = dot(rd, normal);
          cosine = sqrt(1. - (1.4 * 1.4) - (1.4 * 1.4) * cosine * cosine);
        } else {
          normalOut = normal;
          ni_over_nt = 1. / 1.4;
          cosine = -dot(rd, normal);
        }

        // Refract the ray.
        refracted = refract(normalize(rd), normalOut, ni_over_nt);

        // Handle total internal reflection.
        if (refracted.x != 0 && refracted.y != 0 && refracted.z != 0) {
          float r0 = (1. - ni_over_nt) / (1. + ni_over_nt);
          reflectProb = FresnelSchlickRoughness(cosine, r0 * r0, roughness);
        }

        rd = hash1(seed) <= reflectProb ? reflect(rd, normalOut) : refracted;
        rd = modifyDirectionWithRoughness(-normalOut, rd, roughness, seed);
      }
    } else {
      col *= getSkyColor(rd);
      return col;
    }
  }
  return vec3(0);
}

mat3 setCamera(vec3 ro, vec3 ta, float cr) {
  vec3 cw = normalize(ta - ro);
  vec3 cp = vec3(sin(cr), cos(cr), 0.0);
  vec3 cu = normalize(cross(cw, cp));
  vec3 cv =          (cross(cu, cw));
  return mat3(cu, cv, cw);
}

// NOTE(Constantine): RENDER PART END

#define WINDOW_WIDTH  384
#define WINDOW_HEIGHT 384

// NOTE(Constantine): Dumb temporary workaround hacks to delete later:
vec2  iResolution = {WINDOW_WIDTH, WINDOW_HEIGHT};
vec2  iMouse      = {0, 0};
float iTime       = 0;

void mainImage(vec4 & fragColor, vec2 fragCoord) {
  if (0) {
    float screenScale = 1.0f;
    vec2 uv = fragCoord / screenScale;

    float frequency = 5.0f;
    vec2 pos = (uv + iTime * vec2(0.25f, 0.0f)) * frequency;

    vec2 center = floor(pos + vec2(0.5));

    float r0 = rand(center.x * 3.0f  + center.y * 7.0f);
    float r1 = rand(center.x * 7.0f  + center.y * 13.0f);
    float r2 = rand(center.x * 13.0f + center.y * 3.0f);

    float p = mix(0.0f, 4.0f, r0);
    float f = mix(5.0f, 8.0f, r1);

    float a = 0.5f * (1.0f + cos(iTime * f + p));

    float rad0 = mix(0.1, 0.4, r2);
    float rad1 = mix(0.2, 0.9, r0);

    float radius = 0.5f * mix(rad0, rad1, a);

    vec2 delta = pos - center;
    float distance = length(delta);

    fragColor = vec4(r0, r1, r2, 1.0f);

    if (distance > radius) {
      fragColor.x = 0.25f;
      fragColor.y = 0.25f;
      fragColor.z = 0.25f;
    }
  }

  //fragColor.xyz = vec3(1.0, 0.0, 0.0);
  //fragColor.w = 1.0f;

  //bool reset = 0;
  //if (iTime < 0.1) { reset = 1; fragColor.xyz = vec3(0.0, 1.0, 0.0); }

  vec2 mo = (iMouse.x == 0 && iMouse.y == 0) ?
             vec2(0.125f) :
             vec2(abs(iMouse.x), abs(iMouse.y)) / iResolution.xy() - 0.5f;

  //vec4 data = texelFetch(iChannel0, ivec2(0), 0);
  //if (round(mo * iResolution.xy) != round(data.yz) || round(data.w) != round(iResolution.x)) {
  //    reset = true;
  //}

  vec3 ro = vec3(.5 + 2.5 * cos(1.5 + 6. * mo.x), 1. + 2. * mo.y, -.5 + 2.5 * sin(1.5 + 6. * mo.x));
  vec3 ta = vec3(.5, -.4, -.5);
  mat3 ca = setCamera(ro, ta, 0.);
  vec3 normal;

  //float fpd = data.x;
  //if(all(equal(ivec2(fragCoord), ivec2(0)))) {
  //  // Calculate focus plane.
  //  float nfpd = worldhit(ro, normalize(vec3(.5, 0, -.5) - ro), vec2(0, 100), normal).y;
  //  fragColor = vec4(nfpd, mo * iResolution.xy, iResolution.x);
  //} else {
    vec2 iResolutionNeg = iResolution;
    iResolutionNeg.x *= -1.0f;
    iResolutionNeg.y *= -1.0f;
    vec2 p = (iResolutionNeg.xy() + 2.0f * fragCoord - 1.0f) / iResolution.y;
    //p.y *= -1.0f;
    float seed = float(baseHash(floatBitsToUint(p - iTime))) / float(0xffffffffU);

    // AA
    p += 2.0f * hash2(seed) / iResolution.y;
    vec3 rd = ca * normalize(vec3(p.xy(), 1.6f));

    // DOF
    vec3 fp = ro + rd * 2.0f/*fpd*/; // NOTE(Constantine): fpd: Focus Plane Distance.
    ro = ro + ca * vec3(randomInUnitDisk(seed), 0.0f) * 0.02f;
    rd = normalize(fp - ro);

    vec3 col = render(ro, rd, seed);

    // NOTE(Constantine): COLOR CORRECTION BEGIN

    // gamma correction
    col = max(vec3(0), col - 0.004f);
    col = (col * (6.2f * col + 0.5f)) / (col * (6.2f * col + 1.7f) + 0.06f);

    // NOTE(Constantine): COLOR CORRECTION END

    //if (reset) {
      fragColor = vec4(col, 1);
    //} else {
    //   fragColor = vec4(col, 1) + texelFetch(iChannel0, ivec2(fragCoord), 0);
    //}
  //}
}

int main() {
#if defined(__MINGW32__)
  SetProcessDPIAware();
#elif defined(_WIN32)
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "[vkFast] Toy Path Tracer CPU", 0, 0);
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

  gpu_handle_context_t ctx = vfContextInitEx(1, 1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] Toy Path Tracer CPU", WINDOW_WIDTH, WINDOW_HEIGHT, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  unsigned char pixels[WINDOW_HEIGHT][WINDOW_WIDTH][4] = {
    255,0,0,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,
    0,255,0,255,  0,255,0,255,  0,255,0,255,  0,255,0,255,
    0,0,255,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,
  };
  float pixelsSamples[WINDOW_HEIGHT][WINDOW_WIDTH][4] = {};
  int sampleCount = 0;

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    {
      double mouse_x_position = 0.0;
      double mouse_y_position = 0.0;
      glfwGetCursorPos(window, &mouse_x_position, &mouse_y_position);
      if (iMouse.x != mouse_x_position || iMouse.y != mouse_y_position) {
        sampleCount = 0;
        #pragma omp parallel for
        for (int y = 0; y < WINDOW_HEIGHT; y += 1) {
          #pragma omp parallel for
          for (int x = 0; x < WINDOW_WIDTH; x += 1) {
            pixelsSamples[y][x][0] = 0;
            pixelsSamples[y][x][1] = 0;
            pixelsSamples[y][x][2] = 0;
            pixelsSamples[y][x][3] = 0;
          }
        }
      }
      iMouse.x = mouse_x_position;
      iMouse.y = mouse_y_position;
    }
    iTime += 0.01f;
    // NOTE(Constantine): Enable "Project properties -> C/C++ -> Language -> Open MP Support"
    #pragma omp parallel for
    for (int y = 0; y < WINDOW_HEIGHT; y += 1) {
      #pragma omp parallel for
      for (int x = 0; x < WINDOW_WIDTH; x += 1) {
        float xf = (float)x;
        float yf = (float)y;

        vec4 color;
        vec2 fragCoord = {xf + 0.5f, (WINDOW_HEIGHT-yf) + 0.5f}; // https://registry.khronos.org/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml
        mainImage(color, fragCoord);

        char r = (char)(color.r * 255.0f);
        char g = (char)(color.g * 255.0f);
        char b = (char)(color.b * 255.0f);
        char a = (char)(color.a * 255.0f);

        pixelsSamples[y][x][0] += color.r;
        pixelsSamples[y][x][1] += color.g;
        pixelsSamples[y][x][2] += color.b;
        pixelsSamples[y][x][3] += color.a;
      }
    }

    #pragma omp parallel for
    for (int y = 0; y < WINDOW_HEIGHT; y += 1) {
      #pragma omp parallel for
      for (int x = 0; x < WINDOW_WIDTH; x += 1) {
        // NOTE(Constantine): BGRA
        pixels[y][x][2] = (char)((pixelsSamples[y][x][0] / (float)(sampleCount+1)) * 255.0f);
        pixels[y][x][1] = (char)((pixelsSamples[y][x][1] / (float)(sampleCount+1)) * 255.0f);
        pixels[y][x][0] = (char)((pixelsSamples[y][x][2] / (float)(sampleCount+1)) * 255.0f);
        pixels[y][x][3] = (char)((pixelsSamples[y][x][3] / (float)(sampleCount+1)) * 255.0f);
       }
    }
    sampleCount += 1;

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfDrawPixels(ctx, pixels, NULL, 2, gpu_threads, array65536, FF, LL);

    glfwSwapBuffers(window);
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);

  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}
