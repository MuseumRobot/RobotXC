#include "StdAfx.h"
#include "XCSimulateLaser.h"
XCSimulateLaser::XCSimulateLaser(void){
	for(int i=0;i<28;i++) laserResult[i] = 5000;
}
XCSimulateLaser::~XCSimulateLaser(void){}
void XCSimulateLaser::CalculateSurroundStaticStatus(){
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
				float dAngle = Modf360(obsAngle - *m_robotFaceAngle);
				if(dAngle < 70 || dAngle > 290){
					if(dAngle < 70){
						if(10*GetDistanceFromVector(d)<laserResult[(int)dAngle/5])
							laserResult[(int)dAngle/5] = 10*GetDistanceFromVector(d);
					}else{
						if(10*GetDistanceFromVector(d)<laserResult[(int)(360-dAngle)/5+14])
							laserResult[(int)(360-dAngle)/5+14] = 10*GetDistanceFromVector(d);
					}
				}
			}
		}
	}
}
void XCSimulateLaser::CalculateSurroundStatus(){
	for(int i=0;i<28;i++) laserResult[i] = 5000;
	int w = m_config->far_obs_threshold()/10/m_config->architect_scale();	//需要探测的半径(格)
	int t_x = m_robotPos->x()/m_config->architect_scale();
	int t_y = m_robotPos->y()/m_config->architect_scale();
	for(int i=t_x-w;i<t_x+w;i++){
		if(i<0) continue;;
		for(int j=t_y-w;j<t_y+w;j++){
			if(j<0) continue;
			if(m_map->m_maze[j][i] == 1 || m_map->m_dynamicObstacleLife[j][i] > 0){
				QPointF obs((i+0.5)*m_config->architect_scale(),(j+0.5)*m_config->architect_scale());
				QPointF d(obs.x()-m_robotPos->x(),obs.y()-m_robotPos->y());
				float obsAngle = GetAngleFromVector(d);
				float dAngle = Modf360(obsAngle - *m_robotFaceAngle);
				if(dAngle < 70 || dAngle > 290){
					if(dAngle < 70){
						if(10*GetDistanceFromVector(d)<laserResult[(int)dAngle/5])
							laserResult[(int)dAngle/5] = 10*GetDistanceFromVector(d);
					}else{
						if(10*GetDistanceFromVector(d)<laserResult[(int)(360-dAngle)/5+14])
							laserResult[(int)(360-dAngle)/5+14] = 10*GetDistanceFromVector(d);
					}
				}
			}
		}
	}
}
bool XCSimulateLaser::isClear(float dist){
	CalculateSurroundStatus();
	bool flag = true;
	for(int i=7;i<19;i++){
		if(laserResult[i]<dist){
			flag = false;
		}
	}
	return flag;
}
