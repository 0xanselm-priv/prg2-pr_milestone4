#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <iostream>
#include <vector>
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QFileDialog>
#include <QInputDialog>

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
    ui->height->setValue(28);
    ui->height->setMaximum(50);

    ui->width->setMinimum(1);
    ui->width->setValue(28);
    ui->width->setMaximum(50);

    status_update("Painting factor x: " + std::to_string(factor_width)+ " y: " + std::to_string(factor_height));
    factor_width = 20;
    factor_height = factor_width;

    ui->log_matrix_checkbox->setCheckState(Qt::CheckState::Unchecked);

    is_trained = 0;

    NeuralNet net({784,64,10});
    net.eta = 0.3;
    net.toggle_adaptive_learning = true;
    net.setActivationFunction(&logistic, &logistic_derived);

    //Important to keep at end of constructor.
    //Variable to show that all parts are loaded an programm is operational
    status_update("Init done. init = 1. Program nominal");
    emit ui->reset_button->click();
    init = 1;
    //emit ui->reset_button->click();
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
    if (ui->log_matrix_checkbox->isChecked()) {
        qDebug() << "++Matrix Dim: " << input_matrix.size() << " " << input_matrix[0].size();
        //fill_matrix();
        for (int i = 0; i < input_matrix.size(); i++){
            for (int j = 0; j < input_matrix[i].size(); j++) {
                std::cout << "[" << input_matrix[i][j]<< "]";
            }
            std::cout << "" << std::endl;
        }
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
    int canvas_height = ui->canvas_label->height();
    int canvas_width = ui->canvas_label->width();
    factor_width = canvas_width / input_matrix.size();
    factor_height = canvas_height / input_matrix[0].size();

    if (ui->log_matrix_checkbox->isChecked()) {
        status_update(__FUNCTION__);
        status_update("Painting factor x: " + std::to_string(factor_width)+ " y: " + std::to_string(factor_height));

        //Rendering might be faulty ue to rounding error
        if ((factor_width * input_matrix.size() / canvas_width) == true) {
            status_update("Rounding Error eminent");
        }
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

void MainWindow::matrix_updater(std::pair<int, int> item)
{
    tile_map.insert(item);
    for (auto elem : tile_map) {
        //qDebug() << elem << "<---- New Tile added";
        repaint_canvas();
    }
    tile_map.clear();

    //propagate everything
    qDebug() << net.propagate(input_matrix);
}

void MainWindow::assign_clicked_tile(std::pair<int, int> item)
{
    int tile_width = item.first;
    int tile_height = item.second;
    int tile_n_max = std::floor(ui->canvas_label->width() / factor_width);
    int tile_m_max = std::floor(ui->canvas_label->height() / factor_height);

    //Middle
    if (input_matrix[tile_width][tile_height] < 1.0) {
        input_matrix[tile_width][tile_height] = 1.0;
        //North
        if (tile_height > 0 && input_matrix[tile_width][tile_height-1] < 1.0) {
            input_matrix[tile_width][tile_height-1] = input_matrix[tile_width][tile_height-1] + 0.25;
        }

        //East
        if (tile_width < tile_n_max-1 && input_matrix[tile_width+1][tile_height] < 1.0) {
            input_matrix[tile_width+1][tile_height] = input_matrix[tile_width+1][tile_height] + 0.25;
        }

        //South
        if (tile_height < tile_m_max-1 && input_matrix[tile_width][tile_height+1] < 1.0) {
            input_matrix[tile_width][tile_height+1] = input_matrix[tile_width][tile_height+1] + 0.25;
        }

        //West
        if (tile_width > 0 && input_matrix[tile_width-1][tile_height] < 1.0) {
            input_matrix[tile_width-1][tile_height] = input_matrix[tile_width-1][tile_height] + 0.25;
        }

        matrix_updater(std::make_pair(tile_height, tile_width));

        if (ui->log_matrix_checkbox->isChecked()) {
            print_matrix();
        }
        //repaint_canvas();
    }
}


void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    //thanks grg
    //TO DO
    //DONE: Change canvas_label dimensions so that factor * fixed box (e.g. 10px) doesnt produce a rounding error or ugly mishap
    //DONE: Check if each tile has value <= 1.0 so that no tile has a value > 1.0
    //DONE: Check if a tile with value = 1.0 is allowed to be clicked again, so that the adjacent neighbours are in creased
    if (ev->button() == Qt::LeftButton) {
        QString x_str = QString::number(ev->x());
        QString y_str = QString::number(ev->y());
        int x = x_str.toInt();
        int y = y_str.toInt();
        if (ui->canvas_label->geometry().contains(ev->pos())) {
            int tile_width = std::floor(x / factor_width);
            int tile_height = std::floor(y / factor_height);
            int tile_n_max = std::floor(ui->canvas_label->width() / factor_width);
            int tile_m_max = std::floor(ui->canvas_label->height() / factor_height);

            if (ui->log_matrix_checkbox->isChecked()) {
                std::string temp = __FUNCTION__;
                temp = temp.append("Matrix: [" + std::to_string(tile_height) + "][" + std::to_string(tile_width) + "]");
                status_update(temp);
            }
            //assign clicked tile to matrix
            assign_clicked_tile(std::make_pair(tile_width, tile_height));
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    QString x_str = QString::number(ev->x());
    QString y_str = QString::number(ev->y());
    int x = x_str.toInt();
    int y = y_str.toInt();
    if (ui->canvas_label->geometry().contains(ev->pos())) {
        int tile_width = std::floor(x / factor_width);
        int tile_height = std::floor(y / factor_height);
        int tile_n_max = std::floor(ui->canvas_label->width() / factor_width);
        int tile_m_max = std::floor(ui->canvas_label->height() / factor_height);

        if (ui->log_matrix_checkbox->isChecked()) {
            std::string temp = __FUNCTION__;
            temp = temp.append("Matrix: [" + std::to_string(tile_height) + "][" + std::to_string(tile_width) + "]");
            status_update(temp);
        }

        //assign clicked tile to matrix
        assign_clicked_tile(std::make_pair(tile_width, tile_height));
    }
}


void MainWindow::on_height_valueChanged(int arg1)
{
    //TO DO repaint of canvas label
    //TO DO: Find the sizing error.
    if (init > 0) {
        std::string temp = __FUNCTION__;
        input_matrix.clear(); //essentially needed. if not cleared, major fuck up
        this->input_matrix.resize(ui->height->value(), std::vector<float> (ui->width->value()));
        if (ui->log_matrix_checkbox->isChecked()) {
            this->print_matrix();
        }
        repaint_canvas();
        ui->canvas_label->resize(factor_width * input_matrix.size(), factor_height * input_matrix[0].size());
    }
}

void MainWindow::on_width_valueChanged(int arg1)
{
    //TO DO repaint of canvas label
    //TO DO: Find the sizing error.
    if (init > 0) {
        std::string temp = __FUNCTION__;
        input_matrix.clear(); //essentially needed. if not cleared, major fuck up
        this->input_matrix.resize(ui->height->value(), std::vector<float> (ui->width->value()));
        if (ui->log_matrix_checkbox->isChecked()) {
            this->print_matrix();
        }
        repaint_canvas();
        ui->canvas_label->resize(factor_width * input_matrix.size(), factor_height * input_matrix[0].size());
    }
}

void MainWindow::on_reset_button_clicked()
{
    init = 0;
    ui->status_text->clear();
    this->status_update(__TIMESTAMP__);
    this->status_update(__FUNCTION__);
    ui->height->setValue(28);
    ui->width->setValue(28);
    init = 1;
    this->on_height_valueChanged(0);
    this->on_width_valueChanged(0);
}

void MainWindow::on_train_button_clicked()
{
    // Open file dialog
    // train -> filepath
    // training in progress . threading
    // training done
    // net.train_with_file(str filepath der bilder, str filepath der labels, int anzahl= variabel hier)
    QString filter = "Any File (*)";
    QString file_name_images = QFileDialog::getOpenFileName(this, "Open file", "../", filter);
    QString file_name_labels = QFileDialog::getOpenFileName(this, "Open file", "../", filter);
    int training_images_amount = QInputDialog::getInt(this, "User Action", "Set amount of training images", 60000, 1, 60000, 100);
    if (!file_name_images.isEmpty() && !file_name_labels.isEmpty() && training_images_amount > 0) {
        net.test_with_file(file_name_images.toUtf8().constData(), file_name_labels.toUtf8().constData(), training_images_amount);
    }
}

void MainWindow::on_test_batch_button_clicked()
{
    //TO DO: How many Pictures should be put through trained net
    //TO DO: Discuss return value
    QString filter = "Any File (*)";
    QString file_name_images = QFileDialog::getOpenFileName(this, "Open file", "../", filter);
    QString file_name_labels = QFileDialog::getOpenFileName(this, "Open file", "../", filter);
    int training_images_amount = QInputDialog::getInt(this, "User Action", "Set amount of training images", 60000, 1, 60000, 100);
    if (!file_name_images.isEmpty() && !file_name_labels.isEmpty() && training_images_amount > 0) {
        net.test_with_file(file_name_images.toUtf8().constData(), file_name_labels.toUtf8().constData(), training_images_amount);
    }
}

void MainWindow::on_test_single_button_clicked()
{
    //TODO: Make sure to check input dim
    //take image from canvas
    //test input dim
    //give values as
    //net.propagte(meine coole matrix)
}
