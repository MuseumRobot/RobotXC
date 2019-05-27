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
	XCVoice* m_voice;				//�������(����TTS��ASR,���ASR����������ͨ������ص�������emit signal������ʶ����)
	XCConfig* m_config;				//�������(�������Ԥ�����)
	XCOverview* m_overview;			//��ͼ���(�������ͼ�뽻��,���������߼�����)
	int timer_instruction;			//ָ���ʱ��
	void timerEvent(QTimerEvent *event);

	QPointF robotPos;				//����������(cm,cm)
	float robotFaceAngle;			//�����˳����(��)
private slots:
	void OnBtnRecord();
};


