#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 normal_v;
    vec3 fragment_world_position;
    vec2 tex_coords;
} gs_in[];

out GS_OUT {
    vec3 normal_v;
    vec3 fragment_world_position;
    vec2 tex_coords;
} gs_out;

uniform float time;
uniform vec3 center;
uniform mat4 view;
uniform mat4 projection;

vec4 explode(int i);
vec3 get_normal();

void main() {
    gl_Position = explode(0);
    gs_out.normal_v = gs_in[0].normal_v;
    gs_out.fragment_world_position = gs_in[0].fragment_world_position;
    gs_out.tex_coords = gs_in[0].tex_coords;
    EmitVertex();

    gl_Position = explode(1);
    gs_out.normal_v = gs_in[1].normal_v;
    gs_out.fragment_world_position = gs_in[1].fragment_world_position;
    gs_out.tex_coords = gs_in[1].tex_coords;
    EmitVertex();

    gl_Position = explode(2);
    gs_out.normal_v = gs_in[2].normal_v;
    gs_out.fragment_world_position = gs_in[2].fragment_world_position;
    gs_out.tex_coords = gs_in[2].tex_coords;
    EmitVertex();

    EndPrimitive();
}

vec4 explode(int i) {
    vec3 direction = normalize(gs_in[1].fragment_world_position - center);

    float magnitude = 0.5f;
    float scalar = ((sin(time) + 1.0) / 2.0) * magnitude;

    vec3 displaced = gs_in[i].fragment_world_position + direction * scalar;
    return projection * view * vec4(displaced, 1.0f);
}

vec3 get_normal() {
    // TODO: or interpolate vertex normals?
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}
