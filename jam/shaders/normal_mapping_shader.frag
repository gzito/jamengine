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

in vec3 ex_FragPos ;
in vec2 ex_TexCoords ;
in vec3 ex_TangentLightPos[NR_OF_LIGHTS] ;
in vec3 ex_TangentLightDir[NR_OF_LIGHTS] ;
in vec3 ex_TangentViewPos ;
in vec3 ex_TangentFragPos ;	

// for version 140 we can't encapsulate sampler2D into a Material struct, so we define material here
uniform float		material_shininess ;
uniform sampler2D	material_diffuse ;
uniform sampler2D	material_specular ;
uniform sampler2D	material_normal ;

uniform vec3		viewPos;
uniform Light		lights[NR_OF_LIGHTS] ;

out vec4 FragColor;

vec3 calcDirLight( Light light, vec3 tangentLightDir, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize(-tangentLightDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material_diffuse, ex_TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material_diffuse, ex_TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material_specular, ex_TexCoords));
    return (ambient + diffuse + specular);
}

vec3 calcPointLight( Light light, vec3 tangentLightPos, vec3 normal, vec3 viewDir, vec3 fragPos )
{
    vec3 lightDir = normalize(tangentLightPos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    // attenuation
    float dist    = length(tangentLightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
  			     light.quadratic * (dist * dist));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material_diffuse, ex_TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material_diffuse, ex_TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material_specular, ex_TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

void main(void)
{
     // obtain normal from normal map in range [0,1]
	vec3 normal = texture(material_normal,ex_TexCoords).rgb;

	// transform normal vector to range [-1,1]
	normal = normalize(normal * 2.0 - 1.0) ;

    vec3 viewDir = normalize(ex_TangentViewPos - ex_TangentFragPos) ;

	vec3 result	= vec3(0) ;

	for( int i=0; i<NR_OF_LIGHTS; i++ ) {
		if( lights[i].type == 1 ) {
			result += calcDirLight( lights[i], ex_TangentLightDir[i], normal, viewDir ) ;
		}
		else if( lights[i].type == 2 ) {
			result += calcPointLight( lights[i], ex_TangentLightPos[i], normal, viewDir, ex_TangentFragPos ) ;
		}
	}

	FragColor = vec4(result,1.0) ;
}

