#version 330 core

in Varyings {
    vec4 color;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;

void main(){
    //TODO: Done (Req 7) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color

    vec4 tinted_color = tint * fs_in.color;
    frag_color = tinted_color;
}