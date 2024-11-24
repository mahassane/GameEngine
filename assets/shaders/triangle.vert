#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)


out Varyings {
    vec3 color;
} vs_out;


uniform vec2 translation = vec2(0.0, 0.0);
uniform vec2 scale = vec2(1.0, 1.0);


// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

//TODO: Done (Req 1) Finish this shader

void main()
{
    vec3 vertices[3] = vec3[3](
        vec3(-0.5, -0.5, 0.0),
        vec3( 0.5, -0.5, 0.0),
        vec3( 0.0,  0.5, 0.0)
    );

    vec3 vertices_color[3] = vec3[3](
        vec3(1.0, 0.0, 0.0), 
        vec3(0.0, 1.0, 0.0), 
        vec3(0.0, 0.0, 1.0)
    );
    gl_Position = vec4(vec3(scale,1.0) * vertices[gl_VertexID] + vec3 (translation, 0.0), 1.0);
    //gl_Position is used to render vertix positions. Since it takes vec4, we need to add the homogeneous comp.. We set it to one as this is a point in space not a vector.
    // Since scale and translation are vec2, we need to add a third coordinate(z) to be able to multiply/add them to verticesm ehich is vec3
    //We added 1.0 as z coord to scale and 0.0 as z coord to translation since we don't want to change z.
    vs_out.color = vertices_color[gl_VertexID];
}