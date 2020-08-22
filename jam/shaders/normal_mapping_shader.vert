#version 140

#define NR_OF_LIGHTS 2

struct Light {
	int		type ;			// 1=directional, 2=point, 3=spot
	vec3	position ;		// point light
	vec3	direction ;		// directional and spot light

	// attenuation factors for point light
    float	constant ;
    float	linear ;
    float	quadratic ;  

	vec3	ambient ;
	vec3	diffuse ;
	vec3	specular ; 
} ;

in vec3 in_Position ;
in vec3 in_Normal ;
in vec2 in_TexCoords ;
in vec3 in_Tangent ;
//in vec3 in_Bitangent ;

uniform mat4  modelMatrix ;
uniform mat4  viewMatrix ;
uniform mat4  projMatrix ;
uniform mat3  normalMatrix ;
uniform Light lights[NR_OF_LIGHTS] ;
uniform vec3  viewPos ;

out vec3 ex_FragPos ;
out vec2 ex_TexCoords ;
out vec3 ex_TangentLightPos[NR_OF_LIGHTS] ;
out vec3 ex_TangentLightDir[NR_OF_LIGHTS] ;
out vec3 ex_TangentViewPos ;
out vec3 ex_TangentFragPos ;

void main(void)
{
    ex_FragPos = vec3(modelMatrix * vec4(in_Position, 1.0));   
    ex_TexCoords = in_TexCoords;
    
    vec3 T = normalize(normalMatrix * in_Tangent);
    vec3 N = normalize(normalMatrix * in_Normal);
	// re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));
	for( int i = 0 ; i<NR_OF_LIGHTS; i++ ) {    
		ex_TangentLightPos[i] = TBN * lights[i].position ;
		ex_TangentLightDir[i] = TBN * lights[i].direction ;
	}
    ex_TangentViewPos = TBN * viewPos;
    ex_TangentFragPos = TBN * ex_FragPos;
        
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
}
