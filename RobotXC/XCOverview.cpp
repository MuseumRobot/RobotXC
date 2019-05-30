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
		for(std::list<XCPoint>::iterator iter = m_result.begin(); iter != m_result.end(); iter++) 
			painter.drawImage(QRect(x0+w*iter->y,y0+h*iter->x,w,h),QImage("Resources/foot.png"));

		//绘制机器人本体位置朝向
		QMatrix matrix;
		matrix.rotate(180-m_robotFaceAngle);			//rotate默认顺时针旋转
		QImage img_robot = QImage("Resources/yellowCar.png");
		painter.drawImage(m_robotPos,img_robot.scaled(m_config->map_scale(),m_config->map_scale()).transformed(matrix,Qt::FastTransformation));	
		//绘制视野
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(255,0,0,100));
		painter.drawPie(QRect(x0-m_config->map_scale()*8,y0-m_config->map_scale()*8,m_config->map_scale()*8*2,m_config->map_scale()*8*2),150*16,360*16);
		painter.setBrush(QColor(0,255,0,100));
		painter.drawPie(QRect(20,20,400,400),20*16,200*16);
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
