#version 430

out vec4 FragColors;
in vec4 position;


void main()
{
	float depth = position.z/position.w;
	depth = depth*0.5+0.5;

	float moment1 = depth;
	float moment2 = depth*depth;

	//return the partial derivative of an argument with respect to x or y
	//x 또는 y에 대한 인수의 부분 도함수를 반환합니다.
	//도함수는 블록의 픽셀 값 사이의 차이를 취하여 계산됩니다.
	float dx = dFdx(depth);  
	float dy = dFdy(depth);
	moment2 += 0.25*(dx*dx+dy*dy);

   FragColors = vec4(moment1,moment2,0.0,0.0);
}