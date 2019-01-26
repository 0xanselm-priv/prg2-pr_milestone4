#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_height_valueChanged(int arg1);

    void on_width_valueChanged(int arg1);

private:
    int init;
    std::vector< std::vector<float> > input_matrix;

    Ui::MainWindow *ui;
    void status_update(std::string status);
    void print_matrix();
    void fill_matrix();

};

#endif // MAINWINDOW_H
