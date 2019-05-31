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
	XCMap* m_map;					//�洢��ͼ
	XCTaskDataTypeList m_TaskDataRecords;			//����������¼
	XCSpeakContentTypeList m_SpeakContentRecords;	//�������ϼ�¼
	int timer_instruction;				//ָ���ʱ��
	int timer_data_refresh;				//����ˢ�¼�ʱ��
	int m_speakWaitCycle;				//���������ȴ�ָ��������
	std::list<XCPoint> m_result;		//�洢A*�������������·�����б�
	std::list<QPointF> m_result_f;		//�洢A*�������������·�������������
	QPointF robotPos;				//����������(cm,cm)
	QPointF goalPos;				//Ŀ������(cm,cm)
	float robotFaceAngle;			//�����˳����(��)
	float faceAudianceAngle;		//�泯���ڵĽǶ�(��)
	bool isSimulateMode;			//�Ƿ���ģ��ģʽ
	bool isAutoMode;				//�Ƿ����Զ�ģʽ
	bool isFastMode;
	bool LoadMap();							//��ȡ��ͼ
	bool LoadTask();						//��ȡ�����
	bool LoadSpeakContent();				//��ȡ���Ͽ�
	int JudgeTaskType(int taskID);			//�жϵ�ǰ�������ͣ�λ��Ϊ0������Ϊ1
	void timerEvent(QTimerEvent *event);	//��ʱ��ѭ������
	void TrunForwardGoal(float goalAngle);	//����Ŀ��
	void TurnLeft(float ratio);				//��ת
	void TurnRight(float ratio);			//��ת
	void MoveForward(float ratio);			//ǰ��
	void MoveBackward(float ratio);			//����
	void AssignInstruction();				//����ָ��
	void AssignPresetTask(int n);			//����Ԥ������
	void DataRefresh();						//����ˢ��
	float GetAngleFromVector(QPointF delta);		//��������Ƕ�
	float Modf360(float angle);						//���ǶȻ��㵽(0,360)֮��
	void GetResultF();				//����m_result�����m_result_f
	XCTaskDataType* findTask(int taskId);
	XCSpeakContentType* findSpeakContent(int speakContentId);
private slots:
	void OnBtnRecord();				//��ʼ����
	void OnBtnDisplayDilate();		//��ʾ���͵�ͼ
public slots:
	void OnBtnChangeSimulateMode();	//����ģ��ģʽ
	void OnBtnChangeAutoMode();		//�����Զ�ģʽ
	void OnBtnChangeFastMode();		//���Ŀ���ģʽ
	void OnBtnTurnLeft();			//��ť��ת
	void OnBtnTurnRight();			//��ť��ת
	void OnBtnMoveForward();		//��ťǰ��
	void OnBtnMoveBackward();		//��ť����
	void OnBtnSpeak();				//��ť˵��
	void OnBtnStopSpeak();			//��ťֹͣ˵��
	void OnBtnPreset1();			//��ťԤ��1
	void OnBtnPreset2();			//��ťԤ��2
	void OnBtnPreset3();			//��ťԤ��3
	void OnBtnHome();				//��ť�ؼ�
};


