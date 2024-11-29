#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <iostream>

void applyGaussianBlur(unsigned char* src, unsigned char* dst, int width, int height);
void computeSobelGradients(unsigned char* src, float* Gx, float* Gy, int width, int height);
void nonMaximumSuppression(unsigned char* magnitude, float* Gx, float* Gy, unsigned char* output, int width, int height);

#endif