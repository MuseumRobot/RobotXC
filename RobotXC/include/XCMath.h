#pragma once
#include "stdafx.h"
#define PI 3.141592653

float GetAngleFromVector(QPointF delta);		//由向量得角
float GetDistanceFromVector(QPointF delta);		//由向量得距离
float Modf360(float angle);						//将角度换算到(0,360)之间
float cosAngle(float angle);					//求角度余弦值
float sinAngle(float angle);					//求角度正弦值
std::vector<std::vector<int>> DilateMatrix(int n,std::vector<std::vector<int>> originMatrix);	//求膨胀矩阵