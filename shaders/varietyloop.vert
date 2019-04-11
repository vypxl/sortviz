#version 300 es

precision highp float;

#define TWO_PI 6.28318530718
#define radius 0.8

uniform float size;

in float x;
in float i;

flat out float hue;

void main() {
    gl_PointSize = 2.0;
    hue = x / size;
    float ang = (i / size * TWO_PI);
    float d = abs(x - i);
    float dist = radius - min(d, size - d) / ((size - 1.) * 0.5) * radius;
    gl_Position = vec4(vec2(cos(ang), sin(ang)) * dist, 0., 1.);
}
