#pragma once
#include <QtGui/QMainWindow>
#include "ui_robotxc.h"
#include "XCVoice.h"
#include "XCConfig.h"
#include "XCOverview.h"
#include "XCControl.h"
#include "stdafx.h"
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
	XCControl* m_control;			//����̨���
	AStar* m_astar;					//A*Ѱ·������
	Map* m_map;						//�洢��ͼ
	int timer_instruction;			//ָ���ʱ��
	std::list<Point> m_result;		//�洢A*�������������·�����б�
	QPointF robotPos;				//����������(cm,cm)
	QPointF goalPos;				//Ŀ������(cm,cm)
	float robotFaceAngle;			//�����˳����(��)
	bool isSimulateMode;			//�Ƿ���ģ��ģʽ
	bool LoadMap();							//��ȡ��ͼ
	void timerEvent(QTimerEvent *event);	//��ʱ��ѭ������
	void TrunForwardGoal();					//����Ŀ��
	void TurnLeft(float ratio);				//��ת
	void TurnRight(float ratio);			//��ת
	void MoveForward(float ratio);			//ǰ��
	void MoveBackward(float ratio);			//����
	void AssignInstruction();				//����ָ��
	float GetAngleFromVector(QPointF delta);		//��������Ƕ�
	float Modf360(float angle);						//���ǶȻ��㵽(0,360)֮��
private slots:
	void OnBtnRecord();				//��ʼ����
	void OnBtnDisplayDilate();		//��ʾ���͵�ͼ
public slots:
	void OnBtnChangeSimulateMode();
};


