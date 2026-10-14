#define iChannel0 NULL

// Hash function to generate a pseudo-random direction axis
vec3 hash3(float n) {
    return normalize(vec3(fract(sin(n)*43758.54f), fract(sin(n+1.f)*43758.54f), fract(sin(n+2.f)*43758.54f)) * 2.0f - 1.0f);
}

void mainImage(uvec3 id, vec2 fragCoord) {

    vec2 p = (fragCoord - SWIZ(vec2(iResolution).xy) * 0.5f) / vec2(iResolution).y;
    vec3 d = normalize(vec3(p, 1));
    vec3 o = vec3(0, 0, -2);

    // 1. Create an animated quaternion from a random axis and time-based angle
    float seed = 42.0;
    vec3 axis = hash3(seed);    // Static random 3D rotation axis
    float angle = iTime * 0.4f; // Rotation angle updating every frame

    vec4 q = vec4(axis * sin(angle * 0.5f), cos(angle * 0.5f));

    // 2. Convert Quaternion to a 3x3 Rotation Matrix
    mat3 R = mat3(
        1.f - 2.f*(q.y*q.y + q.z*q.z),       2.f*(q.x*q.y - q.z*q.w),       2.f*(q.x*q.z + q.y*q.w),
              2.f*(q.x*q.y + q.z*q.w), 1.f - 2.f*(q.x*q.x + q.z*q.z),       2.f*(q.y*q.z - q.x*q.w),
              2.f*(q.x*q.z - q.y*q.w),       2.f*(q.y*q.z + q.x*q.w), 1.f - 2.f*(q.x*q.x + q.y*q.y)
    );

    // 3. Define base local vertices and rotate them via the matrix
    vec3 a = R * vec3(-1,-0.7f, 0);
    vec3 b = R * vec3( 1,-0.7f, 0);
    vec3 c = R * vec3( 0, 0.9f, 0);

    // Offset the rotated triangle into view (depth Z = 1.4f + animation)
    vec3 offset = vec3(0, 0, 1.4f + cos(iTime * 0.5f) * 1.6f);
    a += offset;
    b += offset;
    c += offset;

    vec2 uvA = vec2(0, 0);
    vec2 uvB = vec2(1, 0);
    vec2 uvC = vec2(0.5f, 1);
    //vec2 uvA = vec2(0,0);
    //vec2 uvB = vec2(4,0);
    //vec2 uvC = vec2(2,4); // High tiling factor

    // Ray-Triangle Plane Intersection
    vec3 e1 = b - a;
    vec3 e2 = c - a;
    vec3 n = cross(e1, e2);
    float t = dot(a - o, n) / dot(d, n);
    vec3 q_pos = o + d * t;

    // Barycentric Coordinates
    float area = dot(n, n);
    float u = dot(cross(c - b, q_pos - b), n) / area;
    float v = dot(cross(a - c, q_pos - c), n) / area;
    float w = 1.0 - u - v;

    vec4 outputColor = vec4(0, 0, 0, 1);

    if (t > 0.0f && u >= 0.0f && v >= 0.0f && w >= 0.0f) {
        vec2 res = vec2(SWIZ(iChannelResolution[0].xy));
        ivec2 iRes = ivec2(res);

        vec2 UV = u * uvA + v * uvB + w * uvC;

        vec2 texCoord = UV * res - 0.5f;
        ivec2 base = ivec2(floor(texCoord));
        vec2 f = fract(texCoord);

        ivec2 c00 = /*imod*/(base + ivec2(0,0)) % iRes;
        ivec2 c10 = /*imod*/(base + ivec2(1,0)) % iRes;
        ivec2 c01 = /*imod*/(base + ivec2(0,1)) % iRes;
        ivec2 c11 = /*imod*/(base + ivec2(1,1)) % iRes;

        vec4 p00 = texelFetch(iChannel0, c00, 0);
        vec4 p10 = texelFetch(iChannel0, c10, 0);
        vec4 p01 = texelFetch(iChannel0, c01, 0);
        vec4 p11 = texelFetch(iChannel0, c11, 0);

        // Bilinear filtering:
        vec4 sampleColor = mix(mix(p00, p10, f.x), mix(p01, p11, f.x), f.y);

        // Nearest filtering:
        //vec4 sampleColor = texelFetch(iChannel0, ivec2(fract(u * uvA + v * uvB + w * uvC) * vec2(SWIZ(iChannelResolution[0].xy))), 0);

        outputColor = sampleColor;
    } else {
        // Background sample color
        outputColor = vec4(0, 0, 0, 1);
    }

    // Final color store
    imageStore(screen, ivec2(SWIZ(id.xy)), outputColor);
}
