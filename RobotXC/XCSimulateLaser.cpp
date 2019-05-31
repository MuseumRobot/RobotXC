#include "StdAfx.h"
#include "XCSimulateLaser.h"
XCSimulateLaser::XCSimulateLaser(void){
	for(int i=0;i<28;i++) laserResult[i] = 5000;
}
XCSimulateLaser::~XCSimulateLaser(void){}
void XCSimulateLaser::CalculateSurroundStatus(){
	for(int i=0;i<28;i++) laserResult[i] = 5000;
	int w = m_config->far_obs_threshold()/10/m_config->architect_scale();	//需要探测的半径(格)
	int t_x = m_robotPos->x()/m_config->architect_scale();
	int t_y = m_robotPos->y()/m_config->architect_scale();
	for(int i=t_x-w;i<t_x+w;i++){
		if(i<0) continue;;
		for(int j=t_y-w;j<t_y+w;j++){
			if(j<0) continue;
			if(m_map->m_maze[j][i] == 1){
				QPointF obs((i+0.5)*m_config->architect_scale(),(j+0.5)*m_config->architect_scale());
				QPointF d(obs.x()-m_robotPos->x(),obs.y()-m_robotPos->y());
				float obsAngle = GetAngleFromVector(d);
				float dAngle = obsAngle - *m_robotFaceAngle;
				if(Modf360(abs(dAngle)) < 70){
					if(Modf360(dAngle<70)){
						laserResult[(int)dAngle/5] = 10*GetDistanceFromVector(d);
					}else{
						laserResult[(int)(dAngle-220)/5] = 10*GetDistanceFromVector(d);
					}
				}
			}
		}
	}
}
bool XCSimulateLaser::isClear(float dist){
	bool flag = true;
	for(int i=5;i<21;i++){
		if(laserResult[i]<dist){
			flag = false;
		}
	}
	return flag;
}
