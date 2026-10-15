// Hash function to generate a pseudo-random direction axis
vec3 hash3(float n) {
    return normalize(vec3(fract(sin(n)*43758.54f), fract(sin(n+1.f)*43758.54f), fract(sin(n+2.f)*43758.54f)) * 2.0f - 1.0f);
}

// 2D Edge Function (Pineda edge function algorithm) - Returns positive if p is to the right of edge v0->v1
float edgeFunction(vec2 v0, vec2 v1, vec2 p) {
    return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
}

void mainImage(uvec3 id, struct Pixels * screen, struct Texture * iChannel0, vec2 fragCoord, vec2 iResolution, vec2 iMouse, float iTime, float iFrame) {
    // Normalized device coordinates configuration
    vec2 p = (fragCoord - SWIZ(vec2(iResolution).xy) * 0.5f) / vec2(iResolution).y;

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

    // UV coordinates
    vec2 uvA = vec2(0, 0);
    vec2 uvB = vec2(1, 0);
    vec2 uvC = vec2(0.5f, 1);
    //vec2 uvA = vec2(0,0);
    //vec2 uvB = vec2(4,0);
    //vec2 uvC = vec2(2,4); // High tiling factor

    // Project 3D points onto the 2D ray plane (z-homogenous coordinate division)
    vec2 pA = SWIZ(a.xy) / a.z;
    vec2 pB = SWIZ(b.xy) / b.z;
    vec2 pC = SWIZ(c.xy) / c.z;

    // Compute sub-triangle areas for barycentric interpolation
    float area = edgeFunction(pA, pB, pC);

    // Evaluate edge equations for the current pixel coordinate 'p'
    float w0 = edgeFunction(pB, pC, p);
    float w1 = edgeFunction(pC, pA, p);
    float w2 = edgeFunction(pA, pB, p);

    vec4 outputColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Check if the pixel resides within the triangle boundaries (handling backface/clockwise orientation)
    if ((area >= 0.0f && w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) ||
        (area <  0.0f && w0 <= 0.0f && w1 <= 0.0f && w2 <= 0.0f)) {

        // Calculate true 2D Barycentric coordinates
        float invArea = 1.0f / area;
        float u = w0 * invArea;
        float v = w1 * invArea;
        float w = w2 * invArea;

        // Perspective-correct interpolation weights
        float invZ = u / a.z + v / b.z + w / c.z;
        u = (u / a.z) / invZ;
        v = (v / b.z) / invZ;
        w = (w / c.z) / invZ;

        // Interpolate UVs
        vec2 res = vec2(TEXTURE0_SIZE_WIDTH, TEXTURE0_SIZE_HEIGHT);
        ivec2 iRes = ivec2(res);

        vec2 UV = u * uvA + v * uvB + w * uvC;

        // UV tiling
        UV = fract(UV);

        // Bilinear texture filtering setup
        vec2 texCoord = UV * res - 0.5f;
        texCoord = clamp(texCoord, vec2(0.0f), res - 1.0f); // Clamp texCoord between 0 and the maximum resolution index
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

        outputColor = mix(mix(p00, p10, f.x), mix(p01, p11, f.x), f.y);

        // Nearest texture filtering
        //outputColor = texelFetch(iChannel0, ivec2(fract(u * uvA + v * uvB + w * uvC) * vec2(TEXTURE0_SIZE_WIDTH, TEXTURE0_SIZE_HEIGHT)), 0);
    }

    // Final color store
    imageStore(screen, ivec2(SWIZ(id.xy)), outputColor);
}
