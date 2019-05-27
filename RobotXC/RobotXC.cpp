#include "stdafx.h"
#include "RobotXC.h"
RobotXC::RobotXC(QWidget *parent, Qt::WFlags flags):QMainWindow(parent, flags){
	ui.setupUi(this);
	m_voice = new XCVoice;
	m_config = new XCConfig;
	m_overview = new XCOverview;
	ui.overview->setWidget(m_overview);
	if(m_overview->m_map!=NULL){
		m_overview->setFixedWidth(m_overview->m_map->N * m_config->map_scale());
		m_overview->setFixedHeight(m_overview->m_map->M * m_config->map_scale());
	}
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
