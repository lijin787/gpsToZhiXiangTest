// ZhiXiangJiaoTest.cpp : 定义控制台应用程序的入口点。
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

	//此函数因为处理的是相对值，所以忽略原点的变化
}

void DlToJt(float &x,float &y,float &z);

float zjPH,zjFY,zjGZ;		//载机偏航、俯仰、滚转

int _tmain(int argc, _TCHAR* argv[])
{
	float zjJD,zjWD,zjGD;		//载机经度、纬度、高度
	float mbJD,mbWD,mbGD;		//目标经度、纬度、高度

	float zjX,zjY,zjZ;			//载机地心系下x,y,z
	float mbX,mbY,mbZ;			//目标地心系下x,y,z
	float xdX,xdY,xdZ;			//相对x,y,z
	float resultPH,resultFY;	//输出的偏航、俯仰
	
	//从控制台输入载机的位置参数
	printf("(请输入载机的经度：\n)");
	scanf("%f",&zjJD);
	printf("(请输入载机的纬度：\n)");
	scanf("%f",&zjWD);
	printf("(请输入载机的高度：\n)");
	scanf("%f",&zjGD);

	printf("(请输入目标的经度：\n)");
	scanf("%f",&mbJD);
	printf("(请输入目标的纬度：\n)");
	scanf("%f",&mbWD);
	printf("(请输入目标的高度：\n)");
	scanf("%f",&mbGD);

	printf("(请输入载机的偏航：\n)");
	scanf("%f",&zjPH);
	printf("(请输入载机的俯仰：\n)");
	scanf("%f",&zjFY);
	printf("(请输入载机的滚转：\n)");
	scanf("%f",&zjGZ);
	zjJD = zjJD * PI / 180.0;
	zjWD = zjWD * PI / 180.0;
	mbJD = mbJD * PI / 180.0;
	mbWD = mbWD * PI / 180.0;
	zjPH = zjPH * PI / 180.0;
	zjFY = zjFY * PI / 180.0;
	zjGZ = zjGZ * PI / 180.0;
	//将载机位置转到地心系
	gpsTo84(zjJD,zjWD,zjGD,zjX,zjY,zjZ);
	//将目标位置转到地心系
	gpsTo84(mbJD,mbWD,mbGD,mbX,mbY,mbZ);
	//求目标相对载机的地心系参数
	xdX = mbX - zjX;
	xdY = mbY - zjY;
	xdZ = mbZ - zjZ;
	//将相对信息转到载机所在位置的地理系,需要载机的经纬度信息
	dxToDl(zjJD,zjWD,xdX,xdY,xdZ);
	//将相对信息转到偏航、俯仰、滚转确定的机体系
	DlToJt(xdX,xdY,xdZ);
	//计算角度信息
	resultFY = asin(xdY/sqrt(xdX*xdX+xdY*xdY+xdZ*xdZ));

	if(xdX > 0)				//前半区
	{
		//if(xdZ == 0)
		//	resultPH = 0;	//在x轴上下式也可以计算
		//else
			resultPH = asin(xdZ/sqrt(xdX*xdX+xdZ*xdZ));	//左为负，右为正
	}
	else if(xdX < 0)		//后半区
	{
		if(xdZ > 0)
			resultPH = PI - asin(xdZ/sqrt(xdX*xdX+xdZ*xdZ));
		else if(xdZ < 0)
			resultPH = -(PI + asin(xdZ/sqrt(xdX*xdX+xdZ*xdZ)));
		else
			resultPH = PI;
	}
	else					//Z轴上
	{
		if(xdZ > 0)
			resultPH = PI/2;
		else if(xdZ < 0)
			resultPH = -(PI/2);
		else
			resultPH = 0;
	}

	printf("目标的偏航角为:%.5f",resultPH*180/PI);
	printf("目标的俯仰角为:%.5f",resultFY*180/PI);


	scanf("%f",&resultPH);		//just for stop the program
	return 0;
}






void DlToJt(float &x,float &y,float &z)
{
	float tempX,tempY,tempZ;
	float tempX2,tempY2,tempZ2;
	//变换顺序改为偏航、俯仰、横滚
	//修正偏航
	tempX = x*cos(zjPH)-z*sin(zjPH);
	tempY = y;
	tempZ = x*sin(zjPH)+z*cos(zjPH);
	//修正俯仰
	tempX2 = tempX*cos(zjFY)+tempY*sin(zjFY);
	tempY2 = -tempX*sin(zjFY)+tempY*cos(zjFY);
	tempZ2 = tempZ;
		
	//修正滚转
	x = tempX2;
	y = tempY2*cos(zjGZ)+tempZ2*sin(zjGZ);
	z = -tempY2*sin(zjGZ)+tempZ2*cos(zjGZ);
}