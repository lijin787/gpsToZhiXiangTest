// ZhiXiangJiaoTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "math.h"


#define PI 3.14159265

void gpsTo84(float jd, float wd, float hh, float &x, float &y, float &z)
{
	float a = 6378137.0;
	float b = 6356752.3142;
	float temp = b/a;
	float e = 1.0 - temp*temp;
	float w = sqrt(1.0 - e*sin(wd)*sin(wd));
	float n = a/w;

	x = (n+hh)*cos(wd)*cos(jd);
	y = (n+hh)*cos(wd)*sin(jd);
	z = (n*(1-e)+hh)*sin(wd);
}

void dxToDl(float jd, float wd, float &x, float &y, float &z)
{
	float px,py,pz,temp;
	px = x*cos(jd)+y*sin(jd);
	py = -x*sin(jd)+y*cos(jd);
	pz = z;

	x = px*cos(wd)+pz*sin(wd);
	y = py;
	z = -px*sin(wd)+pz*cos(wd);

	temp = x;
	x = z;
	z = y;
	y = temp;

	//�˺�����Ϊ����������ֵ�����Ժ���ԭ��ı仯
}

void DlToJt(float &x,float &y,float &z);

float zjPH,zjFY,zjGZ;		//�ػ�ƫ������������ת

int _tmain(int argc, _TCHAR* argv[])
{
	float zjJD,zjWD,zjGD;		//�ػ����ȡ�γ�ȡ��߶�
	float mbJD,mbWD,mbGD;		//Ŀ�꾭�ȡ�γ�ȡ��߶�

	float zjX,zjY,zjZ;			//�ػ�����ϵ��x,y,z
	float mbX,mbY,mbZ;			//Ŀ�����ϵ��x,y,z
	float xdX,xdY,xdZ;			//���x,y,z
	float resultPH,resultFY;	//�����ƫ��������
	
	//�ӿ���̨�����ػ���λ�ò���
	printf("(�������ػ��ľ��ȣ�\n)");
	scanf("%f",&zjJD);
	printf("(�������ػ���γ�ȣ�\n)");
	scanf("%f",&zjWD);
	printf("(�������ػ��ĸ߶ȣ�\n)");
	scanf("%f",&zjGD);

	printf("(������Ŀ��ľ��ȣ�\n)");
	scanf("%f",&mbJD);
	printf("(������Ŀ���γ�ȣ�\n)");
	scanf("%f",&mbWD);
	printf("(������Ŀ��ĸ߶ȣ�\n)");
	scanf("%f",&mbGD);

	printf("(�������ػ���ƫ����\n)");
	scanf("%f",&zjPH);
	printf("(�������ػ��ĸ�����\n)");
	scanf("%f",&zjFY);
	printf("(�������ػ��Ĺ�ת��\n)");
	scanf("%f",&zjGZ);
	zjJD = zjJD * PI / 180.0;
	zjWD = zjWD * PI / 180.0;
	mbJD = mbJD * PI / 180.0;
	mbWD = mbWD * PI / 180.0;
	zjPH = zjPH * PI / 180.0;
	zjFY = zjFY * PI / 180.0;
	zjGZ = zjGZ * PI / 180.0;
	//���ػ�λ��ת������ϵ
	gpsTo84(zjJD,zjWD,zjGD,zjX,zjY,zjZ);
	//��Ŀ��λ��ת������ϵ
	gpsTo84(mbJD,mbWD,mbGD,mbX,mbY,mbZ);
	//��Ŀ������ػ��ĵ���ϵ����
	xdX = mbX - zjX;
	xdY = mbY - zjY;
	xdZ = mbZ - zjZ;
	//�������Ϣת���ػ�����λ�õĵ���ϵ,��Ҫ�ػ��ľ�γ����Ϣ
	dxToDl(zjJD,zjWD,xdX,xdY,xdZ);
	//�������Ϣת��ƫ������������תȷ���Ļ���ϵ
	DlToJt(xdX,xdY,xdZ);
	//����Ƕ���Ϣ
	resultFY = asin(xdY/sqrt(xdX*xdX+xdY*xdY+xdZ*xdZ));

	if(xdX > 0)				//ǰ����
	{
		//if(xdZ == 0)
		//	resultPH = 0;	//��x������ʽҲ���Լ���
		//else
			resultPH = asin(xdZ/sqrt(xdX*xdX+xdZ*xdZ));	//��Ϊ������Ϊ��
	}
	else if(xdX < 0)		//�����
	{
		if(xdZ > 0)
			resultPH = PI - asin(xdZ/sqrt(xdX*xdX+xdZ*xdZ));
		else if(xdZ < 0)
			resultPH = -(PI + asin(xdZ/sqrt(xdX*xdX+xdZ*xdZ)));
		else
			resultPH = PI;
	}
	else					//Z����
	{
		if(xdZ > 0)
			resultPH = PI/2;
		else if(xdZ < 0)
			resultPH = -(PI/2);
		else
			resultPH = 0;
	}

	printf("Ŀ���ƫ����Ϊ:%.5f",resultPH*180/PI);
	printf("Ŀ��ĸ�����Ϊ:%.5f",resultFY*180/PI);


	scanf("%f",&resultPH);		//just for stop the program
	return 0;
}






void DlToJt(float &x,float &y,float &z)
{
	float tempX,tempY,tempZ;
	float tempX2,tempY2,tempZ2;
	//�任˳���Ϊƫ�������������
	//����ƫ��
	tempX = x*cos(zjPH)-z*sin(zjPH);
	tempY = y;
	tempZ = x*sin(zjPH)+z*cos(zjPH);
	//��������
	tempX2 = tempX*cos(zjFY)+tempY*sin(zjFY);
	tempY2 = -tempX*sin(zjFY)+tempY*cos(zjFY);
	tempZ2 = tempZ;
		
	//������ת
	x = tempX2;
	y = tempY2*cos(zjGZ)+tempZ2*sin(zjGZ);
	z = -tempY2*sin(zjGZ)+tempZ2*cos(zjGZ);
}