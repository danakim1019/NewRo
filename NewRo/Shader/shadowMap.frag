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
	//x �Ǵ� y�� ���� �μ��� �κ� ���Լ��� ��ȯ�մϴ�.
	//���Լ��� ����� �ȼ� �� ������ ���̸� ���Ͽ� ���˴ϴ�.
	float dx = dFdx(depth);  
	float dy = dFdy(depth);
	moment2 += 0.25*(dx*dx+dy*dy);

   FragColors = vec4(moment1,moment2,0.0,0.0);
}