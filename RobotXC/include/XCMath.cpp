#include "StdAfx.h"
#include "XCMath.h"
float GetAngleFromVector(QPointF delta){
	float angle = 0.0;
	if(delta.x() > 0 && delta.y() > 0){
		angle =atan(delta.y()/delta.x())/PI/2*360.0;	//目标在第一象限
	}else if(delta.x() < 0 && delta.y() < 0){
		angle =180 + atan(delta.y()/delta.x())/PI/2*360.0;	//目标在第三象限
	}else if(delta.x() <0 && delta.y() > 0){
		angle =180 + atan(delta.y()/delta.x())/PI/2*360.0;	//目标在第二象限
	}else{
		angle =360 + atan(delta.y()/delta.x())/PI/2*360.0;	//目标在第四象限
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