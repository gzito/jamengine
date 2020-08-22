#version 140

in vec3 in_Position;

uniform mat4 viewMatrix ;
uniform mat4 projMatrix ;

out vec3 ex_TexCoords ;

void main(void)
{
	vec4 WVP_Pos = projMatrix * viewMatrix * vec4(in_Position, 1.0) ;
	gl_Position = WVP_Pos.xyww ;
	ex_TexCoords = in_Position ;
}
