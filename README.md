PictureEditor
=============
OVERVIEW

This program utilizes the parallel processing power of OpenCL to do efficient image processing.  A QT based user interface makes it easy and intuitive to operate.

FEATURES

The OpenCL framework allows for low level access to the GPU which can be used to implement efficient graphics processing.  This program uses many cores of the GPU to perform operations on each pixel in parallel.  It has also been optimized so that each kernel in a work group shares local memory to reduce memory reads from the global memory.

The graphical user interface implemented in QT allows the user to manipulate images.  The program currently supports a low pass filter and resetting the original image.  The processing has been moved to a separate thread so the user interface does not have to wait for the processing to finish.


