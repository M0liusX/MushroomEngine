#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

//layout(location = 2) out vec2 fragTexCoord;

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    vec4 ambientLightColor;
    vec3 directionalLight;
    vec4 directionalLightColor;
    vec3 pointLightPosition;
    vec4 pointLightColor;
} ubo;

layout(push_constant) uniform Push {
   mat4 modelMatrix;
} push;

void main() {
   vec4 modelWorldPosition = push.modelMatrix * vec4(position, 1.0f);
   gl_Position = ubo.projection * ubo.view * modelWorldPosition;
   fragColor = color;
}