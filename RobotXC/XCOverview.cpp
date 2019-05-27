#include "StdAfx.h"
#include "XCOverview.h"

XCOverview::XCOverview(QWidget *parent):QWidget(parent){
	ui.setupUi(this);
	m_map = NULL;
	m_timerId = startTimer(30);
	LoadMap();
	x0=0,y0=0,w0=this->width(),h0=this->height();
}

XCOverview::~XCOverview(){
	if(m_map!=NULL){
		delete m_map;
		m_map = NULL;
	}
}
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
				if(m_map->m_maze[i][j] == 1){
					painter.setBrush(QColor(100,100,100));
					painter.drawRect(r);
				}else{
					painter.setBrush(QColor(220,220,220));
					painter.drawRect(r);
				}
			}
		}
		if(m_map->x_start>-1&&m_map->y_start>-1){
			painter.drawImage(QRect(x0+w*m_map->y_start,y0+h*m_map->x_start,w,h),QImage("Resources/start.png"));
		}
		if(m_map->x_end>-1&&m_map->y_end>-1){
			painter.drawImage(QRect(x0+w*m_map->y_end,y0+h*m_map->x_end,w,h),QImage("Resources/end.png"));
		}
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(255,0,0,100));
		painter.drawPie(QRect(x0,y0,400,400),150*16,360*16);
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

bool XCOverview::LoadMap(){
	QString filepath = "./Configure/museum.map";
	if(filepath.length()>0){
		string gbk_name=GBK::FromUnicode(filepath);
		//打开文件，读取内容
		FILE* fp=fopen(gbk_name.c_str(),"rb");
		//文件的大小
		if(fp != NULL){
			int M,N;
			fread(&M,sizeof(int),1,fp);
			fread(&N,sizeof(int),1,fp);
			m_map = new Map(M,N);
			for(int i=0;i<M;i++){
				for(int j=0;j<N;j++){
					fread(&(m_map->m_maze[i][j]),sizeof(int),1,fp);
					fread(&(m_map->m_weight[i][j]),sizeof(int),1,fp);
				}
			}
			fclose(fp);	//关闭文件
		}else{
			return false;
		}
	}else{
		return false;
	}
	return true;
}