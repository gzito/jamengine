#version 140

in vec3 in_Position ;
in vec3 in_Normal ;
in vec4 in_Color ;
in vec2 in_TexCoords ;

uniform mat4  modelMatrix ;
uniform mat4  viewMatrix ;
uniform mat4  projMatrix ;
uniform mat3  normalMatrix ;

out vec3 ex_FragPos ;
out vec3 ex_Normal;
out vec4 ex_Color ;
out vec2 ex_TexCoords ;

void main(void)
{
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0) ;
	ex_FragPos = vec3(modelMatrix*vec4(in_Position,1.0)) ;
	ex_Normal = normalMatrix * in_Normal ;
	ex_Color = vec4(1.0) ;
	ex_TexCoords = in_TexCoords ;
}
