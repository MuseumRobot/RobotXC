#pragma once
#include <QWidget>
#include "ui_XCControl.h"

class XCControl : public QWidget{
	Q_OBJECT

public:
	XCControl(QWidget *parent = 0);
	~XCControl();
	Ui::XCControl ui;
	std::list<int> m_taskSequence;	//�洢��ǰ��������
	void UpdateTaskSequence();
	void ParseTaskSequence(QString str);	//����QString��ʽ�������嵥����ֵ��m_taskSequence�б�
private slots:
	void OnBtnSelfTask();
};
