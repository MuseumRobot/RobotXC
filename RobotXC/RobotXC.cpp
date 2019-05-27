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
}
RobotXC::~RobotXC(){
	delete m_voice;
	delete m_config;
}
void RobotXC::OnBtnRecord(){
	//m_voice->StartRecord();
	if(m_overview->m_map!=NULL){
		delete m_overview->m_map;
		m_overview->m_map = NULL;
	}
}
