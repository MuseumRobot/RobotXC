#pragma once
#include "stdafx.h"
class XCSimulateLaser{
public:
	XCSimulateLaser(void);
	~XCSimulateLaser(void);
	XCMap* m_map;
	XCConfig* m_config;
	QPointF* m_robotPos;
	float* m_robotFaceAngle;
	float laserResult[28];	//´æ´¢¼¤¹â·µ»ØµÄ¾àÀë£¨mm£©
	void CalculateSurroundStaticStatus();
	void CalculateSurroundStatus();
	bool isClear(float dist);
};

