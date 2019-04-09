#version 300 es

precision highp float;

in vec4 pos_uv;

out vec2 uv;

uniform mat4 projection;

void main() {
    uv = pos_uv.zw;
    gl_Position = projection * vec4(pos_uv.xy, 0.0, 1.0);
}
