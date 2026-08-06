@group(0) @binding(0) var screen: texture_storage_2d<rgba8unorm, write>;

struct Time {
  elapsed: f32,
};

// Bind the buffer to group 0, binding 0
@group(0) @binding(1) var<uniform> time: Time;

// ==========================================
// 1. 高质量体积云雾生成器
// ==========================================
fn hash(p: vec2f) -> f32 {
    let q = fract(p * vec2f(12.9898, 78.233));
    return fract(sin(dot(q, vec2f(12.9898, 78.233))) * 43758.5453);
}

// 平滑噪波 (Value Noise)
fn noise(p: vec2f) -> f32 {
    let i = floor(p);
    let f = fract(p);
    let u = f * f * (3.0 - 2.0 * f);

    let a = hash(i + vec2f(0.0, 0.0));
    let b = hash(i + vec2f(1.0, 0.0));
    let c = hash(i + vec2f(0.0, 1.0));
    let d = hash(i + vec2f(1.0, 1.0));

    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

// 分形布朗运动 (fBm)
fn fbm(p: vec2f) -> f32 {
    return noise(p) * 0.5 + noise(p * 2.0) * 0.25 + noise(p * 4.0) * 0.125;
}

// ==========================================
// 2. 主计算着色器 (Compute Shader)
// ==========================================
@compute @workgroup_size(16, 16)
fn main_image(@builtin(global_invocation_id) id: vec3u) {
    let screen_size = textureDimensions(screen);
    if (id.x >= screen_size.x || id.y >= screen_size.y) { return; }

    let fragCoord = vec2f(f32(id.x) + 0.5, f32(screen_size.y - id.y) - 0.5);
    let r = vec2f(f32(screen_size.x), f32(screen_size.y));

    // 坐标矩阵变换
    let uv = (fragCoord + fragCoord - r) / r.y;
    let rotMatrix = mat2x2f(3.0, 4.0, 4.0, -3.0);
    var p = (uv * rotMatrix) / 100.0;

    let t = time.elapsed;

    var S = vec4f(0.0);
    var C = vec4f(1.0, 2.0, 3.0, 0.0);
    var W = vec4f(0.0);
    let T = 0.1 * t + p.y;

    for (var i: f32 = 1.0; i <= 30.0; i += 1.0) {

        // ✨基于粒子 ID 生成专属它的伪随机数 (0.0 到 1.0 之间)
        let randomSize = fract(sin(i * 137.3) * 43758.5453);   // 用于控制粗细
        let randomLength = fract(sin(i * 246.8) * 43758.5453); // 用于控制长度

        // 更新位置
        let phaseX = i * (C.x + 8.0 + i) + T + T;
        let phaseY = i * (C.z + 8.0 + i) + T + T;
        p += 0.02 * vec2f(cos(phaseX), cos(phaseY));

        // 颜色与亮度
        W = sin(i) * C;
        let colorFactor = cos(W) + vec4f(1.0);
        let brightness = exp(sin(i + i * T));

        // 使用 fBm 生成云雾密度
        let sampleUV = p / exp(W.x) + vec2f(i, t) / 8.0;
        let cloudDensity = fbm(sampleUV * 100.0);

        // ✨ 拖尾长度随机化
        // 长度系数原本是写死的 30.0，现在让它在 10.0 (短) 到 50.0 (长) 之间随机
        let tailLength = mix(10.0, 50.0, randomLength);
        let trailStretch = max(cloudDensity * tailLength, 0.01);

        let stretchedP = p / vec2f(2.0, trailStretch);
        var maxP = max(p, stretchedP);

        // 让尾部光晕稍微聚拢一点
        maxP.x *= 1.6;

        // ✨ 粒子粗细（光晕衰减）随机化
        // 注意：这里的数字是衰减除数。数字越小（比如 4000），粒子越粗越亮；数字越大（比如 25000），粒子越细越暗。
        let glowRadius = mix(4000.0, 25000.0, randomSize);

        let pLength = max(length(maxP), 0.000001);
        S += (colorFactor * brightness) / pLength / glowRadius;
    }

    // 纯黑背景映射
    var col = tanh(S * S).rgb;

    textureStore(screen, id.xy, vec4f(col, 1.0));
}
