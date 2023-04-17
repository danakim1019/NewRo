#version 450 core 

out vec4 FragColor;

in vec2 TexCoord;
in vec3 fNormal;
in vec3 fPosition;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shiness;    // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

struct LightInfo {
  vec3 Position; // Light position in eye coords.
  vec3 La;       // Ambient light intensity
  vec3 Ld;       // Diffuse light intensity
  vec3 Ls;       // Specular light intensity
};
uniform LightInfo Light;

uniform vec3 viewPosition;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_diffuse5;
uniform sampler2D texture_diffuse6;
uniform sampler2D texture_normal1;
uniform sampler2D texture_normal2;
uniform sampler2D texture_normal3;
uniform sampler2D texture_normal4;
uniform sampler2D texture_normal5;
uniform sampler2D texture_normal6;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;
uniform sampler2D texture_specular5;


void main()
{
	vec4 objectDiffuse=vec4(0);
	vec4 objectSpecualr = vec4(0);
	vec4 objectNormal = vec4(0);

	objectDiffuse+=texture(texture_diffuse1,TexCoord);
	objectDiffuse+=texture(texture_diffuse2,TexCoord);
	objectDiffuse+=texture(texture_diffuse3,TexCoord);
	objectDiffuse+=texture(texture_diffuse4,TexCoord);
	objectDiffuse+=texture(texture_diffuse5,TexCoord);
	objectDiffuse+=texture(texture_diffuse6,TexCoord);
	objectSpecualr+=texture(texture_specular1,TexCoord);
	objectSpecualr+=texture(texture_specular2,TexCoord);
	objectSpecualr+=texture(texture_specular3,TexCoord);
	objectSpecualr+=texture(texture_specular4,TexCoord);
	objectSpecualr+=texture(texture_specular5,TexCoord);
	objectNormal+=texture(texture_normal1,TexCoord);
	objectNormal+=texture(texture_normal2,TexCoord);
	objectNormal+=texture(texture_normal3,TexCoord);
	objectNormal+=texture(texture_normal4,TexCoord);
	objectNormal+=texture(texture_normal5,TexCoord);
	objectNormal+=texture(texture_normal6,TexCoord);
    
	vec3 N = normalize(fNormal);
   vec3 V = normalize(viewPosition.xyz-fPosition);			//CameraPositon(0,0,0) - P = -P

    vec3 L = normalize(Light.Position.xyz - fPosition);	//내적계산을 위해 vec3로 변경
   	vec3 R = normalize(reflect(-L,N));		//반사

	vec3 ambient = Light.La*Material.Ka;
	vec3 diffuse = Light.Ld*objectDiffuse.xyz*max(dot(L,N),0.0);
	vec3 specular = Light.Ls*objectSpecualr.xyz*pow(max(dot(R,V),0.0),Material.Shiness);

	FragColor = vec4(ambient+diffuse+specular,1.0);
	 //FragColor = mix(texture(texture_diffuse1,TexCoord),vec4(color,1.0),0.1);
}