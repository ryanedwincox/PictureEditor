#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QThread>

#include "filter.h"
#include "filterthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread filterThread;
    QWidget *window;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // GUI variables
    QPushButton *copy;
    QPushButton *blur;
    QVBoxLayout *buttonLayoutV;
    QLabel *imgLabel;
    QHBoxLayout *mainLayoutH;

    QThread* processThread;

signals:
    void initSignal();
    void copySignal();
    void blurSignal();

};

#endif // MAINWINDOW_H
