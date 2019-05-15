#include "stdafx.h"
#include "RobotXC.h"

RobotXC::RobotXC(QWidget *parent, Qt::WFlags flags):QMainWindow(parent, flags){
	ui.setupUi(this);
	connect(ui.btnRecord,SIGNAL(clicked()),this,SLOT(OnBtnRecord()));
}

RobotXC::~RobotXC(){}

void RobotXC::OnBtnRecord(){
	m_voice.StartRecord();
}
