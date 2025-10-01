#version 430 core
out vec4 frag_color;
in vec2 tex_coords;

uniform sampler2D particle_tex;
uniform vec3 fog_color;
uniform float fog_density;
uniform float fog_global_opacity;

void main()
{
    vec2 center = vec2(0.5f, 0.5f);
    float dist = length(center - tex_coords);
    //float mask = clamp((0.5 - dist) / 0.5, 0, 1);
    float mask = smoothstep(0.5, 0.0, dist);

    float noise = texture(particle_tex, tex_coords).r;

    float fog_factor = 1.0 - exp(-fog_density * (gl_FragCoord.z / gl_FragCoord.w));
    fog_factor = clamp(fog_factor, 0.0, 1.0);

    float alpha = mask * noise * fog_factor * fog_global_opacity;
    frag_color = vec4(fog_color, alpha);
}
