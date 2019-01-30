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
#include <QStaticText>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

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
    //ui->canvas_label->setFrameShape(QFrame::Box);
    ui->status_text->setReadOnly(true);

    //value definitions
    ui->height->setMinimum(1);
    ui->height->setValue(28);
    ui->height->setMaximum(30);

    ui->width->setMinimum(1);
    ui->width->setValue(28);
    ui->width->setMaximum(30);



    status_update("Painting factor x: " + std::to_string(factor_width)+ " y: " + std::to_string(factor_height));
    factor_width = 10;
    factor_height = factor_width;

    ui->log_matrix_checkbox->setCheckState(Qt::CheckState::Unchecked);

    //Important to keep at end of constructor.
    //Variable to show that all parts are loaded an programm is operational
    status_update("Init done. init = 1. Program nominal");
    emit ui->reset_button->click();
    init = 1;
    //emit ui->reset_button->click();

    net.eta = 0.3;
    net.toggle_adaptive_learning = true;
    net.setActivationFunction(&logistic, &logistic_derived);

    canvas_height = factor_height * input_matrix[0].size();
    canvas_width = factor_width *input_matrix.size();
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
                std::cout << "[" << input_matrix[j][i]<< "]";
            }
            std::cout << "" << std::endl;
        }
    }

}

void MainWindow::fill_matrix()
{
    //testing purposes. Could be filled with random noise
    for (int i = 0; i < this->input_matrix.size(); i++){
        for (int j = 0; j < this->input_matrix[i].size(); j++) {
            input_matrix[i][j] = 0.0;
        }
    }
}

void MainWindow::repaint_canvas()
{
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
    //qDebug() << net.propagate(input_matrix);
    visualizate();
}

void MainWindow::assign_clicked_tile(std::pair<int, int> item)
{
    int tile_x = item.first;
    int tile_y = item.second;
    int tile_n_max = input_matrix.size();
    int tile_m_max = input_matrix[0].size();

    //Middle
    if (input_matrix[tile_x][tile_y] < 1.0) {
        input_matrix[tile_x][tile_y] = 1.0;
        //North
        if (tile_y > 0 && input_matrix[tile_x][tile_y-1] < 1.0) {
            input_matrix[tile_x][tile_y-1] = input_matrix[tile_x][tile_y-1] + 0.25;
        }

        //East
        if (tile_x < tile_n_max-1 && input_matrix[tile_x+1][tile_y] < 1.0) {
            input_matrix[tile_x+1][tile_y] = input_matrix[tile_x+1][tile_y] + 0.25;
        }

        //South
        if (tile_y < tile_m_max-1 && input_matrix[tile_x][tile_y+1] < 1.0) {
            input_matrix[tile_x][tile_y+1] = input_matrix[tile_x][tile_y+1] + 0.25;
        }

        //West
        if (tile_x > 0 && input_matrix[tile_x-1][tile_y] < 1.0) {
            input_matrix[tile_x-1][tile_y] = input_matrix[tile_x-1][tile_y] + 0.25;
        }

        matrix_updater(std::make_pair(tile_x, tile_y));

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
            int tile_x = std::floor(x / factor_width);
            int tile_y = std::floor(y / factor_height);
            int tile_n_max = input_matrix[0].size();
            int tile_m_max = input_matrix.size();

            if (ui->log_matrix_checkbox->isChecked()) {
                std::string temp = __FUNCTION__;
                temp = temp.append("Matrix: [" + std::to_string(tile_x) + "][" + std::to_string(tile_y) + "]");
                status_update(temp);
            }
            //assign clicked tile to matrix
            assign_clicked_tile(std::make_pair(tile_x, tile_y));
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
        int tile_x = std::floor(x / factor_width);
        int tile_y = std::floor(y / factor_height);
        int tile_n_max = input_matrix[0].size();
        int tile_m_max = input_matrix.size();

        if (ui->log_matrix_checkbox->isChecked()) {
            std::string temp = __FUNCTION__;
            temp = temp.append("Matrix: [" + std::to_string(tile_y) + "][" + std::to_string(tile_x) + "]");
            status_update(temp);
        }

        //assign clicked tile to matrix
        assign_clicked_tile(std::make_pair(tile_x, tile_y));
    }
}


void MainWindow::on_height_valueChanged(int arg1)
{
    //    //TO DO repaint of canvas label
    //    //TO DO: Find the sizing error.
    //    if (init > 0) {
    //        std::string temp = __FUNCTION__;
    //        input_matrix.clear(); //essentially needed. if not cleared, major fuck up
    //        this->input_matrix.resize(ui->height->value(), std::vector<float> (ui->width->value(),0));
    //        canvas_height = factor_height * input_matrix[0].size();
    //        canvas_width = factor_width *input_matrix.size();
    //        this->fill_matrix();
    //        if (ui->log_matrix_checkbox->isChecked()) {
    //            this->print_matrix();
    //        }

    //        repaint_canvas();
    //        ui->canvas_label->resize(canvas_width, canvas_height);
    //    }
    this->on_width_valueChanged(0);
}

void MainWindow::on_width_valueChanged(int arg1)
{
    //TO DO repaint of canvas label
    //TO DO: Find the sizing error.
    if (init > 0) {
        std::string temp = __FUNCTION__;
        input_matrix.clear(); //essentially needed. if not cleared, major fuck up
        this->input_matrix.resize(ui->height->value(), std::vector<float> (ui->width->value(),0));
        canvas_height = factor_height * input_matrix[0].size();
        canvas_width = factor_width *input_matrix.size();
        this->fill_matrix();
        if (ui->log_matrix_checkbox->isChecked()) {
            this->print_matrix();
        }

        repaint_canvas();
        ui->canvas_label->resize(canvas_width, canvas_height);
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
    ui->output_label->setText("");
}

void MainWindow::on_train_button_clicked()
{
    QString filter = "Any File (*)";
    QString file_name_images = QFileDialog::getOpenFileName(this, "Choose Image File", "../", filter);
    QString file_name_labels = QFileDialog::getOpenFileName(this, "Choose Label File", "../", filter);
    int training_images_amount = QInputDialog::getInt(this, "User Action", "Set amount of training images", 60000, 1, 60000, 100);
    if (!file_name_images.isEmpty() && !file_name_labels.isEmpty() && training_images_amount > 0) {
        //net.train_with_file(file_name_images.toUtf8().constData(), file_name_labels.toUtf8().constData(), training_images_amount);
        string img = file_name_images.toUtf8().constData();
        string lbl = file_name_labels.toUtf8().constData();
        int amt = training_images_amount;
        QFuture<void> future = QtConcurrent::run(&this->net, &NeuralNet::train_with_file, img, lbl, amt);
        std::string temp = __FUNCTION__;
        temp.append("Images: ");
        temp.append(file_name_images.toUtf8().constData());
        temp.append("Labels: ");
        temp.append(file_name_labels.toUtf8().constData());
        temp.append("Amount: ");
        temp.append(std::to_string(training_images_amount));
        status_update(temp);
    }
}

void MainWindow::on_test_batch_button_clicked()
{
    //TO DO: Discuss return value
    //TO DO: Check for file validity
    QString filter = "Any File (*)";
    QString file_name_images = QFileDialog::getOpenFileName(this, "Choose Image File", "../", filter);
    QString file_name_labels = QFileDialog::getOpenFileName(this, "Choose Label File", "../", filter);
    auto training_images_amount = QInputDialog::getInt(this, "User Action", "Set amount of training images", 60000, 1, 60000, 100);
    if (!file_name_images.isEmpty() && !file_name_labels.isEmpty() && training_images_amount > 0) {
        //        net.test_with_file(file_name_images.toUtf8().constData(), file_name_labels.toUtf8().constData(), training_images_amount);
        string img = file_name_images.toUtf8().constData();
        string lbl = file_name_labels.toUtf8().constData();
        int amt = training_images_amount;
        QFuture<float> future = QtConcurrent::run(&this->net, &NeuralNet::test_with_file, img, lbl, amt);
        std::string temp = __FUNCTION__;
        temp.append("Images: ");
        temp.append(file_name_images.toUtf8().constData());
        temp.append("Labels: ");
        temp.append(file_name_labels.toUtf8().constData());
        temp.append("Amount: ");
        temp.append(std::to_string(training_images_amount));
        status_update(temp);
        QMessageBox msgBox;
        if (!future.isFinished()) {
            qDebug() << future.result();
        msgBox.setText("Succes Rate 83%" + QString::number(future.result()));
        msgBox.exec();
        }
        qDebug() << future.result();
   }
}

void MainWindow::on_test_single_button_clicked()
{
    net.propagate(this->input_matrix);
}

void MainWindow::visualizate()
{
    //propagate and visualisate
    std::vector<float> result = net.propagate(this->input_matrix);

    QPixmap pixmap(ui->output_canvas_label->width(), ui->output_canvas_label->height());
    pixmap.fill(QColor("transparent"));
    QPainter painter (&pixmap);

    int circle_factor = 15; // diameter adapt to the value of output vector
    int vertical_factor = 40;
    int vertical_middle =  ui->output_canvas_label->width() / 2; //ui->canvas_label->pos().x() +
    int static_x_num = +27;
    int static_y_num = +5;
    int r = 209;
    int g = 204;
    int b = 255;
    int r_factor = r / 10;

    for(int i = 0; i < 10; i++) {
        //No pen for no outline of ellipse
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(r-r_factor,g-15*i,b-15*i,255));
        painter.drawEllipse(QPointF(vertical_middle, vertical_factor*(i+1)), circle_factor * result[i] + 5, circle_factor * result[i] + 5);
        //Reset Pen
        painter.setPen(Qt::SolidLine);
        //Assign Number
        painter.drawStaticText(QPointF(vertical_middle-static_x_num, vertical_factor*(i+1)-static_y_num), QStaticText(QString::number(i)));
        //Assign Value and super easy rounding. Hnngh
        float res = (result[i] * 10000);
        res = floor (res);
        res = res/1000;
        painter.drawStaticText(QPointF(vertical_middle-4, vertical_factor*(i+1)+3), QStaticText(QString::number(res)));
    }
    ui->output_canvas_label->setPixmap(pixmap);

    auto max_val = std::max_element(std::begin(result), std::end(result));
    //qDebug() << *max_val;
    int classifier_output = std::distance(std::begin(result), max_val);

    //Label for output as int
    ui->output_label->clear();
    QFont f( "Arial", 50, QFont::Bold);
    ui->output_label->setFont(f);
    ui->output_label->setAlignment(Qt::AlignCenter);
    ui->output_label->setText(QString::number(classifier_output));
}
