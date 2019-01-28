/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *canvas_label;
    QTextEdit *status_text;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpinBox *height;
    QLabel *label_2;
    QSpinBox *width;
    QPushButton *reset_button;
    QPushButton *train_button;
    QPushButton *test_button;
    QCheckBox *log_matrix_checkbox;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(430, 504);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        canvas_label = new QLabel(centralWidget);
        canvas_label->setObjectName(QString::fromUtf8("canvas_label"));
        canvas_label->setGeometry(QRect(0, 0, 281, 301));
        status_text = new QTextEdit(centralWidget);
        status_text->setObjectName(QString::fromUtf8("status_text"));
        status_text->setEnabled(true);
        status_text->setGeometry(QRect(10, 320, 211, 101));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(300, 50, 121, 131));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        height = new QSpinBox(groupBox);
        height->setObjectName(QString::fromUtf8("height"));

        verticalLayout->addWidget(height);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        width = new QSpinBox(groupBox);
        width->setObjectName(QString::fromUtf8("width"));

        verticalLayout->addWidget(width);

        reset_button = new QPushButton(centralWidget);
        reset_button->setObjectName(QString::fromUtf8("reset_button"));
        reset_button->setGeometry(QRect(10, 450, 80, 21));
        train_button = new QPushButton(centralWidget);
        train_button->setObjectName(QString::fromUtf8("train_button"));
        train_button->setGeometry(QRect(150, 450, 80, 21));
        test_button = new QPushButton(centralWidget);
        test_button->setObjectName(QString::fromUtf8("test_button"));
        test_button->setGeometry(QRect(320, 450, 80, 21));
        log_matrix_checkbox = new QCheckBox(centralWidget);
        log_matrix_checkbox->setObjectName(QString::fromUtf8("log_matrix_checkbox"));
        log_matrix_checkbox->setGeometry(QRect(300, 10, 131, 19));
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        canvas_label->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "Canvas Dimension:", nullptr));
        label->setText(QApplication::translate("MainWindow", "m Dimension:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "n Dimension:", nullptr));
        reset_button->setText(QApplication::translate("MainWindow", "Reset", nullptr));
        train_button->setText(QApplication::translate("MainWindow", "Train NN", nullptr));
        test_button->setText(QApplication::translate("MainWindow", "Test NN", nullptr));
        log_matrix_checkbox->setText(QApplication::translate("MainWindow", "Log Program", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
