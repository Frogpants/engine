#version 330 core

in vec3 vNormal;
in vec3 vWorldPos;

out vec4 fragColor;

uniform vec3 uCameraPos;
uniform vec3 uLightPos;
uniform vec3 uAlbedo;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(uLightPos - vWorldPos);
    float lambert = max(dot(N, L), 0.0);

    vec3 viewDir = normalize(uCameraPos - vWorldPos);
    vec3 H = normalize(L + viewDir);
    float spec = pow(max(dot(N, H), 0.0), 32.0);

    vec3 color = uAlbedo * lambert + vec3(1.0) * spec * 0.2;
    color = clamp(color, 0.0, 1.0);
    fragColor = vec4(color, 1.0);
}
