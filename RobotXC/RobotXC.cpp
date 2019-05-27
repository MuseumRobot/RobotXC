#include "stdafx.h"
#include "RobotXC.h"
RobotXC::RobotXC(QWidget *parent, Qt::WFlags flags):QMainWindow(parent, flags){
	ui.setupUi(this);
	m_voice = new XCVoice;
	m_config = new XCConfig;
	m_overview = new XCOverview;
	ui.overview->setWidget(m_overview);
	m_overview->setFixedWidth(1600);
	m_overview->setFixedHeight(1000);
	connect(ui.btnRecord,SIGNAL(clicked()),this,SLOT(OnBtnRecord()));
	timer_instruction = startTimer(m_config->instruction_cycle());		//启动指令周期计时器
}
RobotXC::~RobotXC(){
	delete m_voice;
	delete m_config;
}
void RobotXC::timerEvent(QTimerEvent *event){
	if(event->timerId() == timer_instruction){

	}
}
void RobotXC::OnBtnRecord(){
	//m_voice->StartRecord();
	if(m_overview->m_map!=NULL){
		delete m_overview->m_map;
		m_overview->m_map = NULL;
	}
}
