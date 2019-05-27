#pragma once
#include <QtGui/QMainWindow>
#include "ui_robotxc.h"
#include "XCVoice.h"
#include "XCConfig.h"
#include "XCOverview.h"
#define PI 3.141592653
#define DODGESSTEPS 50			//闪避时刻中最低有效步数
#define SPEAKWORDSPERSECOND 4	//王静每秒钟阅读的字数
#define PATHTASKTYPE 0			//位移任务点类型
#define SPEAKTASKTYPE 1			//语音任务点类型
class RobotXC : public QMainWindow{
	Q_OBJECT

public:
	RobotXC(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RobotXC();

private:
	Ui::RobotXCClass ui;
	XCVoice* m_voice;				//语音组件
	XCConfig* m_config;				//配置组件
	XCOverview* m_overview;			//地图组件
	int timer_instruction;			//指令计时器
	void timerEvent(QTimerEvent *event);
private slots:
	void OnBtnRecord();
};


