#ifndef ROBOTXC_H
#define ROBOTXC_H

#include <QtGui/QMainWindow>
#include "ui_robotxc.h"

class RobotXC : public QMainWindow
{
	Q_OBJECT

public:
	RobotXC(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RobotXC();

private:
	Ui::RobotXCClass ui;
};

#endif // ROBOTXC_H
