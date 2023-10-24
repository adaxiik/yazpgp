#version 330 core
out vec4 frag_color;

in vec3 vs_texcoord;

uniform samplerCube skybox;

void main()
{    
    frag_color = texture(skybox, vs_texcoord);
    // frag_color = vec4( 1.0);
}