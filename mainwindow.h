#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <filter.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget *window;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void copy();
    void blur();

private:
    Ui::MainWindow *ui;

    int filterInit();

    cv::Mat image;
    size_t imageWidth;
    size_t imageHeight;
    const char* copyImageClPath;
    const char* lowPassClPath;
    cl_int lpfMaskSize;
    filter f1;
};

#endif // MAINWINDOW_H
