#version 330 core

out vec4 frag_color;

float linearize_depth(float depth);

float far = 100.0f;
float near = 0.1f;

void main() {
    frag_color = vec4(vec3(linearize_depth(gl_FragCoord.z) / far), 1.0f);
}

float linearize_depth(float depth) {
    float ndc_z = 2 * depth - 1;
    float z_e = (2.0 * near * far) / (far + near - depth * (far - near));

    return z_e;
}
