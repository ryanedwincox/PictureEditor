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
    void copySlot();
    void blurSlot();

private:
    //Ui::MainWindow *ui;

    int filterInit();

    // GUI variables
    QPushButton *copy;
    QPushButton *blur;
    QVBoxLayout *buttonLayoutV;
    QLabel *imgLabel;
    QHBoxLayout *mainLayoutH;

    // OpenCL variables
    cv::Mat image;
    cv::Mat newImage;
    size_t imageWidth;
    size_t imageHeight;
    const char* copyImageClPath;
    const char* lowPassClPath;
    unsigned char* newDataPointer;
    cl_int lpfMaskSize;
    filter f1;
};

#endif // MAINWINDOW_H
