#define _screen NULL
#define _texture0 NULL

// Hash function to generate a pseudo-random direction axis
vec3 hash3(float n) {
    return normalize(vec3(fract(sin(n)*43758.54f), fract(sin(n+1.f)*43758.54f), fract(sin(n+2.f)*43758.54f)) * 2.0f - 1.0f);
}

void mainImage(uvec3 id, vec2 fragCoord) {
    // 1. Animated Quaternion & Rotation Matrix Setup
    float seed = 42.0f;
    vec3 axis = hash3(seed);
    float angle = iTime * 0.4f;
    vec4 q = vec4(axis * sin(angle * 0.5f), cos(angle * 0.5f));

    mat3 R = mat3(
        1.f - 2.f*(q.y*q.y + q.z*q.z),       2.f*(q.x*q.y - q.z*q.w),       2.f*(q.x*q.z + q.y*q.w),
              2.f*(q.x*q.y + q.z*q.w), 1.f - 2.f*(q.x*q.x + q.z*q.z),       2.f*(q.y*q.z - q.x*q.w),
              2.f*(q.x*q.z - q.y*q.w),       2.f*(q.y*q.z + q.x*q.w), 1.f - 2.f*(q.x*q.x + q.y*q.y)
    );

    // 2. Define vertices and oscillate depth cleanly
    vec3 a = R * vec3(-1,-0.7f, 0), b = R * vec3(1,-0.7f, 0), c = R * vec3(0, 0.9f, 0);
    vec3 offset = vec3(0, 0, 1.4f + cos(iTime * 0.5f) * 1.6f);
    a += offset; b += offset; c += offset;

    vec2 uvA = vec2(0,0), uvB = vec2(1,0), uvC = vec2(.5,1);
    //vec2 uvA = vec2(0,0), uvB = vec2(4,0), uvC = vec2(2,4); // High tiling factor

    vec3 o = vec3(0, 0, -2);
    vec2 res = vec2(SWIZ(iChannelResolution[0].xy));
    ivec2 iRes = ivec2(res);

    vec4 colorAccum = vec4(0.0);
    float totalWeight = 0.0;

    // 3. High-Quality 4x4 Grid Super-Sampling (SSAA)
    // Completely unrolls a sub-pixel integration window to filter high-frequency noise
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            // Compute sub-pixel offsets spanning from -0.375 to +0.375 inside the pixel
            vec2 subPixelOffset = (vec2(float(i), float(j)) - 1.5f) * 0.25f;

            // Generate unique, aspect-corrected ray direction for this sub-sample
            vec2 p = (fragCoord + subPixelOffset - SWIZ(vec2(iResolution).xy) * 0.5f) / vec2(iResolution).y;
            vec3 d = normalize(vec3(p, 1));

            float weight = (1.0f - abs(subPixelOffset.x)) * (1.0f - abs(subPixelOffset.y));

            // Ray-Triangle Plane Intersection
            vec3 e1 = b - a, e2 = c - a, n = cross(e1, e2);
            float t = dot(a - o, n) / dot(d, n);
            vec3 q_pos = o + d * t;

            // Barycentric Coordinates
            float area = dot(n, n);
            float u = dot(cross(c - b, q_pos - b), n) / area;
            float v = dot(cross(a - c, q_pos - c), n) / area;
            float w = 1.0f - u - v;

            if (t > 0.0f && u >= 0.0f && v >= 0.0f && w >= 0.0f) {
                vec4 sampleColor = imageLoad(_texture0, ivec2(fract(u * uvA + v * uvB + w * uvC) * vec2(SWIZ(iChannelResolution[0].xy))), 0);

                colorAccum += sampleColor * weight;
                totalWeight += weight;
            } else {
                // Background sample accumulation
                float weight = (1.0f - abs(subPixelOffset.x)) * (1.0f - abs(subPixelOffset.y));
                colorAccum += vec4(0.0f, 0.0f, 0.0f, 1.0f) * weight;
                totalWeight += weight;
            }
        }
    }

    // Final normalized color emit
    imageStore(_screen, ivec2(SWIZ(id.xy)), colorAccum / totalWeight);
}
