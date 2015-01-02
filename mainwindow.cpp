#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    window = new QWidget(this);
    this->setCentralWidget(window);

    // Create buttons
    copy = new QPushButton("Copy");
    blur = new QPushButton("Blur");

    // Connect button signal to appropriate slot
    connect(copy, SIGNAL(clicked()), this, SLOT(copySlot()));
    connect(blur, SIGNAL(clicked()), this, SLOT(blurSlot()));

    // Create button layout
    buttonLayoutV = new QVBoxLayout();
    buttonLayoutV->addWidget(copy);
    buttonLayoutV->addWidget(blur);

    filterInit();

    // Create image
    QImage im(newDataPointer, imageWidth, imageHeight, QImage::Format_RGB888);
    QPixmap img = QPixmap::fromImage(im);

    imgLabel = new QLabel;
    imgLabel->setPixmap(img);

    mainLayoutH = new QHBoxLayout();
    mainLayoutH->addLayout(buttonLayoutV);
    mainLayoutH->addWidget(imgLabel);

    window->setLayout(mainLayoutH);
}

void MainWindow::copySlot()
{
    std::cout << "Copy!" << std::endl;
    f1.buildProgram(copyImageClPath, 0);
    f1.setImage(newImage);
    f1.runProgram();
    // newDataPointer is used to display image in gui
    newDataPointer = (unsigned char*) f1.readOutput();
    // newImage is passed into the next filter
    newImage = cv::Mat(cv::Size(imageWidth,imageHeight), CV_8UC3, newDataPointer);

    // update image
    QImage im(newDataPointer, imageWidth, imageHeight, QImage::Format_RGB888);
    QPixmap img = QPixmap::fromImage(im);
    imgLabel->setPixmap(img);
}

void MainWindow::blurSlot()
{
    std::cout << "Blur!" << std::endl;
    f1.buildProgram(lowPassClPath, lpfMaskSize);
    f1.setImage(newImage);
    f1.runProgram();
    // newDataPointer is used to display image in gui
    newDataPointer = (unsigned char*) f1.readOutput();
    // newImage is passed into the next filter
    newImage = cv::Mat(cv::Size(imageWidth,imageHeight), CV_8UC3, newDataPointer);

    // update image
    QImage im(newDataPointer, imageWidth, imageHeight, QImage::Format_RGB888);
    QPixmap img = QPixmap::fromImage(im);
    imgLabel->setPixmap(img);
}

int MainWindow::filterInit()
{
    // Load image
    image = cv::imread("/home/bluhaptics1/Documents/ImageManipulator/images/Lenna.png", CV_LOAD_IMAGE_COLOR);

    // converts image to RGB which qt understands from openCVs default BGR
    cvtColor(image, image, CV_BGR2RGB);

    // Check for invalid input
    if(! image.data )
    {
        std::cout <<  "Could not open or find the image" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Image loaded" << std::endl;
    }

    imageWidth = image.cols;
    imageHeight = image.rows;
    std::cout << "image width: " << imageWidth << " image height: " << imageHeight << std::endl;

    copyImageClPath = "/home/bluhaptics1/Documents/ImageManipulator/cl/copy_image.cl";
    lowPassClPath = "/home/bluhaptics1/Documents/ImageManipulator/cl/low_pass.cl";
    lpfMaskSize = 5;

    // emit the copy signal to start with a normal image and set newImage
    //emit SIGNAL(clicked());

    f1.buildProgram(copyImageClPath, 0);
    f1.setImage(image);
    f1.runProgram();
    // newDataPointer is used to display image in gui
    newDataPointer = (unsigned char*) f1.readOutput();
    // newImage is passed into the next filter
    newImage = cv::Mat(cv::Size(imageWidth,imageHeight), CV_8UC3, newDataPointer);

//    // Display images
//    std::cout << "Display images" << std::endl;

//    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE); // Create a window for display.
//    cv::imshow("Original Image", f1.getInputImage());           // Show our image inside it.

//    cv::namedWindow("Blured Image", cv::WINDOW_AUTOSIZE); // Create a window for display.
//    cv::imshow("Blured Image", newImage);                 // Show our image inside it.

    return 1;
}

MainWindow::~MainWindow()
{
}

