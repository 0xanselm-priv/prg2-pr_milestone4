#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <iostream>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->status_update(__TIMESTAMP__);
    this->status_update(__FUNCTION__);

    //Internal datatypes def
    init = 0;
    //input_matrix.resize(6, std::vector<float> (6));

    this->setWindowTitle("ANN - Milestone 4");
    ui->statusBar->showMessage("Initialized");
    ui->canvas_label->setFrameShape(QFrame::Box);
    ui->status_text->setReadOnly(true);

    //value definitions
    ui->height->setMinimum(1);
    ui->height->setValue(3);
    ui->height->setMaximum(800);

    ui->width->setMinimum(1);
    ui->width->setValue(3);
    ui->width->setMaximum(800);
    init = 1;
}

MainWindow::~MainWindow()
{
    this->status_update(__FUNCTION__);
    delete ui;
}

void MainWindow::status_update(std::string status)
{
    std::string current_status = "\n";
    current_status = current_status.append(ui->status_text->toPlainText().toUtf8().constData());
    std::string new_status = status.append(current_status);
    ui->status_text->setText(QString::fromStdString(new_status));
}

void MainWindow::print_matrix()
{
    qDebug() << "++here" << input_matrix.size() << " " << input_matrix[0].size();
    fill_matrix();
    for (int i = 0; i < input_matrix.size(); i++){
        for (int j = 0; j < input_matrix[i].size(); j++) {
            std::cout << input_matrix[i][j];
        }
        std::cout << "" << std::endl;
    }

}

void MainWindow::fill_matrix()
{
    for (int i = 0; i < input_matrix.size(); i++){
        for (int j = 0; j < input_matrix[i].size(); j++) {
            input_matrix[i][j] = j;
        }
    }
}


void MainWindow::on_height_valueChanged(int arg1)
{
    if (init > 0) {
        std::string temp = __FUNCTION__;
        input_matrix.clear();
        this->input_matrix.resize(ui->height->value(), std::vector<float> (ui->width->value()));
        this->print_matrix();
    }

}

void MainWindow::on_width_valueChanged(int arg1)
{
    if (init > 0) {
        std::string temp = __FUNCTION__;
        input_matrix.clear();
        this->input_matrix.resize(ui->height->value(), std::vector<float> (ui->width->value()));
        this->print_matrix();
        qDebug() << "Hi!";
    }
}
