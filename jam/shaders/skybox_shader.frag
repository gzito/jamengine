#version 140

in  vec3 ex_TexCoords ;

uniform samplerCube material_diffuse ;

out vec4 FragColor;

void main(void)
{
	FragColor = texture(material_diffuse, ex_TexCoords) ;
}
