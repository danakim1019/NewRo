#version 430

in vec3 Position;
in vec3 Normal;
in vec3 FragOut;
in vec2 Color;

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

uniform bool hasColor;
uniform bool isShadow;
uniform sampler2D shadowMap;

uniform int shadowType;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

out vec4 FragColors;


float ShadowCalculation(vec4 fragPosLightSpace,vec3 projCoords,vec3 lightDir,vec3 normal){
	vec2 textureSize = textureSize(shadowMap,0);

	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;

	float currentDepth = projCoords.z;

	float bias = 0.005;
	//float bias = max(0.05 * (1.0 - dot(Normal, lightdir)), 0.005); 
	float shadow = currentDepth>closestDepth+bias?1.0:0.0;
	if(projCoords.z>1.0||projCoords.x<0||projCoords.y<0||1<projCoords.x||1<projCoords.y)
			shadow=0.0;

	return shadow;
}

float InterpolatedCompare(sampler2D depths, vec2 uv, float currentDepth, float bias){

	float closestDepth = texture(shadowMap,uv).r;

	return currentDepth>closestDepth+bias?1.0:0.0;
}


float ShadowCalculationInterpolated(vec4 fragPosLightSpace,vec3 projCoords){
	vec2 size =textureSize(shadowMap,0);
	vec2 texelSize = vec2(1.0,1.0)/size;

	projCoords = projCoords*0.5+0.5;
	vec2 uv = projCoords.xy;
	vec2 f = fract(uv*size+0.5);
	float currentDepth = projCoords.z;

	vec2 textureSize = textureSize(shadowMap,0);
	float bias = 1.5/textureSize.x;

	float lb = InterpolatedCompare(shadowMap, uv+texelSize*vec2(0.0,0.0),currentDepth,bias);
	float lt = InterpolatedCompare(shadowMap, uv+texelSize*vec2(0.0,1.0),currentDepth,bias);
	float rb = InterpolatedCompare(shadowMap, uv+texelSize*vec2(1.0,0.0),currentDepth,bias);
	float rt = InterpolatedCompare(shadowMap, uv+texelSize*vec2(1.0,1.0),currentDepth,bias);
	float a = mix(lb,lt,f.y);
	float b = mix(rb,rt,f.y);
	float c = mix(a,b,f.x);
	return c;
}

float ShadowCalculationPCF(vec4 fragPosLightSpace,vec3 projCoords,vec3 lightDir){
	vec2 size = textureSize(shadowMap,0);
	vec2 texelSize =  vec2(1.0,1.0)/size;

	projCoords = projCoords*0.5+0.5;
	vec2 uv = projCoords.xy;
	float currentDepth = projCoords.z;
	float result = 0.0;

	vec2 textureSize = textureSize(shadowMap,0);
	float bias = 1.0/textureSize.x;

	bias = max(bias * (1.0 - dot(Normal, lightDir)), 0.005); 

	for(int x=-2;x<=2;x++)
	{
		for(int y=-2;y<=2;y++)
		{
			vec2 off = vec2(x,y)/size;
			result += InterpolatedCompare(shadowMap,uv+off,currentDepth,bias);
		}
	}

	return result/25.0;
}

float InterpolatedComparePCF(sampler2D depths, vec2 uv, float currentDepth,vec3 lightDir){
	vec2 size = textureSize(shadowMap,0);
	vec2 texelSize = vec2(1.0,1.0)/size;

	vec2 f = fract(uv*size+0.5);

	vec2 textureSize = textureSize(shadowMap,0);
	float bias = 2.0/textureSize.x;

	bias = max(bias * (1.0 - dot(Normal, lightDir)), 0.005); 

	float lb = InterpolatedCompare(depths, uv+texelSize*vec2(0.0,0.0),currentDepth,bias);
	float lt = InterpolatedCompare(depths, uv+texelSize*vec2(0.0,1.0),currentDepth,bias);
	float rb = InterpolatedCompare(depths, uv+texelSize*vec2(1.0,0.0),currentDepth,bias);
	float rt = InterpolatedCompare(depths, uv+texelSize*vec2(1.0,1.0),currentDepth,bias);
	float a = mix(lb,lt,f.y);
	float b = mix(rb,rt,f.y);
	float c = mix(a,b,f.x);
	return c;
}

float ShadowCalculationInterPCF(vec4 fragPosLightSpace,vec3 projCoords,vec3 lightDir){
	vec2 size = textureSize(shadowMap,0);
	vec2 texelSize = vec2(1.0,1.0)/size;

	projCoords = projCoords*0.5+0.5;
	vec2 uv = projCoords.xy;
	float currentDepth = projCoords.z;
	float result = 0.0;

	for(int x=-1;x<=1;x++)
	{
		for(int y=-1;y<=1;y++)
		{
			vec2 off = vec2(x,y)/size;
			result += InterpolatedComparePCF(shadowMap,uv+off,currentDepth,lightDir);
		}
	}

	return result/9.0;
}

float ShadowCalculationVSM(vec4 fragPosLightSpace,vec3 projCoords){

	float p_max =0.0;
	projCoords = projCoords*0.5+0.5;
	float currentDepth = projCoords.z;

	vec2 moments = texture(shadowMap,projCoords.xy).rg;

	if(projCoords.z<=moments.x)
			return 1.0;
	float variance = max(moments.y-(moments.x*moments.x),0.002);

	float d =  projCoords.z - moments.x;
	p_max=variance/(variance+d*d);

   return p_max;
}

void main()
{
	vec3 normal = normalize(Normal);
	vec3 ambient = Material.Ka * Light.Intensity;
	vec3 V = normalize(camPosition-Position);
	vec3 lightDir = normalize(Light.Position.xyz - Position);
	vec3 L = normalize(-Light.Position.xyz-FragOut);
	vec3 R = normalize(reflect(-lightDir,normal));
	vec3 H = normalize(V+lightDir);
	vec3 diffuse = Light.Intensity * Material.Kd * max(dot(lightDir,normal),0);
	vec3 spec=vec3(0,0,0);
	if(dot(lightDir,normal)>0){
		spec= Light.Intensity * Material.Ks* pow(max(dot(H,normal),0.0),Material.Shiness);
	}
	 //change vertex position : 모든 light 계산은 camera좌표계에서 이루어짐
	vec4 texColor=vec4(1);
	if(hasColor){
		texColor= texture(texture_diffuse1,Color);
		//texColor+=texture(texture_normal1,Color);
		//texColor+=texture(texture_specular1,Color).rgba;
	}

	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;

	if(isShadow){
		float shadow;
		if(shadowType>=0&&shadowType<=3)
		{
			if(shadowType==0){			//shadow
				shadow = ShadowCalculation(FragPosLightSpace,projCoords,lightDir,Normal);
			}
			else if(shadowType==1){		//Interpolated
				shadow=ShadowCalculationInterpolated(FragPosLightSpace,projCoords);
			}
			else if(shadowType==2){		//PCF
				shadow=ShadowCalculationPCF(FragPosLightSpace,projCoords,lightDir);
			}
			else if(shadowType==3){		//InterPCF
				shadow = ShadowCalculationInterPCF(FragPosLightSpace,projCoords,lightDir);
			}
			FragColors = vec4((ambient*(1-shadow))+diffuse+spec,1.0)*texColor;
		}
		else{
			float pMax=1.0;
			if(shadowType==4){		//VSM
				pMax = ShadowCalculationVSM(FragPosLightSpace,projCoords);
				shadow = ShadowCalculationPCF(FragPosLightSpace,projCoords,lightDir);
			}
			else if(shadowType==5){		//VSMAnti only
				pMax = ShadowCalculationVSM(FragPosLightSpace,projCoords);
				shadow = ShadowCalculationInterPCF(FragPosLightSpace,projCoords,lightDir);
			}
			FragColors = vec4(ambient*(1-shadow)+diffuse+spec,1.0)*pMax*texColor;
		}
	}
	else{
			FragColors = vec4(ambient+diffuse+spec,1.0)*texColor;
	}
}