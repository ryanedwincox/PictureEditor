#include "filterthread.h"

FilterThread::FilterThread(QLabel* imageLabel)
{
    imgLabel = imageLabel;
}

void FilterThread::filterInit()
{
    // Load image
    image = cv::imread("/home/bluhaptics1/Documents/ImageManipulator/images/Lenna.png", CV_LOAD_IMAGE_COLOR);

    // converts image to RGB which qt understands from openCVs default BGR
    cvtColor(image, image, CV_BGR2RGB);

    // Check for invalid input
    if(! image.data )
    {
        std::cout <<  "Could not open or find the image" << std::endl;
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
    connect(this, SIGNAL(start()), this, SLOT(copySlot()));
    emit start();

//    // Display images using OpenCV
//    std::cout << "Display images" << std::endl;

//    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE); // Create a window for display.
//    cv::imshow("Original Image", f1.getInputImage());           // Show our image inside it.

//    cv::namedWindow("Blured Image", cv::WINDOW_AUTOSIZE); // Create a window for display.
//    cv::imshow("Blured Image", newImage);                 // Show our image inside it.

}

// Resets to the original image
void FilterThread::copySlot()
{
    f1.buildProgram(copyImageClPath, 0);
    f1.setImage(image);
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

// Blurs image
void FilterThread::blurSlot()
{
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

FilterThread::~FilterThread()
{
}