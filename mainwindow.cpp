#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    window = new QWidget(this);
    this->setCentralWidget(window);

    QPushButton *copy = new QPushButton("Copy");
    QPushButton *blur = new QPushButton("Blur");

    QVBoxLayout *buttonLayoutV = new QVBoxLayout();
    buttonLayoutV->addWidget(copy);
    buttonLayoutV->addWidget(blur);

    QPixmap img;
    img.load("/home/bluhaptics1/Documents/PictureEditor/images/Lenna.png");

    QLabel *imgLabel = new QLabel;
    imgLabel->setPixmap(img);

    QHBoxLayout *mainLayoutH = new QHBoxLayout();
    mainLayoutH->addLayout(buttonLayoutV);
    mainLayoutH->addWidget(imgLabel);

    window->setLayout(mainLayoutH);
}

void MainWindow::copy()
{

}

void MainWindow::blur()
{

}

int MainWindow::filterInit()
{
    // Load image
    image = cv::imread("/home/bluhaptics1/Documents/ImageManipulator/images/Lenna.png", CV_LOAD_IMAGE_COLOR);

    // Check for invalid input
    if(! image.data )
    {
        std::cout <<  "Could not open or find the image" << std::endl;
        return -1;
    }

    size_t imageWidth = image.cols;
    size_t imageHeight = image.rows;

    // Will store filter results
    unsigned char* newDataPointer;

    const char* copyImageClPath = "/home/bluhaptics1/Documents/ImageManipulator/cl/copy_image.cl";
    const char* lowPassClPath = "/home/bluhaptics1/Documents/ImageManipulator/cl/low_pass.cl";
    cl_int lpfMaskSize = 5;

    // Create filter object
    filter f1;
}

MainWindow::~MainWindow()
{
}
