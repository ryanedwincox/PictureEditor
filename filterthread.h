#ifndef FILTERTHREAD_H
#define FILTERTHREAD_H

#include <QThread>
#include <QLabel>
#include <QFrame>

#include "filter.h"

class FilterThread : public QObject
{
    Q_OBJECT
    QThread processThread;

public:
    FilterThread();
    ~FilterThread();

public slots:
    void filterInit();
    void copySlot();
    void blurSlot();

signals:
    void start();
    void updateImageSignal(unsigned char* newDataPointer, int imageWidth, int imageHeight);

private:
    // OpenCL variables
    cv::Mat image;
    cv::Mat newImage;
    int imageWidth;
    int imageHeight;
    const char* copyImageClPath;
    const char* lowPassClPath;
    unsigned char* newDataPointer;
    cl_int lpfMaskSize;
    filter f1;

};

#endif // FILTERTHREAD_H
