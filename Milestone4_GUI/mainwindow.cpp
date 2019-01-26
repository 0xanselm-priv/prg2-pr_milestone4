#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <iostream>
#include <vector>
#include <QPainter>
#include <QMouseEvent>
#include <math.h>

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

    status_update("Painting factor x: " + std::to_string(factor_width)+ " y: " + std::to_string(factor_height));
    factor_width = 20;
    factor_height = factor_width;

    //Important to keep at end of constructor.
    //Variable to show that all parts are loaded an programm is operational
    status_update("Init done. init = 1. Program nominal");
    init = 1;
}

MainWindow::~MainWindow()
{
    this->status_update(__FUNCTION__);
    delete ui;
}

void MainWindow::status_update(std::string status)
{
    //Function for logging all important processes
    std::string current_status = "\n";
    current_status = current_status.append(ui->status_text->toPlainText().toUtf8().constData());
    std::string new_status = status.append(current_status);
    ui->status_text->setText(QString::fromStdString(new_status));
}

void MainWindow::print_matrix()
{
    //testing purposes. Could show a picture
    qDebug() << "++here" << input_matrix.size() << " " << input_matrix[0].size();
    //fill_matrix();
    for (int i = 0; i < input_matrix.size(); i++){
        for (int j = 0; j < input_matrix[i].size(); j++) {
            std::cout << input_matrix[i][j];
        }
        std::cout << "" << std::endl;
    }

}

void MainWindow::fill_matrix()
{
    //testing purposes. Could be filled with random noise
    for (int i = 0; i < input_matrix.size(); i++){
        for (int j = 0; j < input_matrix[i].size(); j++) {
            input_matrix[i][j] = j;
        }
    }
}

void MainWindow::repaint_canvas()
{
    status_update(__FUNCTION__);
    int canvas_height = ui->canvas_label->height();
    int canvas_width = ui->canvas_label->width();
    factor_width = canvas_width / input_matrix.size();
    factor_height = canvas_height / input_matrix[0].size();
    status_update("Painting factor x: " + std::to_string(factor_width)+ " y: " + std::to_string(factor_height));

    //Rendering might be faulty ue to rounding error
    if ((factor_width * input_matrix.size() / canvas_width) == true) {
        status_update("Rounding Error eminent");
    }

    QPixmap pixmap(canvas_width, canvas_height);
    pixmap.fill(QColor("transparent"));
    QPainter painter (&pixmap);
    // void QPainter::drawRect(int x, int y, int width, int height)
    for(auto i = 0; i < input_matrix.size(); i++) {
        for (auto j = 0; j < input_matrix[i].size(); j++) {

            if (input_matrix[i][j] == 0.0) {
                painter.setBrush(Qt::black);
                painter.drawRect(i*factor_width, j*factor_height, factor_width, factor_height);
            } else if (input_matrix[i][j] == 0.25) {
                painter.setBrush(Qt::darkGray);
                painter.drawRect(i*factor_width, j*factor_height, factor_width, factor_height);
            } else if (input_matrix[i][j] == 0.5) {
                painter.setBrush(Qt::gray);
                painter.drawRect(i*factor_width, j*factor_height, factor_width, factor_height);
            } else if (input_matrix[i][j] == 0.75) {
                painter.setBrush(Qt::gray);
                painter.drawRect(i*factor_width, j*factor_height, factor_width, factor_height);
            } else if (input_matrix[i][j] == 1.0) {
                painter.setBrush(Qt::white);
                painter.drawRect(i*factor_width, j*factor_height, factor_width, factor_height);
            }

        }
    }
    ui->canvas_label->setPixmap(pixmap);
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    //thanks grg
    //TO DO
    //Change canvas_label dimensions so that factor * fixed box (e.g. 10px) doesnt produce
    //a rounding error or ugly mishap
    //Check if each tile has value <= 1.0 so that no tile has a value > 1.0
    //Check if a tile with value = 1.0 is allowed to be clicked again, so that the adjacent neighbours
    //are in creased
    QString x_str = QString::number(ev->x());
    QString y_str = QString::number(ev->y());
    int x = x_str.toInt();
    int y = y_str.toInt();
    if (x < ui->canvas_label->width() && y < ui->canvas_label->height()) {
        int tile_width = std::floor(x / factor_width);
        int tile_height = std::floor(y / factor_height);
        int tile_n_max = std::floor(ui->canvas_label->width() / factor_width);
        int tile_m_max = std::floor(ui->canvas_label->height() / factor_height);

        std::string temp = __FUNCTION__;
        temp = temp.append("Matrix: [" + std::to_string(tile_height) + "][" + std::to_string(tile_width) + "]");
        status_update(temp);

        //assign clicked tile to matrix

        input_matrix[tile_width][tile_height] = 1.0;
        //North
        if (tile_height > 0) {
            input_matrix[tile_width][tile_height-1] = input_matrix[tile_width][tile_height-1] + 0.25;
        }

        //East
        if (tile_width < tile_n_max-1) {
            qDebug() << "tile_width:" << tile_width << "tile_n:" << tile_n_max;
            input_matrix[tile_width+1][tile_height] = input_matrix[tile_width+1][tile_height] + 0.25;
        }

        //South
        if (tile_height < tile_m_max-1) {
            input_matrix[tile_width][tile_height+1] = input_matrix[tile_width][tile_height+1] + 0.25;
        }

        //West
        if (tile_width > 0) {
            input_matrix[tile_width-1][tile_height] = input_matrix[tile_width-1][tile_height] + 0.25;
        }

        print_matrix();
        repaint_canvas();
    }
}


void MainWindow::on_height_valueChanged(int arg1)
{
    //TO DO repaint of canvas label
    if (init > 0) {
        std::string temp = __FUNCTION__;
        input_matrix.clear(); //essentially needed. if not cleared, major fuck up
        this->input_matrix.resize(ui->height->value(), std::vector<float> (ui->width->value()));
        this->print_matrix();
        repaint_canvas();
    }
}

void MainWindow::on_width_valueChanged(int arg1)
{
    //TO DO repaint of canvas label
    if (init > 0) {
        std::string temp = __FUNCTION__;
        input_matrix.clear(); //essentially needed. if not cleared, major fuck up
        this->input_matrix.resize(ui->height->value(), std::vector<float> (ui->width->value()));
        this->print_matrix();
        repaint_canvas();
    }
}
