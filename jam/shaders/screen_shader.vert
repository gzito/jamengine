#version 140

in vec2 in_Position;
in vec2 in_TexCoords;
in vec4 in_Color ;

out vec2 ex_TexCoords;
out vec4 ex_Color ;

uniform mat4 modelMatrix;
uniform mat4 projMatrix;

void main()
{
    ex_TexCoords = in_TexCoords;
	ex_Color = in_Color ;
//    gl_Position = projMatrix * modelMatrix * vec4(in_Position.xy, 0.0, 1.0);
    gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
}
