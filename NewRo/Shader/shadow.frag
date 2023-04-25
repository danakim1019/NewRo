#version 430

in vec3 Position;
in vec3 Normal;
in vec3 FragOut;
in vec3 Color;

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

uniform bool hasColor;
uniform sampler2D shadowMap;

uniform int shadowType;

out vec4 FragColors;


float ShadowCalculation(vec4 fragPosLightSpace){
	vec3 projCoords = fragPosLightSpace.xyz/fragPosLightSpace.w;
	vec2 textureSize = textureSize(shadowMap,0);

	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;

	float currentDepth = projCoords.z;

	vec3 normal = normalize(Normal);
	vec3 lightdir = normalize(Light.Position.xyz - Position);
	float bias = 1/textureSize.x;
	float shadow = currentDepth>closestDepth+bias?1.0:0.0;
	if(projCoords.z>1.0||projCoords.x<0||projCoords.y<0||1<projCoords.x||1<projCoords.y)
			shadow=0.0;

	return shadow;
}

float ShadowCalculationVSMAnti(vec4 fragPosLightSpace){
   float shadow =0.0;
   vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
   projCoords = projCoords*0.5+0.5;
   vec2 uv = projCoords.xy;
   float currentDepth = projCoords.z;

   for(float x=-2.0;x<=2.0;x+=1.0){
		for(float y=-2.0;y<=2.0;y+=1.0){
			vec2 tmp = uv+vec2(x,y);
			vec2 moments = texture(shadowMap,tmp).xy;

			if(projCoords.z<=moments.x)
				return 1.0;
			float variance = max(moments.y-moments.x*moments.x,0.00002);
			float d = moments.x-currentDepth;
			shadow+=variance/(variance+d*d);
		}
   }

   if(currentDepth>1.0||uv.x<0||uv.y<0||1<uv.x||1<uv.y){
		shadow=1.0;
		return shadow;
   }

   return (shadow/25.0);
}

void main()
{
	vec3 ambient = Material.Ka * Light.Intensity;
	vec3 V = normalize(-Position.xyz);
	vec3 L = normalize(Light.Position.xyz-Position);
	vec3 R = normalize(reflect(-L,Normal));
	vec3 H = normalize(V+L);
	vec3 diffuse = Light.Intensity * Material.Kd * max(dot(L,Normal),0);
	vec3 spec = Light.Intensity * Material.Ks* pow(max(dot(H,Normal),0.0),Material.Shiness);

	float shadow;		
//	shadow = ShadowCalculation(FragPosLightSpace);
//	if(hasColor)
//		FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0)*vec4(Color,1.0);
//	else
//		FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0);
	//VSMAnti only
	shadow = ShadowCalculationVSMAnti(FragPosLightSpace);
	vec3 finalShadow = vec3(shadow);
	if(hasColor)
		FragColors = vec4((ambient*shadow)+diffuse+spec,1.0)*vec4(Color,1.0);
	else
		FragColors = vec4((ambient*shadow)+diffuse+spec,1.0);

}