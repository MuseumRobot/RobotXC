#pragma once
#include <QWidget>
#include "ui_XCControl.h"

class XCControl : public QWidget{
	Q_OBJECT

public:
	XCControl(QWidget *parent = 0);
	~XCControl();
	Ui::XCControl ui;
};
