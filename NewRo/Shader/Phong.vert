#version 450

in vec3 coord3d;
in vec3 v_normal;

layout(std430, binding = 0) buffer ssbo1  //for specifying joints : full
 {
	mat4 aInstanceMatrix[];
 };

out vec3 f_color;

struct LightInfo {
  vec4 Position; // Light position in eye coords.
  vec3 La;       // Ambient light intensity
  vec3 Ld;       // Diffuse light intensity
  vec3 Ls;       // Specular light intensity
};

uniform LightInfo Light;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shiness;    // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform uint numChar;

uniform mat4 location;

void main()
{
	//change vertex position : 모든 light 계산은 camera좌표계에서 이루어짐
	vec4 P= ModelViewMatrix * vec4(coord3d,1.0); //camera좌표계에서의 vertex위치값
	//normal 변경
	vec3 N = normalize(NormalMatrix * v_normal); //local 좌표계에서의 normal을 변경
	
	vec3 L = normalize(vec3(Light.Position - P));	//내적계산을 위해 vec3로 변경

	vec3 V = normalize(vec3(-P));			//CameraPositon(0,0,0) - P = -P
	vec3 R = normalize(reflect(-L,N));		//반사

	vec3 ambient = Light.La * Material.Ka;
	vec3 diffuse = Light.Ld * Material.Kd * max(dot(L,N),0);
	vec3 spec = Light.Ls * Material.Ks * pow(max(dot(R,V), 0.0), Material.Shiness);

	f_color = ambient + diffuse + spec;

	//gl_Position = projection*view*aInstanceMatrix[gl_InstanceID+numChar] *model* vec4(coord3d, 1.0f);
	gl_Position = projection* view *location* model * vec4(coord3d,1.0);
	
}