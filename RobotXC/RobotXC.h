#pragma once
#include <QtGui/QMainWindow>
#include "ui_robotxc.h"
#include "XCVoice.h"
#include "XCConfig.h"
#include "XCOverview.h"
#include "XCControl.h"
#include "XCSimulateLaser.h"
#include "stdafx.h"
#define DODGESTEPS 8			//����ʱ���������Ч����
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
	XCSimulateLaser* m_simulateLaser;	//���⼤���״�
	XCTaskDataTypeList m_TaskDataRecords;			//����������¼
	XCSpeakContentTypeList m_SpeakContentRecords;	//�������ϼ�¼
	int timer_instruction;				//ָ���ʱ��
	int timer_data_refresh;				//����ˢ�¼�ʱ��
	int m_speakWaitCycle;				//���������ȴ�ָ��������
	int dodgeMoveSetps;					//�Ѿ����ܵĲ���
	int dodgeMethod;					//���ܷ���(0����,1��,2��)
	std::list<XCPoint> m_result;		//�洢A*�������������·�����б�
	QPointF robotPos;				//����������(cm,cm)
	QPointF goalPos;				//Ŀ������(cm,cm)
	float robotFaceAngle;			//�����˳����(��)
	float faceAudianceAngle;		//�泯���ڵĽǶ�(��)
	bool isSimulateMode;			//�Ƿ���ģ��ģʽ
	bool isAutoMode;				//�Ƿ����Զ�ģʽ
	bool isFastMode;				//�Ƿ��ǿ���ģʽ
	bool isDodgeMode;				//�Ƿ�������ģʽ
	bool LoadMap();							//��ȡ��ͼ
	bool LoadTask();						//��ȡ�����
	bool LoadSpeakContent();				//��ȡ���Ͽ�
	bool JudgeWhetherEnterDodgeModeOrShoutOut();	//�ж��ǽ�������ģʽ���Ǻ����뿪
	int JudgeTaskType(int taskID);			//�жϵ�ǰ�������ͣ�λ��Ϊ0������Ϊ1
	int GetDodgeMethodByAstar();			//ͨ��A*��ȡ���ܷ���
	void timerEvent(QTimerEvent *event);	//��ʱ��ѭ������
	void TrunForwardGoal(float goalAngle);	//����Ŀ��
	void TurnLeft(float ratio);				//��ת
	void TurnRight(float ratio);			//��ת
	void MoveForward(float ratio);			//ǰ��
	void MoveBackward(float ratio);			//����
	void AssignInstruction();				//����ָ��
	void AssignPresetTask(int n);			//����Ԥ������
	void DataRefresh();						//����ˢ��
	void CommomMeasures();					//��ָͨ��
	void DodgeMeasures();					//����ָ��
	void DodgeLeft();						//��������
	void DodgeRight();						//��������
	std::list<XCPoint> GetAstarResult(bool isIgnoreCorner, bool isWithDynamicObstacle);
	XCTaskDataType* findTask(int taskId);
	XCSpeakContentType* findSpeakContent(int speakContentId);
private slots:
	void OnBtnRecord();				//��ʼ����
	void OnBtnDisplayDilate();		//��ʾ���͵�ͼ
	void OnBtnSetDynamicBarrier();	//���ö�̬�ϰ�
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


