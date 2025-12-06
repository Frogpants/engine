#include <iostream>
#include <stdexcept>
#include <cmath>
#include <optional>

#include "essentials.hpp"

vec2 v1(0.0);
vec2 v2(1.0, 1.0);

float dist2 = length(v2 - v1);

vec3 v3(0.0);
vec3 v4(1.0, 1.0, 1.0);

float dist3 = length(v4 - v3);

vec4 v5(0.0);
vec4 v6(1.0, 1.0, 1.0, 1.0);

float dist4 = length(v6 - v5);

int main() {
    std::cout << "dist2=" << dist2 << " dist3=" << dist3 << " dist4=" << dist4 << "\n";
    return 0;
}