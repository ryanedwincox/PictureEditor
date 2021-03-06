#include "filter.h"

using namespace std;

// Constructor
filter::filter()
{
    cl_int init_status;
    cl_platform_id *platforms;

    cl_uint numPlatforms = 0;

    // Query for the number of recongnized platforms
    init_status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if(init_status != CL_SUCCESS) {
        cout << "GPU init: clGetPlatformIDs failed" << endl;
    }

    // Make sure some platforms were found
    if(numPlatforms == 0) {
        cout << "GPU init: No platforms detected" << endl;
    }

    // Allocate enough space for each platform
    platforms = (cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));
    if(platforms == NULL) {
        cout << "not enough space for platforms" << endl;
    }

    // Fill in platforms
    init_status = clGetPlatformIDs(numPlatforms, platforms, NULL);
    if(init_status != CL_SUCCESS) {
        cout << "GPU init: clGetPlatformIDs failed" << endl;
    }

    // Print out some basic information about each platform
    cout << numPlatforms << " platforms detected" << endl;
    for(unsigned int i = 0; i < numPlatforms; i++) {
        char buf[100];
        cout << "Platform: " << i << endl;
        init_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR,
                                        sizeof(buf), buf, NULL);
        cout << "\tVendor: " << buf << endl;
        init_status |= clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME,
                                         sizeof(buf), buf, NULL);
        cout << "\tName: " <<  buf << endl;

        if(init_status != CL_SUCCESS) {
            cout << "GPU init: clGetPlatformInfo failed" << endl;
        }
    }

    // get device count
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &deviceCount);
    cout << "device count error: " << err << endl;

    // get devices
    devices = new cl_device_id[deviceCount];
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, deviceCount, devices, NULL);
    cout << "device ID error: " << err << endl;

    // create a single context for all devices
    context = clCreateContext(NULL, deviceCount, devices, NULL, NULL, &err);
    cout << "context error: " << err << endl;
}

// Builds OpenCl program
void filter::buildProgram(const char* clPath, cl_int maskSize)
{
    this->clPath = clPath;
    this->maskSize = maskSize;

    // get size of kernel source
    const char* kernelSource = clPath;
    programHandle = fopen(kernelSource, "r");
    if (programHandle == NULL)
    {
        cout << "Kernel source not found" << endl;
    }
    fseek(programHandle, 0, SEEK_END);
    programSize = ftell(programHandle);
    rewind(programHandle);

    // read kernel source into buffer
    programBuffer = (char*) malloc(programSize + 1);
    programBuffer[programSize] = '\0';
    fread(programBuffer, sizeof(char), programSize, programHandle);
    fclose(programHandle);

    // create program from buffer
    program = clCreateProgramWithSource(context, 1,
            (const char**) &programBuffer, &programSize, NULL);

    // build program
    const char* buildOptions = "";
    err = clBuildProgram(program, deviceCount, devices, buildOptions, NULL, NULL);
    cout << "program error: " << err << "\n";

    // create the log string and show it to the user. Then quit
    char buildLog[MAX_LOG_SIZE];
    err = clGetProgramBuildInfo(program,
                          devices[0],
                          CL_PROGRAM_BUILD_LOG,
                          MAX_LOG_SIZE,
                          &buildLog,
                          NULL);
    printf("**BUILD LOG**\n%s",buildLog);
    cout << "clGetProgramBuildInfo error: " << err << "\n";

    //create queue to which we will push commands for the device
    queue = clCreateCommandQueue(context,devices[0],0,&err);
    cout << "command queue error: " << err << "\n";

    // build kernel
    kernel = clCreateKernel(program, "filter_kernel", &err);
    cout << "cl_kernel error: " << err << "\n";
}

// Stores image to process
// Creates buffers to store image on device
void filter::setImage(cv::Mat img)
{
    cout << "Creating image buffers" << endl;
    image = img;

    imageWidth = image.cols;
    imageHeight = image.rows;
    imageSize = imageHeight * imageWidth;

    // Create an OpenCL buffer for the image
    clImage = clCreateBuffer(context,
                             CL_MEM_READ_ONLY,
                             imageSize * 3 * sizeof(char),
                             NULL,
                             &err);
    cout << "clImage Buffer error: " << err << "\n";
    //CL_SUCCESS
    // Create an OpenCL buffer for the result
    clResult = clCreateBuffer(context,
                              CL_MEM_WRITE_ONLY,
                              imageSize * 4 * sizeof(char),
                              NULL,
                              &err);
    cout << "clResult Buffer error: " << err << "\n";

    // Create an extra buffer for debugging
    clDebug = clCreateBuffer(context,
                             CL_MEM_WRITE_ONLY,
                             DEBUG_BUFFER_SIZE,
                             NULL,
                             &err);
    cout << "clDebug Buffer error: " << err << "\n";

    // load image to device
    err = clEnqueueWriteBuffer(queue,
                               clImage,
                               CL_TRUE,
                               0,
                               imageSize * 3 * sizeof(char),
                               (void*) &image.data[0],
                               0,
                               NULL,
                               NULL);
    cout << "enqueueWriteImage error: " << err << "\n";
}

// Excecutes the kernel
void filter::runProgram()
{
    // This loop slows the processing to show multithreading.
    // The user interface still responds while processing is underway
    // allowing the user to start multiple processes before the first is finished
//    for (int i = 0; i < 100000; i++)
//    {
//        cout << i << endl;
//    }

    std::cout << "runProgram" << std::endl;

    // set kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&clImage);
    cout << "kernel arg 0 error: " << err << "\n";
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&clResult);
    cout << "kernel arg 1 error: " << err << "\n";
    err = clSetKernelArg(kernel, 2, sizeof(cl_int), &imageWidth);
    cout << "kernel arg 2 error: " << err << "\n";
    err = clSetKernelArg(kernel, 3, sizeof(cl_int), &imageHeight);
    cout << "kernel arg 3 error: " << err << "\n";
    err = clSetKernelArg(kernel, 4, sizeof(cl_int), &maskSize);
    cout << "kernel arg 4 error: " << err << "\n";
    err = clSetKernelArg(kernel, 5, sizeof(cl_mem), &clDebug);
    cout << "kernel arg 5 error: " << err << "\n";

    // Set local and global workgroup sizes
    size_t localws[2] = {16,16};
    size_t globalws[2] = {imageWidth, imageHeight};

    // Run filter kernel
    err = clEnqueueNDRangeKernel(queue,
                                 kernel,
                                 2,
                                 NULL,
                                 globalws,
                                 localws,
                                 0,
                                 NULL,
                                 NULL);
    cout << "clEnqueueNDRangeKernel error: " << err << "\n";

    clImage = clResult;
}

// Returns the data read from the output buffer
void* filter::readOutput() {
    std::cout << "readOutput" << std::endl;

    unsigned char newData [imageSize * 4 * sizeof(char)];

    // Transfer image back to host
    err = clEnqueueReadBuffer(queue,
                              clResult,
                              CL_TRUE,
                              0,
                              imageSize * 4 * sizeof(char),
                              (void*) newData,
                              0,
                              NULL,
                              NULL);
    cout << "enqueueReadImage error: " << err << "\n";

    return newData;
}

// Transfer debug buffer back to host
void* filter::readDebugOutput()
{
    //float debug [DEBUG_BUFFER_SIZE];
    unsigned char debug [DEBUG_BUFFER_SIZE];
    err = clEnqueueReadBuffer(queue,
                              clDebug,
                              CL_TRUE,
                              0,
                              DEBUG_BUFFER_SIZE,
                              debug,
                              0,
                              NULL,
                              NULL);
    cout << "clDebug read buffer error: " << err << "\n";

    return debug;
}

cv::Mat filter::getInputImage()
{
    return image;
}

filter::~filter()
{
}
