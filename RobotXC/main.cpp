#include "stdafx.h"
#include "RobotXC.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	RobotXC w;
	w.show();
	return a.exec();
}
