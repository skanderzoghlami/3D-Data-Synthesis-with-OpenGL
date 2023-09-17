#version 330 // Version de Shader

#ifdef VERTEX_SHADER

// Liste des parametres

layout(location= 0) in vec3 position;
layout(location= 2) in vec3 normal;

uniform mat4 mvpMatrix;

out vec3 direction_normal;

void main( )
{
        gl_Position= mvpMatrix * vec4(position, 1);
        direction_normal= normalize(normal);
}
#endif

#ifdef FRAGMENT_SHADER

in vec3 direction_normal;


uniform vec4 color;


void main( )
{

        vec3 light_direction = normalize(vec3(1.0, 1.0, 1.0));
        float cosine = dot(light_direction,direction_normal);
        if (cosine < 0.2){
            cosine = 0.2 ;
        }else if (cosine < 0.4){
            cosine = 0.4 ;
        }else if (cosine < 0.6){
            cosine = 0.6 ;
        }else if (cosine < 0.8){
            cosine = 0.8 ;
        }else if (cosine <  1){
            cosine = 1 ;
        }
        gl_FragColor = vec4(color*cosine  );
}
#endif
