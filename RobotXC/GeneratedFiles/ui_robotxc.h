/********************************************************************************
** Form generated from reading UI file 'robotxc.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROBOTXC_H
#define UI_ROBOTXC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RobotXCClass
{
public:
    QWidget *centralWidget;
    QPushButton *btnRecord;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RobotXCClass)
    {
        if (RobotXCClass->objectName().isEmpty())
            RobotXCClass->setObjectName(QString::fromUtf8("RobotXCClass"));
        RobotXCClass->resize(539, 400);
        centralWidget = new QWidget(RobotXCClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        btnRecord = new QPushButton(centralWidget);
        btnRecord->setObjectName(QString::fromUtf8("btnRecord"));
        btnRecord->setGeometry(QRect(270, 250, 75, 23));
        RobotXCClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RobotXCClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 539, 23));
        RobotXCClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RobotXCClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        RobotXCClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RobotXCClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        RobotXCClass->setStatusBar(statusBar);

        retranslateUi(RobotXCClass);

        QMetaObject::connectSlotsByName(RobotXCClass);
    } // setupUi

    void retranslateUi(QMainWindow *RobotXCClass)
    {
        RobotXCClass->setWindowTitle(QApplication::translate("RobotXCClass", "RobotXC", 0, QApplication::UnicodeUTF8));
        btnRecord->setText(QApplication::translate("RobotXCClass", "\345\274\200\345\247\213\345\275\225\351\237\263", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RobotXCClass: public Ui_RobotXCClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROBOTXC_H
