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

    void on_canvas_label_linkActivated(const QString &link);

private:
    int init;
    int factor_width;
    int factor_height;
    std::vector< std::vector<float> > input_matrix;

    Ui::MainWindow *ui;
    void status_update(std::string status);
    void print_matrix();
    void fill_matrix();
    void repaint_canvas();
    void pixel_painter();

protected:
    void mousePressEvent(QMouseEvent* ev);
};

#endif // MAINWINDOW_H
