#include "StdAfx.h"
#include "XCOverview.h"

XCOverview::XCOverview(QWidget *parent):QWidget(parent){
	ui.setupUi(this);
	m_map = NULL;
	m_timerId = startTimer(100);
	x0=0,y0=0,w0=this->width(),h0=this->height();
	m_showDilateMap = 0;
}

XCOverview::~XCOverview(){}
void XCOverview::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	painter.setBrush(QColor(200,200,200));
	painter.drawRect(x0,y0,w0,h0);
	if(m_map != NULL){
		int w = w0/m_map->N;
		int h = h0/m_map->M;
		for(int i=0;i<m_map->M;i++){
			for(int j=0;j<m_map->N;j++){
				QRect r(x0+w*j,y0+h*i,w,h);
				if(m_showDilateMap == 0){
					if(m_map->m_maze[i][j] == 1){
						painter.setBrush(QColor(100,100,100));
						painter.drawRect(r);
					}else{
						painter.setBrush(QColor(220,220,220));
						painter.drawRect(r);
					}
				}else{
					if(m_map->m_dilate_maze[i][j] == 1){
						painter.setBrush(QColor(100,100,100));
						painter.drawRect(r);
					}else{
						painter.setBrush(QColor(220,220,220));
						painter.drawRect(r);
					}
				}
			}
		}
		if(m_map->x_start>-1&&m_map->y_start>-1){
			painter.drawImage(QRect(x0+w*m_map->y_start,y0+h*m_map->x_start,w,h),QImage("Resources/start.png"));
		}
		if(m_map->x_end>-1&&m_map->y_end>-1){
			painter.drawImage(QRect(x0+w*m_map->y_end,y0+h*m_map->x_end,w,h),QImage("Resources/end.png"));
		}
		for(std::list<XCPoint>::iterator iter = m_result->begin(); iter != m_result->end(); iter++) 
			painter.drawImage(QRect(x0+w*iter->y,y0+h*iter->x,w,h),QImage("Resources/foot.png"));

		//绘制机器人本体位置朝向
		QPointF robotPosByPixes(m_robotPos->x()/m_config->architect_scale()*m_config->map_scale(),m_robotPos->y()/m_config->architect_scale()*m_config->map_scale());
		QMatrix matrix;
		matrix.rotate(*m_robotFaceAngle);			//rotate默认顺时针旋转
		QImage img_robot = QImage("Resources/yellowCar.png");
		painter.drawImage(QPointF(robotPosByPixes.x()-m_config->map_scale(),robotPosByPixes.y()-m_config->map_scale()),img_robot.scaled(2*m_config->map_scale(),2*m_config->map_scale()).transformed(matrix,Qt::FastTransformation));	
		//绘制视野
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(255,0,0,150));
		painter.drawPie(QRect(robotPosByPixes.x()-m_config->map_scale()*m_config->obstacle_threshold()/10/m_config->architect_scale(),robotPosByPixes.y()-m_config->map_scale()*m_config->obstacle_threshold()/10/m_config->architect_scale(),m_config->map_scale()*m_config->obstacle_threshold()/10/m_config->architect_scale()*2,m_config->map_scale()*m_config->obstacle_threshold()/10/m_config->architect_scale()*2),((360-*m_robotFaceAngle)-70)*16,140*16);
		painter.setBrush(QColor(0,255,0,50));
		painter.drawPie(QRect(robotPosByPixes.x()-m_config->map_scale()*m_config->far_obs_threshold()/10/m_config->architect_scale(),robotPosByPixes.y()-m_config->map_scale()*m_config->far_obs_threshold()/10/m_config->architect_scale(),m_config->map_scale()*m_config->far_obs_threshold()/10/m_config->architect_scale()*2,m_config->map_scale()*m_config->far_obs_threshold()/10/m_config->architect_scale()*2),((360-*m_robotFaceAngle)-70)*16,140*16);
		for(int i=0;i<28;i++){
			if(m_simulateLaserResult[i]<1000){
				painter.setPen(QPen(Qt::black));
				QPointF obsOfLaserPosByPixes(m_simulateLaserResult[i]/10/m_config->architect_scale()*m_config->map_scale()*cosAngle((i+0.5)*5 + *m_robotFaceAngle)+robotPosByPixes.x(),m_simulateLaserResult[i]/10/m_config->architect_scale()*m_config->map_scale()*sinAngle((i+0.5)*5 + *m_robotFaceAngle)+robotPosByPixes.y());
				if(i>13){
					obsOfLaserPosByPixes = QPointF(m_simulateLaserResult[i]/10/m_config->architect_scale()*m_config->map_scale()*cosAngle(-1*(i+0.5)*5 + 70 + *m_robotFaceAngle)+robotPosByPixes.x(),m_simulateLaserResult[i]/10/m_config->architect_scale()*m_config->map_scale()*sinAngle(-1*(i+0.5)*5 + 70 + *m_robotFaceAngle)+robotPosByPixes.y());
				}				
				painter.drawLine(robotPosByPixes,obsOfLaserPosByPixes);
			}
		}

	}else{
		painter.setPen(Qt::blue);
		painter.setFont(QFont("Arial", 40));
		painter.drawText(rect(), Qt::AlignCenter, GBK::ToUnicode("地图编辑器A*"));
	}
}
void XCOverview::timerEvent(QTimerEvent *event){
	if(event->timerId() == m_timerId){
		if(m_map!=NULL){
			this->setFixedWidth(m_map->N * m_config->map_scale());
			this->setFixedHeight(m_map->M * m_config->map_scale());
		}
		x0=0,y0=0,w0=this->width(),h0=this->height();
		update();
	}
}
void XCOverview::mouseMoveEvent(QMouseEvent *event){

}
void XCOverview::mousePressEvent(QMouseEvent *event){

}
void XCOverview::mouseReleaseEvent(QMouseEvent *event){

}
