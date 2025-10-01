#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;

    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;

    float shininess;
};

uniform vec3 camera_pos;
uniform Material material;
uniform samplerCube sampler_skybox;

in GS_OUT {
    vec3 normal_v;
    vec3 fragment_world_position;
    vec2 tex_coords;
} fs_in;

out vec4 frag_color;

void main() {
    vec3 temp = fs_in.normal_v + fs_in.fragment_world_position;
    vec4 temp2 = texture(material.texture_diffuse1, fs_in.tex_coords) + texture(material.texture_specular1, fs_in.tex_coords);
    temp2.x += camera_pos.x;
    vec4 temp_frag_color = vec4(1.0, 1.0, 0.99 + temp.x * 0.0001 + temp2.x * 0.0001, 1.0f);

    vec3 look_dir = normalize(fs_in.fragment_world_position - camera_pos);
    vec3 reflection_dir = reflect(look_dir, normalize(fs_in.normal_v));

    frag_color = vec4(texture(sampler_skybox, reflection_dir).rgb, 1.0f);

    frag_color = mix(frag_color, temp_frag_color, 0.001f);
}
