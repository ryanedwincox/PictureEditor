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
    FilterThread(QLabel*);
    ~FilterThread();

public slots:
    void filterInit();
    void copySlot();
    void blurSlot();

signals:
    void start();

private:
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
    QLabel *imgLabel;

};

#endif // FILTERTHREAD_H
