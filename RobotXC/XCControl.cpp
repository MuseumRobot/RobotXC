#include "StdAfx.h"
#include "XCControl.h"

XCControl::XCControl(QWidget *parent):QWidget(parent){
	ui.setupUi(this);
	connect(ui.btnSelfTask,SIGNAL(clicked()),this,SLOT(OnBtnSelfTask()));
}
XCControl::~XCControl(){}
void XCControl::UpdateTaskSequence(){
	QString str_taskSequence;
	for(std::list<int>::iterator iter = m_taskSequence.begin(); iter!= m_taskSequence.end(); iter++){
		QString num = QString("%1").arg(*iter);
		str_taskSequence.append(num);
		str_taskSequence += ",";
	}
	str_taskSequence.resize(str_taskSequence.size()-1);
	ui.textCurrentTask->setText(str_taskSequence);
}
void XCControl::OnBtnSelfTask(){
	QString str_taskSequence = ui.textSelfTask->toPlainText();
	ParseTaskSequence(str_taskSequence);
}
void XCControl::ParseTaskSequence(QString str){
	m_taskSequence.clear();
	char* s = NULL;
	QByteArray ba = str.toLatin1();
	s = ba.data();
	QString tempNum;
	for(int i = 0;s[i] != '\0';i++){
		char a = s[i];
		if(a != ','){	//û�ж���','�ͼ�����������ӵ�tempNum���ݴ�
			if(a != ' ') tempNum.append(a);	//��ȡ����ʱ���Կո�
		}else{			//�������','�ͽ��ݴ�����ֵ�����todoList��
			m_taskSequence.push_back(tempNum.toInt());
			tempNum = "";
		}
	}
	if(tempNum != "")	m_taskSequence.push_back(tempNum.toInt());
	UpdateTaskSequence();
}