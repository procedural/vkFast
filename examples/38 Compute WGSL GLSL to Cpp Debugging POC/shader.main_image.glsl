//#version 310 es

//layout(binding = 0, rgba8) uniform highp writeonly image2D screen;
//layout(binding = 1, std140)
//uniform time_block_1_ubo {
//  uvec4 inner[1];
//} v;
float hash(vec2 p) {
  vec2 q = fract((p * vec2(12.98980045318603515625f, 78.233001708984375f)));
  return fract((sin(dot(q, vec2(12.98980045318603515625f, 78.233001708984375f))) * 43758.546875f));
}
float noise(vec2 p) {
  vec2 i = floor(p);
  vec2 f = fract(p);
  vec2 u = ((f * f) * (3.0f - (2.0f * f)));
  float a = hash((i + vec2(0.0f)));
  float b = hash((i + vec2(1.0f, 0.0f)));
  float c = hash((i + vec2(0.0f, 1.0f)));
  float d = hash((i + vec2(1.0f)));
  return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}
float fbm(vec2 p) {
  float v_1 = (noise(p) * 0.5f);
  float v_2 = (v_1 + (noise((p * 2.0f)) * 0.25f));
  return (v_2 + (noise((p * 4.0f)) * 0.125f));
}
void main_image_inner(uvec3 id) {
  uvec2 screen_size = uvec2(imageSize(screen));
  bool v_3 = false;
  if ((id.x >= screen_size.x)) {
    v_3 = true;
  } else {
    v_3 = (id.y >= screen_size.y);
  }
  if (v_3) {
    return;
  }
  float v_4 = (float(id.x) + 0.5f);
  vec2 fragCoord = vec2(v_4, (float((screen_size.y - id.y)) - 0.5f));
  float v_5 = float(screen_size.x);
  vec2 r = vec2(v_5, float(screen_size.y));
  vec2 uv = (((fragCoord + fragCoord) - r) / r.y);
  mat2 rotMatrix = mat2(vec2(3.0f, 4.0f), vec2(4.0f, -3.0f));
  vec2 p = ((uv * rotMatrix) / 100.0f);
  //uvec4 v_6 = v.inner[0u];
  float t = iTime;//uintBitsToFloat(v_6.x);
  vec4 S = vec4(0.0f);
  vec4 C = vec4(1.0f, 2.0f, 3.0f, 0.0f);
  vec4 W = vec4(0.0f);
  float T = ((0.10000000149011611938f * t) + p.y);
  {
    uvec2 tint_loop_idx = uvec2(4294967295u);
    float i = 1.0f;
    while(true) {
      if (all(equal(tint_loop_idx, uvec2(0u)))) {
        break;
      }
      if ((i <= 30.0f)) {
      } else {
        break;
      }
      float randomSize = fract((sin((i * 137.3000030517578125f)) * 43758.546875f));
      float randomLength = fract((sin((i * 246.8000030517578125f)) * 43758.546875f));
      float phaseX = (((i * ((C.x + 8.0f) + i)) + T) + T);
      float phaseY = (((i * ((C.z + 8.0f) + i)) + T) + T);
      vec2 v_7 = (0.01999999955296516418f * vec2(cos(phaseX), cos(phaseY)));
      p = (p + v_7);
      W = (sin(i) * C);
      vec4 colorFactor = (cos(W) + vec4(1.0f));
      float brightness = exp(sin((i + (i * T))));
      vec2 v_8 = (p / exp(W.x));
      vec2 sampleUV = (v_8 + (vec2(i, t) / 8.0f));
      float cloudDensity = fbm((sampleUV * 100.0f));
      float tailLength = mix(10.0f, 50.0f, randomLength);
      float trailStretch = max((cloudDensity * tailLength), 0.00999999977648258209f);
      vec2 v_9 = p;
      vec2 stretchedP = (v_9 / vec2(2.0f, trailStretch));
      vec2 maxP = max(p, stretchedP);
      maxP.x = (maxP.x * 1.60000002384185791016f);
      float glowRadius = mix(4000.0f, 25000.0f, randomSize);
      float pLength = max(length(maxP), 0.00000099999999747524f);
      S = (S + (((colorFactor * brightness) / pLength) / glowRadius));
      {
        uint tint_low_inc = (tint_loop_idx.x - 1u);
        tint_loop_idx.x = tint_low_inc;
        uint tint_carry = uint((tint_low_inc == 4294967295u));
        tint_loop_idx.y = (tint_loop_idx.y - tint_carry);
        i = (i + 1.0f);
      }
      continue;
    }
  }
  vec3 col = SWIZ(tanh((S * S)).xyz);
  vec4 v_10 = vec4(col, 1.0f);
  imageStore(screen, ivec2(SWIZ(id.xy)), v_10);
}
//layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
//void main() {
//  main_image_inner(gl_GlobalInvocationID);
//}
