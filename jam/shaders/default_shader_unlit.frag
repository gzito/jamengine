#version 140

in  vec4 ex_Color;
in  vec2 ex_TexCoords ;

uniform sampler2D material_diffuse ;

out vec4 out_Color;

void main(void)
{
	out_Color = texture(material_diffuse, ex_TexCoords) * ex_Color ;
}
