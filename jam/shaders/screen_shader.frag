#version 140

in vec2 ex_TexCoords;
in vec4 ex_Color ;

out vec4 FragColor;

uniform sampler2D	material_diffuse;

void main()
{    
    FragColor = texture(material_diffuse, ex_TexCoords);
} 
