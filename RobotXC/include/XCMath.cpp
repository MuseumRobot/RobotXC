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
std::vector<std::vector<int>> DilateMatrix(int n,std::vector<std::vector<int>> originMatrix){
	std::vector<std::vector<int>> dilate_maze;
	int M = originMatrix.size();
	int N = originMatrix[0].size();
	dilate_maze.resize(M);
	for(int i=0;i<M;i++){
		dilate_maze[i].resize(N);
	}
	if(n == 0){
		return originMatrix;
	}else if(n > 0){
		std::vector<std::vector<int>> pre_dilate_maze =  DilateMatrix(n-1,originMatrix);
		for(int i=0;i<M;i++){
			for(int j=0;j<N;j++){
				if(pre_dilate_maze[i][j]>0){
					for(int x=i-1;x<i+2;x++){
						for(int y=j-1;y<j+2;y++){
							if(x>-1 && y>-1 && x<M && y<N){
								if(dilate_maze[x][y] == 0){
									dilate_maze[x][y] = pre_dilate_maze[i][j];
								}
							}
						}
					}
				}
			}
		}
	}
	return dilate_maze;
}