wget -nc https://github.com/procedural/dawn_monorepo/releases/download/v7562-gl/google_tint_compiler_fedora44_dawn_monorepo-7562-gl.zip
7z x -aos google_tint_compiler_fedora44_dawn_monorepo-7562-gl.zip
./google_tint_compiler_fedora44_dawn_monorepo-7562-gl/tint shader.wgsl --format glsl -o shader.glsl
