#pragma once
#include "stdafx.h"
#define PI 3.141592653

float GetAngleFromVector(QPointF delta);		//�������ý�
float GetDistanceFromVector(QPointF delta);		//�������þ���
float Modf360(float angle);						//���ǶȻ��㵽(0,360)֮��
float cosAngle(float angle);					//��Ƕ�����ֵ
float sinAngle(float angle);					//��Ƕ�����ֵ
std::vector<std::vector<int>> DilateMatrix(int n,std::vector<std::vector<int>> originMatrix);	//�����;���