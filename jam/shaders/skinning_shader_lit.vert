#version 140

in vec3  in_Position ;
in vec3  in_Normal ;
in vec2  in_TexCoords ;
in ivec4 in_BonesId ;
in vec4  in_Weights ;

const int MAX_BONES = 100;

uniform mat4  modelMatrix ;
uniform mat4  viewMatrix ;
uniform mat4  projMatrix ;
uniform mat3  normalMatrix ;
uniform mat4  bones[MAX_BONES] ;

out vec3 ex_FragPos ;
out vec3 ex_Normal;
//out vec4 ex_Color ;
out vec2 ex_TexCoords ;

void main(void)
{
    mat4 BoneTransform = bones[in_BonesId[0]] * in_Weights[0];
    BoneTransform     += bones[in_BonesId[1]] * in_Weights[1];
    BoneTransform     += bones[in_BonesId[2]] * in_Weights[2];
    BoneTransform     += bones[in_BonesId[3]] * in_Weights[3];

	vec4 PosL = BoneTransform * vec4(in_Position, 1.0);
	gl_Position = projMatrix * viewMatrix * modelMatrix * PosL ;

	ex_FragPos = vec3(modelMatrix*vec4(in_Position,1.0)) ;
	vec4 normalL = BoneTransform * vec4(in_Normal,0.0) ;
	ex_Normal = normalMatrix * (normalL.xyz) ;
//	ex_Color = vec4(1.0) ;
	ex_TexCoords = in_TexCoords ;
}
