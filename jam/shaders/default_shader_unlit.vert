#version 140

in vec3 in_Position;
in vec4 in_Color;
in vec2 in_TexCoords ;

uniform mat4 modelMatrix ;
uniform mat4 viewMatrix ;
uniform mat4 projMatrix ;

out vec4 ex_Color;
out vec2 ex_TexCoords ;

void main(void)
{
mat4 v = viewMatrix ;
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0) ;
	ex_Color = in_Color ;
	ex_TexCoords = in_TexCoords ;
}
