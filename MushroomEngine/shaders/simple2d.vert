#version 450

layout(location = 0) in vec2 position;

layout(location = 0) out vec3 oColor;


layout(push_constant) uniform Push {
   vec3 polyColor;
} push;


// Constant
const vec2 vp2d = vec2(1.0f / 19.20f, 1.0f / 10.80f);

void main() {
    oColor = push.polyColor;
    gl_Position = vec4(position * vp2d, 0.0, 1.0);
}