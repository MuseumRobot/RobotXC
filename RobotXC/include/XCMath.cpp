#include "StdAfx.h"
#include "XCMath.h"
float GetAngleFromVector(QPointF delta){
	float angle = 0.0;
	if(delta.x() > 0 && delta.y() > 0){
		angle =atan(delta.y()/delta.x())/PI/2*360.0;	//Ŀ���ڵ�һ����
	}else if(delta.x() < 0 && delta.y() < 0){
		angle =180 + atan(delta.y()/delta.x())/PI/2*360.0;	//Ŀ���ڵ�������
	}else if(delta.x() <0 && delta.y() > 0){
		angle =180 + atan(delta.y()/delta.x())/PI/2*360.0;	//Ŀ���ڵڶ�����
	}else{
		angle =360 + atan(delta.y()/delta.x())/PI/2*360.0;	//Ŀ���ڵ�������
	}
	return angle;
}
float GetDistanceFromVector(QPointF delta){
	return sqrt(pow(delta.x(),2)+pow(delta.y(),2));
}
float Modf360(float angle){
	while(angle>360.0 || angle<0.0){
		if(angle>360.0) angle -= 360.0;
		else angle += 360.0;
	}
	return angle;
}
float cosAngle(float angle){
	return cos(angle/360.0*2*PI);
}
float sinAngle(float angle){
	return sin(angle/360.0*2*PI);
}