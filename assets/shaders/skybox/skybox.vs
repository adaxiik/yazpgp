#version 330 core
layout (location = 0) in vec3 vp;

out vec3 vs_texcoord;

uniform mat4 view_projection_matrix;

void main()
{
    vs_texcoord = vp;
    // gl_Position = view_projection_matrix * vec4(vp, 1.0);
    
    vec4 position = view_projection_matrix * vec4(vp, 1.0);
    gl_Position = position.xyww;
}  