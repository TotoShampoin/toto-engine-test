#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texcoord;

out vec4 f_color;

void main() {
    // f_color = vec4(v_normal, 1.0);
    f_color = vec4(v_texcoord, 0.0, 1.0);
}
