#version 330 // Version de Shader

#ifdef VERTEX_SHADER


layout(location= 0) in vec3 position;

uniform mat4 mvpMatrix;

// indice du sommet : gl_VertexID

void main( )
{
        gl_Position= mvpMatrix * vec4(position, 1);
     
}
#endif

#ifdef FRAGMENT_SHADER
// doit calculer la couleur du fragment

void main( )
{
        vec3 color;
        color = vec3(1.0,0.5,0);
        gl_FragColor = vec4(color,1);
}
#endif
