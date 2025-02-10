#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragWorldPos;
layout(location = 2) out vec3 fragWorldNormal;
layout(location = 3) out vec2 fragTexCoord;

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
   vec4 modelWorldPosition = push.modelMatrix * vec4(position, 1.0f);
   gl_Position = ubo.projection * ubo.view * modelWorldPosition;
   fragWorldPos = modelWorldPosition.xyz;
   fragWorldNormal = normalize(mat3(push.normalMatrix) * normal);
   fragColor = color;
   fragTexCoord = texcoord;

   /* Vertex Lighting! */
   // vec3 directionFromLight = modelWorldPosition.xyz - ubo.pointLightPosition;
   //float attenuation = 1.0 / dot(directionFromLight, directionFromLight); // distance squared
   //vec3 lightColor = ubo.pointLightColor.xyz * ubo.pointLightColor.w * attenuation;

   //vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
   //vec3 diffuseLight = lightColor * max(dot(normalWorldSpace, normalize(directionFromLight)), 0);
   //float lightIntensity = AMBIENT + max(dot(normalWorldSpace, ubo.directionToLight), 0);

   //fragColor = (diffuseLight + ambientLight) * color;
}