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

    void on_test_button_clicked();

    void on_reset_button_clicked();

private:
    int init;
    int factor_width;
    int factor_height;
    int is_trained;
    std::vector< std::vector<float> > input_matrix;
    std::vector< std::vector<float> > scaled_input_matrix;

    Ui::MainWindow *ui;
    void status_update(std::string status);
    void print_matrix();
    void fill_matrix();
    void repaint_canvas();
    std::vector< std::vector<float> > matrix_multiplyer();

protected:
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
};

#endif // MAINWINDOW_H
