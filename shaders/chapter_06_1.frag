#version 330 core

uniform float float_color_from_cpu;
out vec4 frag_color;

void main() {
    frag_color = vec4(float_color_from_cpu, float_color_from_cpu, float_color_from_cpu, float_color_from_cpu);
}
