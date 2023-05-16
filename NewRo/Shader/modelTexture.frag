#version 450 core 

out vec4 FragColor;

in vec2 TexCoord;
//in vec3 fNormal;
//in vec3 fPosition;
in vec3 f_color;

uniform sampler2D shadowMap;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

void main()
{
	vec4 objectDiffuse=vec4(0);
	vec4 objectSpecualr = vec4(0);
	vec4 objectNormal = vec4(0);


	objectDiffuse+= texture(texture_diffuse1,TexCoord);
	objectDiffuse+= texture(texture_diffuse2,TexCoord);
	objectDiffuse+= texture(texture_diffuse3,TexCoord);
	objectSpecualr +=texture(texture_specular1,TexCoord);
	objectNormal +=texture(texture_normal1,TexCoord);


	FragColor = vec4(mix(objectDiffuse,vec4(f_color,1.0),0.5));	 
}