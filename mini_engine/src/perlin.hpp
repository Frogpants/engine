#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <cmath>

#include "essentials.hpp"
 
float hash(const vec2& v) {
    vec2 i = floor(v * 10.0);
    return fract(sin(dot(i, vec2(12.989, 78.233))) * 43758.5453);
}

float hash(const vec3& v) {
    vec3 i = floor(v * 10.0);
    return fract(sin(dot(i, vec3(12.989, 78.233, 45.164))) * 43758.5453);
}

float hash(const vec4& v) {
    vec4 i = floor(v * 10.0);
    return fract(sin(dot(i, vec4(12.989, 78.233, 45.164, 24.377))) * 43758.5453);
}


float noise(const vec2& p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = hash(i + vec2(0,0));
    float b = hash(i + vec2(1,0));
    float c = hash(i + vec2(0,1));
    float d = hash(i + vec2(1,1));

    vec2 u = f * f * (f * -2.0 + 3.0);

    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

float noise(const vec3& p) {
    vec3 i = floor(p);
    vec3 f = fract(p);

    float a = hash(i + vec3(0,0,0));
    float b = hash(i + vec3(1,0,0));
    float c = hash(i + vec3(0,1,0));
    float d = hash(i + vec3(1,1,0));
    float e = hash(i + vec3(0,0,1));
    float f1 = hash(i + vec3(1,0,1));
    float g = hash(i + vec3(0,1,1));
    float h = hash(i + vec3(1,1,1));

    vec3 u = f*f*(f * -2.0 + 3.0);

    return mix(mix(mix(a, b, u.x), mix(c, d, u.x), u.y), mix(mix(e, f1, u.x), mix(g, h, u.x), u.y), u.z);
}

vec3 noiseNormal(const vec3& p) {
    float eps = 0.002;

    float dx = noise(p + vec3(eps,0,0)) - noise(p - vec3(eps,0,0));
    float dy = noise(p + vec3(0,eps,0)) - noise(p - vec3(0,eps,0));
    float dz = noise(p + vec3(0,0,eps)) - noise(p - vec3(0,0,eps));

    return normalize(vec3(dx, dy, dz));
}

vec3 noiseNormal(const vec2& p) {
    vec3 pos = vec3(p.x, noise(p), p.y);
    return noiseNormal(pos);
}

#endif // PERLIN_HPP