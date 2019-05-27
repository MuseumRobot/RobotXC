#pragma once
#include <QtGui/QMainWindow>
#include "ui_robotxc.h"
#include "XCVoice.h"
#include "XCConfig.h"
#include "XCOverview.h"
#define PI 3.141592653
#define DODGESSTEPS 50			//����ʱ���������Ч����
#define SPEAKWORDSPERSECOND 4	//����ÿ�����Ķ�������
#define PATHTASKTYPE 0			//λ�����������
#define SPEAKTASKTYPE 1			//�������������
class RobotXC : public QMainWindow{
	Q_OBJECT

public:
	RobotXC(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RobotXC();

private:
	Ui::RobotXCClass ui;
	XCVoice* m_voice;				//�������
	XCConfig* m_config;				//�������
	XCOverview* m_overview;			//��ͼ���
	int timer_instruction;			//ָ���ʱ��
	void timerEvent(QTimerEvent *event);
private slots:
	void OnBtnRecord();
};


