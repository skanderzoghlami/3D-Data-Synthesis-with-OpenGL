
#version 420

#ifdef VERTEX_SHADER

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;

layout(location= 0) in vec3 position;
layout(location= 1) in vec2 texcoord;
layout(location= 2) in vec3 normal;

out vec2 vertex_texcoord;
out vec3 vertex_normal;
out vec3 vertex_position;

void main( )
{
	gl_Position= mvpMatrix * vec4(position, 1);
	
	vertex_texcoord= texcoord;
	vertex_normal= mat3(mvMatrix) * normal;
	vertex_position= vec3(mvMatrix * vec4(position, 1));
}

#endif

#ifdef FRAGMENT_SHADER

in vec2 vertex_texcoord;
in vec3 vertex_normal;
in vec3 vertex_position;

uniform sampler2D grid;
out vec4 fragment_color;

layout(early_fragment_tests) in ;
void main( )
{
	vec3 color= texture(grid, vertex_texcoord).rgb;
	vec3 l= vec3(0, 0, 1) - vertex_position;
	float ndotl= dot(normalize(vertex_normal), normalize(l));
	fragment_color= vec4(color * ndotl, 1);
	
	//~ fragment_color= vec4(1);
}

#endif
