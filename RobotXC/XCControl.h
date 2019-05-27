#ifndef XCCONTROL_H
#define XCCONTROL_H

#include <QWidget>
#include "ui_XCControl.h"

class XCControl : public QWidget
{
	Q_OBJECT

public:
	XCControl(QWidget *parent = 0);
	~XCControl();

private:
	Ui::XCControl ui;
};

#endif // XCCONTROL_H
