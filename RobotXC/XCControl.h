#pragma once
#include <QWidget>
#include "ui_XCControl.h"

class XCControl : public QWidget{
	Q_OBJECT

public:
	XCControl(QWidget *parent = 0);
	~XCControl();
	Ui::XCControl ui;
	std::list<int> m_taskSequence;	//存储当前任务序列
	void UpdateTaskSequence();
	void ParseTaskSequence(QString str);	//解析QString格式的任务清单，赋值给m_taskSequence列表
private slots:
	void OnBtnSelfTask();
};
