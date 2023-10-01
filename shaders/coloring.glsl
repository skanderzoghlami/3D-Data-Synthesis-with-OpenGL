#version 330 // Version de Shader

#ifdef VERTEX_SHADER


layout(location= 0) in vec3 position;
layout(location= 2) in vec3 normal;
layout(location= 4) in uint material;

uniform mat4 mvpMatrix;
// sorties / varyings

flat out uint vertex_material;
out vec3 direction_normal;

void main( )
{
    gl_Position= mvpMatrix * vec4(position, 1);
    vertex_material = material ;
    direction_normal = normal;
}

#endif

#ifdef FRAGMENT_SHADER

flat in uint vertex_material; 
in vec3 direction_normal;

uniform float colors_r[100]; 
uniform float colors_g[100]; 
uniform float colors_b[100]; 

uniform uint n_colors; 

void main() {
    vec3 light_direction = normalize(vec3(1.0, 1.0, 1.0));
    float cosine = dot(light_direction, direction_normal);
    // Use vertex_material as an index to access the correct color
    vec3 color = vec3(colors_r[vertex_material], colors_g[vertex_material], colors_b[vertex_material]) ;
    gl_FragColor = vec4(cosine*color, 1.0);
}

#endif