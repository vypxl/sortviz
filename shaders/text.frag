#version 300 es

precision highp float;

out vec4 color;

in vec2 uv;

uniform sampler2D tex;

void main() {
    color = vec4(1.0, 1.0, 1.0, texture(tex, uv).a);
}
