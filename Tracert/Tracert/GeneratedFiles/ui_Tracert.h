/********************************************************************************
** Form generated from reading UI file 'Tracert.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRACERT_H
#define UI_TRACERT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TracertClass
{
public:
    QWidget *centralWidget;
    QWidget *widget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer;
    QLabel *RIP_2;
    QLabel *RIP;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QLabel *label_2;
    QListWidget *listWidget;
    QWidget *content;
    QHBoxLayout *horizontalLayout_2;
    QLabel *A;
    QLabel *B;
    QLabel *C;
    QLabel *D;
    QLabel *E;
    QLabel *F;
    QLabel *G;
    QLabel *H;
    QLabel *I;
    QLabel *label_3;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TracertClass)
    {
        if (TracertClass->objectName().isEmpty())
            TracertClass->setObjectName(QStringLiteral("TracertClass"));
        TracertClass->resize(1074, 672);
        centralWidget = new QWidget(TracertClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 0, 1001, 581));
        horizontalLayoutWidget = new QWidget(widget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(70, 100, 851, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(15);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(horizontalLayoutWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(lineEdit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        RIP_2 = new QLabel(horizontalLayoutWidget);
        RIP_2->setObjectName(QStringLiteral("RIP_2"));
        RIP_2->setStyleSheet(QStringLiteral(""));

        horizontalLayout->addWidget(RIP_2);

        RIP = new QLabel(horizontalLayoutWidget);
        RIP->setObjectName(QStringLiteral("RIP"));
        RIP->setStyleSheet(QStringLiteral("background-color: rgb(215, 215, 215);"));

        horizontalLayout->addWidget(RIP);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(pushButton);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 4);
        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(4, 5);
        horizontalLayout->setStretch(5, 2);
        horizontalLayout->setStretch(6, 2);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(210, 10, 621, 71));
        label_2->setStyleSheet(QStringLiteral("font: 28pt \"Microsoft YaHei UI\";"));
        label_2->setAlignment(Qt::AlignCenter);
        listWidget = new QListWidget(widget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(40, 190, 921, 401));
        content = new QWidget(widget);
        content->setObjectName(QStringLiteral("content"));
        content->setGeometry(QRect(40, 149, 921, 31));
        horizontalLayout_2 = new QHBoxLayout(content);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 10, 0);
        A = new QLabel(content);
        A->setObjectName(QStringLiteral("A"));

        horizontalLayout_2->addWidget(A);

        B = new QLabel(content);
        B->setObjectName(QStringLiteral("B"));

        horizontalLayout_2->addWidget(B);

        C = new QLabel(content);
        C->setObjectName(QStringLiteral("C"));

        horizontalLayout_2->addWidget(C);

        D = new QLabel(content);
        D->setObjectName(QStringLiteral("D"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(D->sizePolicy().hasHeightForWidth());
        D->setSizePolicy(sizePolicy3);
        D->setStyleSheet(QStringLiteral(""));

        horizontalLayout_2->addWidget(D);

        E = new QLabel(content);
        E->setObjectName(QStringLiteral("E"));

        horizontalLayout_2->addWidget(E);

        F = new QLabel(content);
        F->setObjectName(QStringLiteral("F"));

        horizontalLayout_2->addWidget(F);

        G = new QLabel(content);
        G->setObjectName(QStringLiteral("G"));

        horizontalLayout_2->addWidget(G);

        H = new QLabel(content);
        H->setObjectName(QStringLiteral("H"));

        horizontalLayout_2->addWidget(H);

        I = new QLabel(content);
        I->setObjectName(QStringLiteral("I"));

        horizontalLayout_2->addWidget(I);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 2);
        horizontalLayout_2->setStretch(2, 2);
        horizontalLayout_2->setStretch(3, 1);
        horizontalLayout_2->setStretch(4, 1);
        horizontalLayout_2->setStretch(5, 1);
        horizontalLayout_2->setStretch(6, 1);
        horizontalLayout_2->setStretch(7, 1);
        horizontalLayout_2->setStretch(8, 1);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(880, 600, 131, 21));
        TracertClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TracertClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1074, 21));
        TracertClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(TracertClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TracertClass->setStatusBar(statusBar);

        retranslateUi(TracertClass);
        QObject::connect(pushButton, SIGNAL(clicked()), TracertClass, SLOT(tracerting()));

        QMetaObject::connectSlotsByName(TracertClass);
    } // setupUi

    void retranslateUi(QMainWindow *TracertClass)
    {
        TracertClass->setWindowTitle(QApplication::translate("TracertClass", "Tracert", nullptr));
        label->setText(QApplication::translate("TracertClass", "IP/\345\237\237\345\220\215", nullptr));
        RIP_2->setText(QApplication::translate("TracertClass", "\347\234\237\345\256\236IP\357\274\232", nullptr));
        RIP->setText(QApplication::translate("TracertClass", "\347\234\237\345\256\236IP", nullptr));
        pushButton->setText(QApplication::translate("TracertClass", "Tracert Now", nullptr));
        label_2->setText(QApplication::translate("TracertClass", "\345\217\257\350\247\206\345\214\226Tracert\350\277\275\350\270\252\345\267\245\345\205\267", nullptr));
        A->setText(QApplication::translate("TracertClass", "Router", nullptr));
        B->setText(QApplication::translate("TracertClass", "Source IP", nullptr));
        C->setText(QApplication::translate("TracertClass", "Destination IP", nullptr));
        D->setText(QApplication::translate("TracertClass", "One", nullptr));
        E->setText(QApplication::translate("TracertClass", "Two", nullptr));
        F->setText(QApplication::translate("TracertClass", "Tree", nullptr));
        G->setText(QApplication::translate("TracertClass", "Four", nullptr));
        H->setText(QApplication::translate("TracertClass", "Average", nullptr));
        I->setText(QApplication::translate("TracertClass", "Variance", nullptr));
        label_3->setText(QApplication::translate("TracertClass", "Power by\357\274\232MARXCBR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TracertClass: public Ui_TracertClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACERT_H
