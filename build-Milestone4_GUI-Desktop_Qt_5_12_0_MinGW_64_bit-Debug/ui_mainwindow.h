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
    QCheckBox *log_matrix_checkbox;
    QGroupBox *groupBox_2;
    QLabel *output_canvas_label;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QPushButton *reset_button;
    QPushButton *train_button;
    QPushButton *test_single_button;
    QPushButton *test_batch_button;
    QLabel *output_label;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(548, 505);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        canvas_label = new QLabel(centralWidget);
        canvas_label->setObjectName(QString::fromUtf8("canvas_label"));
        canvas_label->setGeometry(QRect(0, 0, 281, 301));
        status_text = new QTextEdit(centralWidget);
        status_text->setObjectName(QString::fromUtf8("status_text"));
        status_text->setEnabled(true);
        status_text->setGeometry(QRect(10, 340, 271, 101));
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

        log_matrix_checkbox = new QCheckBox(centralWidget);
        log_matrix_checkbox->setObjectName(QString::fromUtf8("log_matrix_checkbox"));
        log_matrix_checkbox->setGeometry(QRect(300, 10, 131, 19));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(440, 10, 91, 451));
        output_canvas_label = new QLabel(groupBox_2);
        output_canvas_label->setObjectName(QString::fromUtf8("output_canvas_label"));
        output_canvas_label->setGeometry(QRect(0, 20, 81, 431));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(300, 200, 121, 143));
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        reset_button = new QPushButton(groupBox_3);
        reset_button->setObjectName(QString::fromUtf8("reset_button"));

        verticalLayout_2->addWidget(reset_button);

        train_button = new QPushButton(groupBox_3);
        train_button->setObjectName(QString::fromUtf8("train_button"));

        verticalLayout_2->addWidget(train_button);

        test_single_button = new QPushButton(groupBox_3);
        test_single_button->setObjectName(QString::fromUtf8("test_single_button"));

        verticalLayout_2->addWidget(test_single_button);

        test_batch_button = new QPushButton(groupBox_3);
        test_batch_button->setObjectName(QString::fromUtf8("test_batch_button"));

        verticalLayout_2->addWidget(test_batch_button);

        output_label = new QLabel(centralWidget);
        output_label->setObjectName(QString::fromUtf8("output_label"));
        output_label->setGeometry(QRect(300, 340, 121, 121));
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
        log_matrix_checkbox->setText(QApplication::translate("MainWindow", "Log Program", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Network Output:", nullptr));
        output_canvas_label->setText(QString());
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Controls:", nullptr));
        reset_button->setText(QApplication::translate("MainWindow", "Reset", nullptr));
        train_button->setText(QApplication::translate("MainWindow", "Train NN", nullptr));
        test_single_button->setText(QApplication::translate("MainWindow", "Test Single NN", nullptr));
        test_batch_button->setText(QApplication::translate("MainWindow", "Test Batch NN", nullptr));
        output_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
