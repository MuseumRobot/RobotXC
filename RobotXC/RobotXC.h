#pragma once
#include <QtGui/QMainWindow>
#include "ui_robotxc.h"
#include "XCVoice.h"
#include "XCConfig.h"
#include "XCOverview.h"
#include "XCControl.h"
#include "stdafx.h"
#define PI 3.141592653
#define DODGESSTEPS 50			//闪避时刻中最低有效步数
#define SPEAKWORDSPERSECOND 4	//王静每秒钟阅读的字数
class RobotXC : public QMainWindow{
	Q_OBJECT
public:
	RobotXC(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RobotXC();
private:
	Ui::RobotXCClass ui;
	XCVoice* m_voice;				//语音组件(负责TTS与ASR,如果ASR正常运行则通过在其回调函数中emit signal来发送识别结果)
	XCConfig* m_config;				//配置组件(负责管理预设参数)
	XCOverview* m_overview;			//地图组件(仅负责绘图与交互,尽量避免逻辑运算)
	XCControl* m_control;			//控制台组件
	AStar* m_astar;					//A*寻路计算器
	XCMap* m_map;					//存储地图
	XCTaskDataTypeList m_TaskDataRecords;			//所有任务点记录
	XCSpeakContentTypeList m_SpeakContentRecords;	//所有语料记录
	int timer_instruction;				//指令计时器
	int timer_data_refresh;				//数据刷新计时器
	int m_speakWaitCycle;				//语音播报等待指令周期数
	std::list<XCPoint> m_result;		//存储A*计算器计算出的路径点列表
	std::list<QPointF> m_result_f;		//存储A*计算器计算出的路径点的世界坐标
	QPointF robotPos;				//机器人坐标(cm,cm)
	QPointF goalPos;				//目标坐标(cm,cm)
	float robotFaceAngle;			//机器人朝向角(°)
	float faceAudianceAngle;		//面朝观众的角度(°)
	bool isSimulateMode;			//是否是模拟模式
	bool isAutoMode;				//是否是自动模式
	bool isFastMode;
	bool LoadMap();							//读取地图
	bool LoadTask();						//读取任务库
	bool LoadSpeakContent();				//读取语料库
	int JudgeTaskType(int taskID);			//判断当前任务类型，位移为0，语音为1
	void timerEvent(QTimerEvent *event);	//计时器循环函数
	void TrunForwardGoal(float goalAngle);	//朝向目标
	void TurnLeft(float ratio);				//左转
	void TurnRight(float ratio);			//右转
	void MoveForward(float ratio);			//前进
	void MoveBackward(float ratio);			//后退
	void AssignInstruction();				//安排指令
	void AssignPresetTask(int n);			//安排预设任务
	void DataRefresh();						//数据刷新
	float GetAngleFromVector(QPointF delta);		//由向量求角度
	float Modf360(float angle);						//将角度换算到(0,360)之间
	void GetResultF();				//依据m_result计算出m_result_f
	XCTaskDataType* findTask(int taskId);
	XCSpeakContentType* findSpeakContent(int speakContentId);
private slots:
	void OnBtnRecord();				//开始聆听
	void OnBtnDisplayDilate();		//显示膨胀地图
public slots:
	void OnBtnChangeSimulateMode();	//更改模拟模式
	void OnBtnChangeAutoMode();		//更改自动模式
	void OnBtnChangeFastMode();		//更改快速模式
	void OnBtnTurnLeft();			//按钮左转
	void OnBtnTurnRight();			//按钮右转
	void OnBtnMoveForward();		//按钮前进
	void OnBtnMoveBackward();		//按钮后退
	void OnBtnSpeak();				//按钮说话
	void OnBtnStopSpeak();			//按钮停止说话
	void OnBtnPreset1();			//按钮预设1
	void OnBtnPreset2();			//按钮预设2
	void OnBtnPreset3();			//按钮预设3
	void OnBtnHome();				//按钮回家
};


