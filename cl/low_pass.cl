// TODO convert to float4 to use GPU vector arithimitic optimization
// TODO make static parameters constant
// maskSize must be odd
#define BLOCK_SIZE 16  // size of local workspace
__kernel void filter_kernel(
        __global uchar4 * image, //bgr
        __global uchar4 * blurredImage, //bgr
        int imageWidth,
        int imageHeight,
        int maskSize,
	__global uchar * debugBuffer
    ) {

    int r = maskSize / 2;

    // storing image data locally\
    // TODO pass in this memory as a paramter to allow for variable sizing
    __local uchar4 P[(BLOCK_SIZE+2*2)*(BLOCK_SIZE+2*2)];  // cannot be variable values

    // Identify workgroup
    int i = get_group_id(0);
    int j = get_group_id(1);

    // TODO: avoid edge cases
    /*clamp(i, 1, imageWidth / 16 - 2);
    clamp(j, 1, imageHeight / 16 - 2);*/

    // Indentify workitem
    int iDx = get_local_id(0);
    int iDy = get_local_id(1);
    int ii = i*BLOCK_SIZE + iDx;  // == get_global_id(0)
    int jj = j*BLOCK_SIZE + iDy;  // == get_global_id(1)

    // Calculate local and global indexes
    int imgPosGlobal = (jj * imageWidth + ii);
    int imgPosLocal = (iDy * (BLOCK_SIZE+2*r) + iDx);

    // moves the local workspace from the center of the roi to the upper left corner of the roi
//    int buffOffset = (BLOCK_SIZE*r+maskSize*r);
    int buffOffset = (BLOCK_SIZE*r+r);

    // Read pixels in workspace
    P[imgPosLocal] = image[imgPosGlobal - buffOffset];

    // Copies borders on the right side
    if (iDx < maskSize)  // for maskSize of 5 this is less than 4
    {
        // TODO: edge cases which grab black pixels around the edges
        P[imgPosLocal + BLOCK_SIZE] = image[imgPosGlobal - buffOffset + BLOCK_SIZE];
    }
    // Copies borders on the bottom
    if (iDy < maskSize)
    {
        // TODO: edge cases
        P[imgPosLocal + (BLOCK_SIZE+2*r)*BLOCK_SIZE] = image[imgPosGlobal - buffOffset + BLOCK_SIZE*imageWidth];
    }
    // Copies bottom right corner
    if (iDx > BLOCK_SIZE - maskSize && iDy > BLOCK_SIZE - maskSize)
    {
        P[imgPosLocal + (r*2 + (BLOCK_SIZE+r*2)*r*2)] = image[imgPosGlobal - buffOffset + (r*2 + imageWidth*r*2)];
    }

    // Make sure all threads have finished loading all pixels
    barrier(CLK_LOCAL_MEM_FENCE);

//    // copies image for testing, not need for convolution
    blurredImage[imgPosGlobal - buffOffset] = P[imgPosLocal];

//    // Perform convolution
//    uchar sum0 = 0;
//    uchar sum1 = 0;
//    uchar sum2 = 0;
//    uchar sum3 = 0;

//    for (int i = -r; i <= r; i++) {
//        for (int j = -r; j <= r; j++) {
//            sum0 = sum0 + P[imgPosLocal + buffOffset + 3 + ((BLOCK_SIZE+r*2) * j + i)].x / (maskSize * maskSize);
//            sum1 = sum1 + P[imgPosLocal + buffOffset + 3 + ((BLOCK_SIZE+r*2) * j + i)].y / (maskSize * maskSize);
//            sum2 = sum2 + P[imgPosLocal + buffOffset + 3 + ((BLOCK_SIZE+r*2) * j + i)].z / (maskSize * maskSize);
//            sum2 = sum2 + P[imgPosLocal + buffOffset + 3 + ((BLOCK_SIZE+r*2) * j + i)].w / (maskSize * maskSize);
//        }
//    }
//    blurredImage[imgPosGlobal - buffOffset].x = sum0;
//    blurredImage[imgPosGlobal - buffOffset].y = sum1;
//    blurredImage[imgPosGlobal - buffOffset].z = sum2;
//    blurredImage[imgPosGlobal - buffOffset].w = sum3;
}

