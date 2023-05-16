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

uniform bool hasColor;
uniform bool isShadow;
uniform sampler2D shadowMap;

uniform int shadowType;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

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

float InterpolatedCompare(sampler2D depths, vec2 uv, float currentDepth, float bias){
	vec2 textureSize = textureSize(shadowMap,0);
	float closestDepth = texture(shadowMap,uv).r;

	vec3 normal = normalize(Normal);
	vec3 lightdir = normalize(Light.Position.xyz-Position);

	float shadow = currentDepth>closestDepth+bias?1.0:0.0;
	if(currentDepth>1.0||uv.x<0||uv.y<0||1<uv.x||1<uv.y)
		shadow=0.0;

	return shadow;
}


float ShadowCalculationInterpolated(vec4 fragPosLightSpace){
	vec2 size =textureSize(shadowMap,0);;
	vec2 texelSize = vec2(1.0,1.0)/size;

	vec3 projCoords = fragPosLightSpace.xyz/fragPosLightSpace.w;
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

float ShadowCalculationPCF(vec4 fragPosLightSpace){
	vec2 size = textureSize(shadowMap,0);
	vec2 texelSize =  vec2(1.0,1.0)/size;

	vec3 projCoords = fragPosLightSpace.xyz/fragPosLightSpace.w;
	projCoords = projCoords*0.5+0.5;
	vec2 uv = projCoords.xy;
	float currentDepth = projCoords.z;
	float result = 0.0;

	vec2 textureSize = textureSize(shadowMap,0);
	float bias = 2.0/textureSize.x;

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

float InterpolatedComparePCF(sampler2D depths, vec2 uv, float currentDepth){
	vec2 size = textureSize(shadowMap,0);
	vec2 texelSize = vec2(1.0,1.0)/size;

	vec2 f = fract(uv*size+0.5);

	vec2 textureSize = textureSize(shadowMap,0);
	float bias = 2.0/textureSize.x;

	float lb = InterpolatedCompare(depths, uv+texelSize*vec2(0.0,0.0),currentDepth,bias);
	float lt = InterpolatedCompare(depths, uv+texelSize*vec2(0.0,1.0),currentDepth,bias);
	float rb = InterpolatedCompare(depths, uv+texelSize*vec2(1.0,0.0),currentDepth,bias);
	float rt = InterpolatedCompare(depths, uv+texelSize*vec2(1.0,1.0),currentDepth,bias);
	float a = mix(lb,lt,f.y);
	float b = mix(rb,rt,f.y);
	float c = mix(a,b,f.x);
	return c;
}

float ShadowCalculationInterPCF(vec4 fragPosLightSpace){
	vec2 size = textureSize(shadowMap,0);
	vec2 texelSize = vec2(1.0,1.0)/size;

	vec3 projCoords = fragPosLightSpace.xyz/fragPosLightSpace.w;
	projCoords = projCoords*0.5+0.5;
	vec2 uv = projCoords.xy;
	float currentDepth = projCoords.z;
	float result = 0.0;

	for(int x=-1;x<=1;x++)
	{
		for(int y=-1;y<=1;y++)
		{
			vec2 off = vec2(x,y)/size;
			result += InterpolatedComparePCF(shadowMap,uv+off,currentDepth);
		}
	}

	return result/9.0;
}

float ShadowCalculationVSM(vec4 fragPosLightSpace){
	float shadow =0.0;
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords*0.5+0.5;
	vec2 uv = projCoords.xy;
	float currentDepth = projCoords.z;

	vec2 moments = texture(shadowMap,uv).xy;

	if(projCoords.z<=moments.x)
			return 1.0;
	float variance = max(moments.y-moments.x*moments.x,0.00002);
	float d = moments.x-currentDepth;
	shadow=variance/(variance+d*d);

	if(currentDepth>1.0||uv.x<0||uv.y<0||1<uv.x||1<uv.y){
		shadow=1.0;
	}

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
	vec3 spec=vec3(0,0,0);
	if(dot(L,Normal)>0){
		spec= Light.Intensity * Material.Ks* pow(max(dot(H,Normal),0.0),Material.Shiness);
	}
	 //change vertex position : 모든 light 계산은 camera좌표계에서 이루어짐
	vec4 texColor;
	if(hasColor){
		texColor= texture(texture_diffuse1,Color);
		//texColor+=texture(texture_normal1,Color);
		//texColor+=texture(texture_specular1,Color).rgba;
	}

	if(isShadow){
		float shadow;
		if(shadowType==0){			//shadow
			shadow = ShadowCalculation(FragPosLightSpace);
			if(hasColor)
				FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0)*texColor;
			else
				FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0);
		}
		else if(shadowType==1){		//Interpolated
			shadow=ShadowCalculationInterpolated(FragPosLightSpace);
			if(hasColor)
				FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0)*texColor;
			else
				FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0);
		}
		else if(shadowType==2){		//PCF
			shadow=ShadowCalculationPCF(FragPosLightSpace);
			if(hasColor)
				FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0)*texColor;
			else
				FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0);
		}
		else if(shadowType==3){		//InterPCF
			shadow = ShadowCalculationInterPCF(FragPosLightSpace);
			if(hasColor)
				FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0)*texColor;
			else
				FragColors = vec4((ambient*(1.0-shadow))+diffuse+spec,1.0);
		}
		else if(shadowType==4){		//VSM
			shadow = ShadowCalculationVSM(FragPosLightSpace);
			if(hasColor)
				FragColors = vec4((ambient*shadow)+diffuse+spec,1.0)*texColor;
			else
				FragColors = vec4((ambient*shadow)+diffuse+spec,1.0);
		}
		else if(shadowType==5){
			//VSMAnti only
			shadow = ShadowCalculationVSMAnti(FragPosLightSpace);
			vec3 finalShadow = vec3(shadow);
			if(hasColor)
				FragColors = vec4((ambient*shadow)+diffuse+spec,1.0)*texColor;
			else
				FragColors = vec4((ambient*shadow)+diffuse+spec,1.0);
		}
	}
	else{
		if(hasColor)
			FragColors = vec4(ambient+diffuse+spec,1.0)*texColor;
		else
			FragColors = vec4(ambient+diffuse+spec,1.0);
	}
}