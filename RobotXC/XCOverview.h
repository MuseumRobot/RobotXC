#pragma once
#include <QWidget>
#include "ui_XCOverview.h"
#include "stdafx.h"

class XCOverview : public QWidget{
	Q_OBJECT
public:
	XCOverview(QWidget *parent = 0);
	~XCOverview();
	XCMap* m_map;
	XCConfig* m_config;
	QPointF* m_robotPos;
	QPointF* m_goalPos;
	float* m_robotFaceAngle;
	QPoint m_mousePos;
	std::list<XCPoint>* m_result;
	bool m_showDilateMap;
	float* m_simulateLaserResult;
private:
	Ui::XCOverview ui;
	int x0,y0,w0,h0;	//绘图板的位置与大小
	int m_timerId;
	
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
};
