#version 430

in vec3 Position;
in vec3 Normal;
in vec3 FragOut;

in vec4 FragPosLightSpace;

struct LightInfo{
  vec4 Position;
  vec3 Intensity;
};
uniform LightInfo Light;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shiness;    // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

uniform vec3 camPosition;

uniform bool isShadow;
uniform sampler2D shadowMap;

uniform int shadowType;

out vec4 FragColors;


float ShadowCalculation(vec4 fragPosLightSpace){
	vec3 projCoords = fragPosLightSpace.xyz/fragPosLightSpace.w;

	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;

	float currentDepth = projCoords.z;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(Light.Position.xyz - camPosition);
	//float bias = 1/textureSize.x;
    //float bias = 0.005;
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);  
	float shadow = currentDepth>closestDepth+bias?1.0:0.0;

	return shadow;
}

void main()
{           
    //vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    //vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * Material.Kd;
    // diffuse
    vec3 lightDir = normalize(Light.Position.xyz - FragOut);
    float diff = max(dot(lightDir, Normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(camPosition -FragOut);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(Normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace);       
    if(isShadow){
       FragColors = vec4(ambient * (1.0 - shadow) + (diffuse + specular),1.0);    
    }
    else{
        FragColors = vec4(ambient+diffuse + specular,1.0); 
    }
}