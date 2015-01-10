#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    window = new QWidget(this);
    this->setCentralWidget(window);

    // Create buttons
    copy = new QPushButton("Reset");
    blur = new QPushButton("Blur");

    // Create button layout
    buttonLayoutV = new QVBoxLayout();
    buttonLayoutV->addWidget(copy);
    buttonLayoutV->addWidget(blur);

    // Initialize image widget
    QPixmap img;

    imgLabel = new QLabel;
    imgLabel->setPixmap(img);

    mainLayoutH = new QHBoxLayout();
    mainLayoutH->addLayout(buttonLayoutV);
    mainLayoutH->addWidget(imgLabel);

    window->setLayout(mainLayoutH);

    // Start image processing thread
    processThread = new QThread;
    FilterThread* worker = new FilterThread();
    worker->moveToThread(processThread);
    // Connect init signal
    connect(processThread, SIGNAL(started()), worker, SLOT(filterInit()));
    // Connect button signal to appropriate slot
    connect(copy, SIGNAL(clicked()), worker, SLOT(copySlot()));
    connect(blur, SIGNAL(clicked()), worker, SLOT(blurSlot()));

    // connect update image signal
    connect(worker, SIGNAL(updateImageSignal(unsigned char*, int, int)), this, SLOT(updateImageSlot(unsigned char*, int, int)));

//    // Connect finish signal to close down thread
//    connect(worker, SIGNAL(finished()), processThread, SLOT(quit()));
//    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
//    connect(processThread, SIGNAL(finished()), processThread, SLOT(deleteLater()));
    processThread->start();

}

void MainWindow::updateImageSlot(unsigned char* newDataPointer, int imageWidth, int imageHeight)
{
    // update image
    QImage im(newDataPointer, imageWidth, imageHeight, QImage::Format_RGB888);
    QPixmap img = QPixmap::fromImage(im);
    imgLabel->setPixmap(img);
}

MainWindow::~MainWindow()
{
    processThread->quit();
    processThread->wait();
}

