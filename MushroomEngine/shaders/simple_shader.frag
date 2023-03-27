#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPos;
layout(location = 2) in vec3 fragWorldNormal;
//layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

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
   mat4 normalMatrix;
} push;

void main() {
   vec3 directionToLight = ubo.pointLightPosition - fragWorldPos;
   float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
   vec3 pLightColor = ubo.pointLightColor.xyz * ubo.pointLightColor.w * attenuation;
   vec3 dLightColor = ubo.directionalLightColor.xyz * ubo.directionalLightColor.w;

   vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
   vec3 fwnn = normalize(fragWorldNormal);

   vec3 pdLight = pLightColor * max(dot(fwnn, normalize(directionToLight)), 0);
   vec3 ddLight = dLightColor * max(dot(fwnn, normalize(-ubo.directionalLight)), 0);
   outColor = vec4((ddLight + pdLight + ambientLight) * fragColor, 1.0f);
}