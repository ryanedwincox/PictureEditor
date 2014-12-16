#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    window = new QWidget(this);
    this->setCentralWidget(window);

    QPushButton *button1 = new QPushButton("One");
    QPushButton *button2 = new QPushButton("Two");

    QVBoxLayout *buttonLayoutV = new QVBoxLayout();
    buttonLayoutV->addWidget(button1);
    buttonLayoutV->addWidget(button2);

    QPixmap img;
    img.load("/home/bluhaptics1/Documents/PictureEditor/images/Lenna.png");

    QLabel *imgLabel = new QLabel;
    imgLabel->setPixmap(img);

    QHBoxLayout *mainLayoutH = new QHBoxLayout();
    mainLayoutH->addLayout(buttonLayoutV);
    mainLayoutH->addWidget(imgLabel);

    window->setLayout(mainLayoutH);

}

MainWindow::~MainWindow()
{

}
