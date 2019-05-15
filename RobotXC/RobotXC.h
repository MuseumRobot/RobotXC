#ifndef ROBOTXC_H
#define ROBOTXC_H

#include <QtGui/QMainWindow>
#include "ui_robotxc.h"
#include "XCVoice.h"

class RobotXC : public QMainWindow{
	Q_OBJECT

public:
	RobotXC(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RobotXC();

private:
	Ui::RobotXCClass ui;
	XCVoice m_voice;

private slots:
	void OnBtnRecord();
};

#endif // ROBOTXC_H
