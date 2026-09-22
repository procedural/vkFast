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

#define PI 3.1415926535f
#define MAX_FLOAT 1e20f
#define SAMPLES_PER_FRAME 1
#define DIFFUSE_BOUNCE 20
#define SMOKE_DENSITY 0.01f

#define MAT_ISOTROPIC 4
#define MAT_LIGHT 3
#define MAT_GLASS 2
#define MAT_METAL 1
#define MAT_LAMBERTIAN 0

#define CONSTANT_MEDIUM_BOX 0
#define CONSTANT_MEDIUM_SPHERE 1

#define RANDOM_SPHERE 20

#define FOCUS_DIST 10.0f
#define DEFOCUS_ANGLE 0.0f
#define VFOV 40.0f
#define CAMERA_ORIGIN vec3(278, 278, -800)
#define CAMERA_TARGET vec3(278, 278, 0)

vec3 mod289(vec3 x) { return x-floor(x*(1.f/289.f))*289.f; }

vec3 permute(vec3 x) { return mod289((34.0f*x+1.f)*x); }

vec4 mod289(vec4 x) { return x - floor(x * (1.0f/289.0f)) * 289.0f; }

vec4 permute(vec4 x) { return mod289((34.0f*x + 1.0f) * x); }

float hash11(float p)
{
  p = fract(p * .1031f);
  p *= p + 33.33f;
  p *= p + p;
  return fract(p);
}

float hash13(vec3 p3)
{
  p3  = fract(p3 * .1031f);
  p3 += dot(p3, p3.zyx() + 33.33f);
  return fract((p3.x + p3.y) * p3.z);
}

// https://www.shadertoy.com/view/XlXcW4
vec3 hash3f(vec3 s) {
  uvec3 r = floatBitsToUint(s);
  r = ((r >> 16u) ^ r.yzx()) * 1111111111u;
  r = ((r >> 16u) ^ r.yzx()) * 1111111111u;
  r = ((r >> 16u) ^ r.yzx()) * 1111111111u;
  return vec3(r) / float(-1u);
}

vec3 fade(vec3 t) { return t*t*t*(t*(t*6.0f - 15.0f) + 10.0f); }

vec4 rand01_from_hash(vec4 h)
{
  return fract(h * (1.0f / 41.0f));
}

vec3 grad3_from_hash(float h)
{
  float x = fract(h * (1.0f/41.0f)) * 2.0f - 1.0f;
  float y = fract(h * (1.0f/53.0f)) * 2.0f - 1.0f;
  float z = fract(h * (1.0f/61.0f)) * 2.0f - 1.0f;

  vec3 g = vec3(x, y, z);

  return normalize(g + 1e-6f);
}

// https://suricrasia.online/blog/shader-functions/
vec3 i_uniformSphereBlackle(vec3 xi) {
  return normalize(xi * 2.0f - 1.0f);
}

// 3D Value Noise
float valueNoise3(vec3 p)
{
  vec3 Pi = floor(p);
  vec3 Pf = fract(p);
  vec3 f  = fade(Pf);

  vec4 ix = vec4(Pi.x, Pi.x+1.0f, Pi.x,      Pi.x+1.0f);
  vec4 iy = vec4(Pi.y, Pi.y,      Pi.y+1.0f, Pi.y+1.0f);
  vec4 iz0 = vec4(Pi.z);        // z
  vec4 iz1 = vec4(Pi.z + 1.0f);  // z+1

  // h = permute( permute(ix) + iy ) + iz
  vec4 h0 = permute(permute(permute(ix) + iy) + iz0);
  vec4 h1 = permute(permute(permute(ix) + iy) + iz1);

  vec4 v0 = rand01_from_hash(h0); // z
  vec4 v1 = rand01_from_hash(h1); // z+1

  // interp
  float x00 = mix(v0.x, v0.y, f.x);
  float x10 = mix(v0.z, v0.w, f.x);
  float y0  = mix(x00, x10, f.y);

  float x01 = mix(v1.x, v1.y, f.x);
  float x11 = mix(v1.z, v1.w, f.x);
  float y1  = mix(x01, x11, f.y);

  return mix(y0, y1, f.z);
}

float perlin13(vec3 p)
{
  vec3 Pi = floor(p);
  vec3 Pf = fract(p);
  vec3 w  = fade(Pf);

  vec2 e = vec2(1.0f,0.0);

  // Corner gradient
  vec3 g000 = i_uniformSphereBlackle( hash3f(mod289(Pi+e.yyy())) );
  vec3 g100 = i_uniformSphereBlackle( hash3f(mod289(Pi+e.xyy())) );
  vec3 g010 = i_uniformSphereBlackle( hash3f(mod289(Pi+e.yxy())) );
  vec3 g110 = i_uniformSphereBlackle( hash3f(mod289(Pi+e.xxy())) );

  vec3 g001 = i_uniformSphereBlackle( hash3f(mod289(Pi+e.yyx())) );
  vec3 g101 = i_uniformSphereBlackle( hash3f(mod289(Pi+e.xyx())) );
  vec3 g011 = i_uniformSphereBlackle( hash3f(mod289(Pi+e.yxx())) );
  vec3 g111 = i_uniformSphereBlackle( hash3f(mod289(Pi+e.xxx())) );

  // Dot product of weights (Pf - cornerOffset)
  float n000 = dot(g000, Pf-e.yyy());
  float n100 = dot(g100, Pf-e.xyy());
  float n010 = dot(g010, Pf-e.yxy());
  float n110 = dot(g110, Pf-e.xxy());

  float n001 = dot(g001, Pf-e.yyx());
  float n101 = dot(g101, Pf-e.xyx());
  float n011 = dot(g011, Pf-e.yxx());
  float n111 = dot(g111, Pf-e.xxx());

  // Trilinear interpolation
  float nx00 = mix(n000, n100, w.x);
  float nx10 = mix(n010, n110, w.x);
  float nxy0 = mix(nx00, nx10, w.y);

  float nx01 = mix(n001, n101, w.x);
  float nx11 = mix(n011, n111, w.x);
  float nxy1 = mix(nx01, nx11, w.y);

  float nxyz = mix(nxy0, nxy1, w.z);

  return nxyz * 1.2f;
}

float turb(vec3 p, int depth) {
  float accum = 0.0f;
  vec3 temp_p = p;
  float weight = 1.0f;

  for (int i = 0; i < depth; i++) {
    accum += weight * perlin13(temp_p);
    weight *= 0.5f;
    temp_p *= 2.0f;
  }

  return abs(accum);
}

mat2 rotate(float angle) {
  float s = sin(angle);
  float c = cos(angle);
  return mat2(c, -s, s, c);
}


// translation
mat4 translate(vec3 m)
{
  return mat4(1.0f,0.0f,0.0f,0.0f,
              0.0f,1.0f,0.0f,0.0f,
              0.0f,0.0f,1.0f,0.0f,
              m.x,m.y,m.z,1.0f);
}


mat4 rotationAxisAngle( vec3 v, float angle )
{
  float s = sin( angle );
  float c = cos( angle );
  float ic = 1.0f - c;

  return mat4( v.x*v.x*ic + c,     v.y*v.x*ic - s*v.z, v.z*v.x*ic + s*v.y, 0.0f,
               v.x*v.y*ic + s*v.z, v.y*v.y*ic + c,     v.z*v.y*ic - s*v.x, 0.0f,
               v.x*v.z*ic - s*v.y, v.y*v.z*ic + s*v.x, v.z*v.z*ic + c,     0.0f,
               0.0f,               0.0f,               0.0f,               1.0f );
}

mat4 rotateTranslate(vec3 v, float angle, vec3 m)
{
  float rad = angle/180.0f*PI;
  return translate(m)*rotationAxisAngle(v,rad);
}

struct Material
{
  int type;
  float fuzz;
  float ir;
  vec3 albedo;
  vec3 emission;
};

struct sphere
{
  Material material;
  vec4 sp;
};

struct quad
{
  Material material;
  vec3 q;
  vec3 u;
  vec3 v;
};

struct cube
{
  Material material;
  vec3 a;
  vec3 b;
};

struct hit
{
  bool front_face;
  float t;
  vec3 n;
  vec3 p;
  Material material;
};

const int object_num = 5;

const Material materials[] = {
  {MAT_LAMBERTIAN, 1.0f, 0.0f, vec3(0.65, 0.05, 0.05), vec3(0.0)},
  {MAT_LAMBERTIAN, 1.0f, 0.0f, vec3(0.73, 0.73, 0.73), vec3(0.0)},
  {MAT_LAMBERTIAN, 1.0f, 0.0f, vec3(0.12, 0.45, 0.15), vec3(0.0)},
  {MAT_LIGHT, 1.0f, 0.0f, vec3(0.0), vec3(7)},
  {MAT_GLASS, 1.0f, 1.5, vec3(1), vec3(0.0)},
  {MAT_GLASS, 1.0f, 1.0/1.5, vec3(1), vec3(0.0)},
  {MAT_ISOTROPIC, 1.0f, 0.0f, vec3(0), vec3(0.0)},
  {MAT_ISOTROPIC, 1.0f, 0.0f, vec3(1), vec3(0.0)},
  {MAT_ISOTROPIC, 1.0f, 0.0f, vec3(0.2,0.8,1), vec3(0.0)}
};

const sphere objects[] = {
  {materials[8], vec4(402.5, 80.0,  147.5, 80.0)},
  {materials[5], vec4(182.5, 245.0, 147.5, 75.0)}
};

const quad quadObjects[] = {
  {materials[2], vec3(555,0,0), vec3(0,555,0), vec3(0,0,555)},
  {materials[0], vec3(0,0,0), vec3(0,555,0), vec3(0,0,555)},
  {materials[1], vec3(0,0,0), vec3(555,0,0), vec3(0,0,555)},
  {materials[1], vec3(555,555,555), vec3(-555,0,0), vec3(0,0,-555)},
  {materials[1], vec3(0,0,555), vec3(555,0,0), vec3(0,555,0)}
};

const cube cubeObjects[] = {
  {materials[1], vec3(0, 0, 0), vec3(165, 330, 165)},
  {materials[1], vec3(0, 0, 0), vec3(165, 165, 165)},
  {materials[4], vec3(0, 165, 0), vec3(165, 330, 165)}
};

vec2 random_in_unit_disk(vec3 & seed) {
  vec2 p;
  for(int i=0; i<3; i++) {
    seed = hash3f(seed);
    p = 2.0f * seed.xy() - 1.0f;
    if(dot(p,p) < 1.0) return p;
  }
  return normalize(p);
}

vec4 iSphere(vec3 ro, vec3 rd, vec4 sph, vec2 tm) {
  vec3 oc = ro - sph.xyz();
  float b = dot(oc, rd);
  float h = b*b - dot(oc, oc) + sph.w*sph.w;

  if (h < 0.0) return vec4(1e20);

  h = sqrt(h);

  float t = -b - h;
  if (t > tm.x && t < tm.y) {
    vec3 p = ro + t * rd;
    vec3 n = normalize(p-sph.xyz());
    return vec4(t,n);
  }
  t = -b + h;
  if (t > tm.x && t < tm.y) {
    vec3 p = ro + t * rd;
    vec3 n = normalize(p-sph.xyz());
    return vec4(t,n);
  }

  return vec4(1e20);
}

vec4 iQuad(vec3 ro, vec3 rd, vec3 q, vec3 u, vec3 v, vec2 tm)
{
  vec3 n = cross(u,v);
  float D = dot(n,q);
  vec3 w = n/dot(n,n);
  float denom = dot(n,rd);
  if(abs(denom) < 1e-6) return vec4(1e20);
  float t = (D-dot(n,ro))/dot(n,rd);
  vec3 pos = ro + t*rd;
  vec3 p = pos - q;
  float a = dot(w,cross(p,v));
  float b = dot(w,cross(u,p));
  if (a > 1.0 || a < 0.0 || b > 1.0 || b < 0.0) return vec4(1e20);
  if (t > tm.x && t < tm.y) return vec4(t,normalize(n));
  return vec4(1e20);
}

vec4 iBox(vec3 ro, vec3 rd, vec3 center, vec3 rad, mat4 l2wMatrix, vec2 tm)
{
  mat4 w2lMatrix = inverse(l2wMatrix);
  vec3 roo = (w2lMatrix*vec4(ro,1.0)).xyz()-center;
  vec3 rdd = (w2lMatrix*vec4(rd,0.0)).xyz();

  vec3 m = 1.0f/rdd;
  vec3 n = roo*m;
  vec3 k = rad*abs(m);
  vec3 t1 = -n-k;
  vec3 t2 = -n+k;

  float t1max = max(t1.x,max(t1.y,t1.z));
  float t2min = min(t2.x,min(t2.y,t2.z));

  if(t2min < t1max || t2min < 0.0) return vec4(1e20);

  vec4 res = vec4( t1max,step(vec3(t1max),t1) );

  if(res.x > tm.x && res.x < tm.y)
  {
    res.yzw() = normalize((l2wMatrix * vec4(-sign(rdd)*res.yzw(),0.0)).xyz());
    return res;
  }

  res = vec4( t2min,step(t2,vec3(t2min)) );
  if(res.x > tm.x && res.x < tm.y)
  {
    res.yzw() = normalize((l2wMatrix * vec4(-sign(rdd)*res.yzw(),0.0)).xyz());
    return res;
  }
  return vec4(1e20);
}

void getQube(cube c, quad * box)
{
  vec3 a = c.a,b=c.b;
  vec3 pa = vec3( min(a.x,b.x),min(a.y,b.y),min(a.z,b.z) );
  vec3 pb = vec3( max(a.x,b.x),max(a.y,b.y),max(a.z,b.z) );
  vec3 dx = vec3(pb.x-pa.x,0,0);
  vec3 dy = vec3(0,pb.y-pa.y,0);
  vec3 dz = vec3(0,0,pb.z-pa.z);

  Material mat1 = c.material;
  box[0] = {mat1,vec3(pa.x,pa.y,pa.z),dx,dy}; //front
  box[1] = {mat1,vec3(pb.x,pa.y,pa.z),dz,dy}; //right
  box[2] = {mat1,vec3(pa.x,pa.y,pb.z),-dz,dy}; //left
  box[3] = {mat1,vec3(pb.x,pa.y,pb.z),-dx,dy}; // back
  box[4] = {mat1,vec3(pa.x,pa.y,pb.z),dx,-dz}; // bottom
  box[5] = {mat1,vec3(pa.x,pb.y,pa.z),dx,dz}; // top
}

void hit_cube(vec3 ro, vec3 rd, cube c, vec3 offset, float rotY, vec2 tm, hit & rec)
{
  float a = rotY/180.0*PI;
  mat2 rotateMatrix = rotate(a);
  //world->local
  vec3 localRo = ro - offset;
  localRo.xz() = rotateMatrix*localRo.xz();
  vec3 localRd = rd;
  localRd.xz() = rotateMatrix*localRd.xz();

  quad box[6];
  getQube(c, box);

  for(int j=0; j<6; j++) {
    vec4 d = iQuad(localRo, localRd, box[j].q, box[j].u, box[j].v, vec2(tm.x, rec.t));

    if (d.x < rec.t) {
      rec.t = d.x;
      rec.material = box[j].material;

      vec3 localNormal = d.yzw();

      rec.n = localNormal;
      //local->world
      rec.n.xz() = rotate(-a)*rec.n.xz();
    }
  }
}

// moke and Fog Box/Sphere
float hit_constantMedium(int type, vec3 ro, vec3 rd, vec3 center, vec3 rad, vec3 axis, vec4 tr, vec2 tm, vec3 & seed) {
  mat4 matrix = rotateTranslate(axis, tr.w, tr.xyz());

  vec4 h1,h2;
  if(CONSTANT_MEDIUM_BOX == type){
    h1 = iBox(ro, rd, center, rad, matrix, vec2(-MAX_FLOAT, MAX_FLOAT));
  }
  else if(CONSTANT_MEDIUM_SPHERE == type){
    h1 = iSphere(ro, rd, vec4(center,rad.x), vec2(-MAX_FLOAT, MAX_FLOAT));
  }

  if (h1.x >= 1e19) return MAX_FLOAT;

  if(CONSTANT_MEDIUM_BOX == type){
    h2 = iBox(ro, rd, center, rad, matrix, vec2(h1.x + 1e-4, MAX_FLOAT));
  }
  else if(CONSTANT_MEDIUM_SPHERE == type){
    h2 = iSphere(ro, rd, vec4(center,rad.x), vec2(h1.x + 1e-4, MAX_FLOAT));
  }
  if (h2.x >= 1e19) return MAX_FLOAT;

  float t1 = h1.x;
  float t2 = h2.x;

  t1 = max(t1, tm.x);
  t2 = min(t2, tm.y);
  if (t1 >= t2) return MAX_FLOAT;
  if (t1 < 0.0) t1 = 0.0;

  float distInside = t2 - t1;

  float density = SMOKE_DENSITY;
  seed = hash3f(seed);
  float u = clamp(seed.x, 1e-6f, 1.0f);
  float hitDist = (-1.0 / density) * log(u);

  if (hitDist >= distInside) return MAX_FLOAT;

  return t1 + hitDist;
}

// Confirm the hit object among all the objects in the scene.
void hit_list(vec3 ro, vec3 rd, vec2 tm, hit & rec, vec3 & seed) {
  rec.t = tm.y;
  rec.material.type = -1; // no hit

  // Quad
  for(int i=0; i<object_num; i++) {
    vec4 d = iQuad(ro, rd, quadObjects[i].q, quadObjects[i].u, quadObjects[i].v, vec2(tm.x, rec.t));
    if (d.x < rec.t) {
      rec.t = d.x;
      rec.n = d.yzw();
      rec.material = quadObjects[i].material;
    }
  }

  // Emission
  vec4 d1 = iQuad(ro, rd, vec3(113, 554, 127), vec3(330,0,0), vec3(0,0,305), vec2(tm.x, rec.t));
  if (d1.x < rec.t) {
    rec.t = d1.x;
    rec.n = d1.yzw();
    rec.material = materials[3];
  }

  // Cube

  //vec4 d3 = iBox(ro,rd,vec3(82.5,165,82.5),vec3(82.5,165,82.5),rotateTranslate(vec3(0,1,0),-15.0f,vec3(265, 0, 295)),vec2(tm.x, rec.t));
  //if (d3.x < rec.t) { rec.t = d3.x;rec.n = d3.yzw;rec.material = materials[1]; }

  //d3 = iBox(ro,rd,vec3(82.5),vec3(82.5),rotateTranslate(vec3(0,1,0),18.0f,vec3(330, 0, 105)),vec2(tm.x, rec.t));
  //if (d3.x < rec.t) { rec.t = d3.x;rec.n = d3.yzw;rec.material = materials[7]; }

  float d2 = hit_constantMedium(CONSTANT_MEDIUM_BOX,ro,rd,vec3(82.5,165.001,82.5),vec3(82.5,165,82.5), vec3(0,1,0), vec4(265, 0, 295, -15.0), vec2(tm.x,rec.t), seed);
  if (d2 < rec.t) { rec.t = d2;rec.n = vec3(1,0,0);rec.material = materials[6]; }
  d2 = hit_constantMedium(CONSTANT_MEDIUM_BOX,ro,rd,vec3(82.5,82.501,82.5),vec3(82.5), vec3(0,1,0), vec4(130, 0, 65, 18.0), vec2(tm.x,rec.t), seed);
  if (d2 < rec.t) { rec.t = d2;rec.n = vec3(1,0,0);rec.material = materials[7]; }

  d2 = hit_constantMedium(CONSTANT_MEDIUM_SPHERE,ro,rd,objects[0].sp.xyz(),vec3(objects[0].sp.w), vec3(0,1,0), vec4(130, 0, 65, 18.0), vec2(tm.x,rec.t), seed);
  if (d2 < rec.t) { rec.t = d2;rec.n = vec3(1,0,0);rec.material = objects[0].material; }

  if (rec.t < tm.y) {
    rec.p = ro + rec.t * rd;
    rec.front_face = dot(rec.n, rd) < 0.0;
    rec.n = rec.front_face ? rec.n : -rec.n;
  }
}

// random scattering: Cosine-Weighted
vec3 random_on_hemisphere(vec3 n, vec3 u) {
  float r = sqrt(u.x);
  float phi = 2.0*PI*u.y;

  float x = r * cos(phi);
  float y = r * sin(phi);
  float z = sqrt(max(0.0f, 1.0f - u.x)); // cos(theta)

  vec3 local = vec3(x, y, z);

  vec3 up = abs(n.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
  vec3 t = normalize(cross(up, n));
  vec3 b = cross(n, t);
  return t*local.x + b*local.y + n*local.z;
}

vec3 random_in_unit_sphere_dir(vec3 & seed) {
  float z = seed.x*2.0-1.0;
  float r = sqrt(1.0-z*z);
  float phi = 2.0*PI*seed.y;

  float x = r * cos(phi);
  float y = r * sin(phi);

  return normalize(vec3(x,y,z));
}

float reflectance(float cosine, float rindex)
{
  float r0 = (1.0-rindex)/(1.0+rindex);
  r0 = r0*r0;
  return r0 + (1.0-r0)*pow(1.0-cosine,5.0);
}

vec3 getDir(vec3 rd, hit rec, vec3 & seed)
{
  seed = hash3f(seed);
  vec3 dir = vec3(0.0);
  vec3 n = rec.n;
  int type = rec.material.type;
  float fuzz = rec.material.fuzz;
  if(type == MAT_LAMBERTIAN)
  {
    dir = random_in_unit_sphere_dir(seed)+n;
    dir = length(dir)<1e-6 ? n : dir;
  }
  else if(type == MAT_ISOTROPIC)
  {
    dir = random_in_unit_sphere_dir(seed);
  }
  else if(type == MAT_METAL)
  {
    dir = rd - 2.0f*dot(rd,n)*n;
    dir = normalize(dir) + fuzz*random_on_hemisphere(n, seed);
  }
  else if(type == MAT_GLASS)
  {
    float refract_index = rec.material.ir;
    float ri = rec.front_face ? 1.0/refract_index : refract_index;
    float cos_theta = min(dot(-rd,n),1.0f);
    float sin_theta = sqrt(1.0-cos_theta*cos_theta);
    vec3 refracted = refract(rd, n, ri);
    bool isTIR = length(refracted) < 1e-6;
    if (isTIR || reflectance(cos_theta, ri) > seed.x)
    {
      dir = rd - 2.0f*dot(rd,n)*n;
    }
    else
    {
      dir = refracted;
    }
  }
  return normalize(dir);
}

vec4 render(vec2 fragCoord, vec2 iResolution, vec2 iMouse, float iFrame) {
  vec3 seed = hash3f(vec3(fragCoord, iFrame));
  vec3 color = vec3(0.0);

  vec2 mo = iMouse.xy()/iResolution.xy();

  vec3 ro = CAMERA_ORIGIN;
  vec3 target = CAMERA_TARGET;
  vec3 zz = normalize(target - ro);
  vec3 xx = normalize(cross(zz, vec3(0.0f, 1.0f, 0.0)));
  vec3 yy = cross(xx, zz);

  float v_size = 2.0*FOCUS_DIST*tan(VFOV*0.5/180.0*PI);
  float h_size = v_size * iResolution.x/iResolution.y;
  vec3 viewport_u = xx * h_size;
  vec3 viewport_v = yy * v_size;
  vec3 delta_u = viewport_u/iResolution.x;
  vec3 delta_v = viewport_v/iResolution.y;

  vec3 viewport_upleft = ro + FOCUS_DIST*zz - (viewport_u + viewport_v)*0.5f;
  vec3 pixel00_loc = viewport_upleft + 0.5f*(delta_u + delta_v);

  // Generating Pixels with Multiple Samples
  for(int i=0; i < SAMPLES_PER_FRAME; i++)
  {
    seed = hash3f(seed);
    vec2 jitter = seed.xy()-0.5f;

    vec3 pixel_sample = pixel00_loc + (fragCoord.x+jitter.x)*delta_u + (fragCoord.y+jitter.y)*delta_v;

    float defocus_radius = FOCUS_DIST * tan(DEFOCUS_ANGLE*0.5 /180.0*PI);
    vec3 defocus_disk_u = xx * defocus_radius;
    vec3 defocus_disk_v = yy * defocus_radius;

    vec2 randCircle = random_in_unit_disk(seed);
    vec3 curRo = DEFOCUS_ANGLE<=0.0 ? ro : ro + (randCircle.x)*defocus_disk_u + (randCircle.y)*defocus_disk_v;
    vec3 curRd = normalize(pixel_sample - curRo);

    // ray bounces into scene
    vec3 randRo = curRo, randRd = curRd, intensity = vec3(1.0), col = vec3(0.);

    for(int j=0; j < DIFFUSE_BOUNCE; j++)
    {
      hit r;
      hit_list(randRo, randRd, vec2(1e-3, 1e20), r, seed);

      if(r.material.type >= 0)
      {
        col += intensity*r.material.emission;
        if(r.material.type == MAT_LIGHT)
        {
          break;
        }
        intensity *= r.material.albedo;
        randRo = r.p;
        randRd = getDir(randRd, r, seed);
        if(dot(intensity, intensity) < 1e-4) break;
        if(length(randRd) <= 1e-6) break;
      }
      else
      {
        //vec3 sky = (vec3(1.0), vec3(0.5, 0.7, 1.0), randRd.y * 0.5 + 0.5);
        vec3 sky = vec3(0.0);
        col += sky * intensity;
        break;
      }
    }

    color += col;
  }

  color /= float(SAMPLES_PER_FRAME);

  return vec4(color, 1.0);
}

void mainImage(vec4 & fragColor, vec2 fragCoord, vec2 iResolution, vec2 iMouse, float iTime) {
  vec2 p = fragCoord.xy() / iResolution.xy() * 2.0f - 1.0f;
  p.x *= iResolution.x / iResolution.y;

  vec4 tex = render(fragCoord, iResolution, iMouse, iTime);

  vec3 color = tex.rgb() / tex.a;

  // NOTE(Constantine): Should do this after the final samples accumulation.
  //color = pow(color, vec3(0.4545f));

  fragColor = vec4(color, 1);
}

#define WINDOW_WIDTH  384
#define WINDOW_HEIGHT 384

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "[vkFast] Sycl The Next Week Volumes", 0, 0);
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
  vfWindowFullscreen(ctx, window_handle, "[vkFast] Sycl The Next Week Volumes", WINDOW_WIDTH, WINDOW_HEIGHT, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  struct Pixels {
    unsigned char pixels[WINDOW_HEIGHT][WINDOW_WIDTH][4];
  };

  struct PixelsSamples {
    float pixels[WINDOW_HEIGHT][WINDOW_WIDTH][4];
  };

  sycl::property_list sycl_queue_properties{sycl::property::queue::enable_profiling()};
  sycl::queue sycl_queue(sycl::gpu_selector_v, sycl_queue_properties);

  std::cout << "Running Sycl on device: " << sycl_queue.get_device().get_info<sycl::info::device::name>() << std::endl;

  // Create an output frame buffer using USM shared allocation
  struct Pixels * pixels = (struct Pixels *)sycl::malloc_shared(sizeof(struct Pixels), sycl_queue);
  struct PixelsSamples * pixelsSamples = (struct PixelsSamples *)sycl::malloc_shared(sizeof(struct PixelsSamples), sycl_queue);

  VfeReBARMallocShared pixelsHandles = {};
  volatile struct Pixels * pix = (volatile struct Pixels *)vfeReBARMallocShared(ctx, sizeof(struct Pixels), &pixelsHandles);

  int sampleCount = 0;

  // Mouse state tracking
  double lastX = 0.0;
  double lastY = 0.0;
  int wasPressed = 0;

  vec2  iResolution = {WINDOW_WIDTH, WINDOW_HEIGHT};
  vec2  iMouse      = {0, 0};
  float iTime       = 0;

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

        // Clear previous samples
        {
          sampleCount = 0;
          #pragma omp parallel for
          for (int y = 0; y < WINDOW_HEIGHT; y += 1) {
            #pragma omp parallel for
            for (int x = 0; x < WINDOW_WIDTH; x += 1) {
              pixelsSamples->pixels[y][x][0] = 0;
              pixelsSamples->pixels[y][x][1] = 0;
              pixelsSamples->pixels[y][x][2] = 0;
              pixelsSamples->pixels[y][x][3] = 0;
            }
          }
        }
      }

      // Save current positions for the next frame's comparison
      lastX = currentX;
      lastY = currentY;
      wasPressed = 1; // Mark that dragging is active
    } else {
      wasPressed = 0; // Reset state when button is released
    }

    iTime += 0.01f;

    sycl_queue.submit([&](sycl::handler& sycl_cgh) {
      sycl_cgh.parallel_for(sycl::range<2>(WINDOW_HEIGHT, WINDOW_WIDTH), [=](sycl::id<2> sycl_id) {
        int y = sycl_id[0];
        int x = sycl_id[1];

        float xf = (float)x;
        float yf = (float)y;

        vec4 color = {};
        vec2 fragCoord = {xf + 0.5f, (WINDOW_HEIGHT-yf) + 0.5f}; // https://registry.khronos.org/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml

        mainImage(color, fragCoord, iResolution, iMouse, iTime);

        unsigned char r = (unsigned char)(color.r * 255.0f);
        unsigned char g = (unsigned char)(color.g * 255.0f);
        unsigned char b = (unsigned char)(color.b * 255.0f);
        unsigned char a = (unsigned char)(color.a * 255.0f);

        pixelsSamples->pixels[y][x][0] += color.r;
        pixelsSamples->pixels[y][x][1] += color.g;
        pixelsSamples->pixels[y][x][2] += color.b;
        pixelsSamples->pixels[y][x][3] += color.a;
      });
    });

    // Wait for execution to finish
    sycl_queue.wait();

    sampleCount += 1;

    #pragma omp parallel for
    for (int y = 0; y < WINDOW_HEIGHT; y += 1) {
      #pragma omp parallel for
      for (int x = 0; x < WINDOW_WIDTH; x += 1) {
        // NOTE(Constantine): pow(x, 0.4545f) for gamma correction.
        unsigned char r = (unsigned char)(pow(pixelsSamples->pixels[y][x][0] / (float)(sampleCount), 0.4545f) * 255.0f);
        unsigned char g = (unsigned char)(pow(pixelsSamples->pixels[y][x][1] / (float)(sampleCount), 0.4545f) * 255.0f);
        unsigned char b = (unsigned char)(pow(pixelsSamples->pixels[y][x][2] / (float)(sampleCount), 0.4545f) * 255.0f);
        unsigned char a = (unsigned char)(pow(pixelsSamples->pixels[y][x][3] / (float)(sampleCount), 0.4545f) * 255.0f);

        // NOTE(Constantine): pixels are in BGRA order.
        pixels->pixels[y][x][0] = b;
        pixels->pixels[y][x][1] = g;
        pixels->pixels[y][x][2] = r;
        pixels->pixels[y][x][3] = a;
      }
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
  sycl::free(pixelsSamples, sycl_queue);

  vfeReBARFreeShared(ctx, &pixelsHandles);

  vfGpuThreadDestroy(ctx, gpu_thread);

  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}
