#pragma once
#include <QWidget>
#include "ui_XCOverview.h"
#include "stdafx.h"
class XCOverview : public QWidget{
	Q_OBJECT
public:
	XCOverview(QWidget *parent = 0);
	~XCOverview();
	Map* m_map;
	QPoint m_robotPos;
	QPoint m_mousePos;
private:
	Ui::XCOverview ui;
	int x0,y0,w0,h0;	//绘图板的位置与大小
	int m_timerId;
	bool LoadMap();
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
};
