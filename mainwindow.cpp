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
    FilterThread* worker = new FilterThread(imgLabel);
    worker->moveToThread(processThread);
    // Connect init signal
    connect(processThread, SIGNAL(started()), worker, SLOT(filterInit()));
    // Connect button signal to appropriate slot
    connect(copy, SIGNAL(clicked()), worker, SLOT(copySlot()));
    connect(blur, SIGNAL(clicked()), worker, SLOT(blurSlot()));
//    // Connect finish signal to close down thread
//    connect(worker, SIGNAL(finished()), processThread, SLOT(quit()));
//    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
//    connect(processThread, SIGNAL(finished()), processThread, SLOT(deleteLater()));
    processThread->start();

}

MainWindow::~MainWindow()
{
    processThread->quit();
    processThread->wait();
}

