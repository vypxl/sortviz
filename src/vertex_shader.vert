#version 330 core

#define TWO_PI 6.28318530718

uniform float size;
uniform float radius;

in float x;
in float i;

out float hue;

float rad = 200;

void main() {
    hue = x / size;
    float ang = (i / size * TWO_PI);
    float dist = radius - abs(x - i) / size;
    gl_Position = vec4(vec2(cos(ang), sin(ang)) * dist, 0., 1.);
}
