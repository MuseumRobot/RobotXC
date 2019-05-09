#include "stdafx.h"
#include "RobotXC.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	RobotXC w;
	w.show();
	return a.exec();
}
