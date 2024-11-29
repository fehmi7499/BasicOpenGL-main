#include "ImageProcesseing.h"
#include <cmath>
#define M_PI 3.14159265358979323846

// Implement of Gaussian blur
void applyGaussianBlur(unsigned char* src, unsigned char* dst, int width, int height){
    float gaussianKernel[5][5] = {
        {0.0126, 0.0252, 0.0314, 0.0252, 0.0126},
        {0.0252, 0.0566, 0.0755, 0.0566, 0.0252},
        {0.0314, 0.0755, 0.0943, 0.0755, 0.0314},
        {0.0252, 0.0566, 0.0755, 0.0566, 0.0252},
        {0.0126, 0.0252, 0.0314, 0.0252, 0.0126}
    };

    float sum1 = 0.0f;
    for(int i = 0; i< 5; i++){
        for(int j = 0; j< 5; j++){
            sum1 += gaussianKernel[i][j];
        }
    }
    std::cout << "Gaussian Kernel Sum: " << sum1 << "\n" << std::endl;

    // the first and second for() starts at index 2 are to avoid 
    // accessing out of bounds pixels at the edges of the image
    for (int y = 2; y < height -2; y++) {
        for (int x=2; x < width -2; x++){
            float sum = 0.0f;

            // kernel offset
            for (int ky = -2; ky <= 2; ky++){
                for (int kx = -2; kx <= 2; kx++){
                    int pixeldx = (y + ky) * width + (x + kx);
                    sum += src[pixeldx] * gaussianKernel[ky + 2][kx + 2];
                }
            }

            dst[y * width + x] = static_cast<unsigned char>(sum);
        }
    }
}

// Implementation of sobel gradient computation
void computeSobelGradients(unsigned char* src, float* Gx, float* Gy, int width, int height){
    int sobelX[3][3] = {
        {-1, 0, 1} ,
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int sobelY[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for(int y = 1; y < height - 1; y++){
        for(int x = 1; x < width - 1; x++){
            float gradX = 0.0f, gradY = 0.0f;

            for(int ky = -1; ky <= 1; ky++){
                for(int kx = -1; kx <= 1; kx++){
                    int pixeldx = (y + ky) * width + (x + kx);
                    gradX += src[pixeldx] * sobelX[ky + 1][kx + 1];
                    gradY += src[pixeldx] * sobelY[ky + 1][kx + 1];
                }
            }

            Gx[y * width + x] = gradX;
            Gy[y * width + x] = gradY;
        }
    }

}

void nonMaximumSuppression(unsigned char* magnitude, float* Gx, float* Gy, unsigned char* output, int width, int height){
    for (int y = 1; y < height -1; y++){
        for (int x = 1; x < height -1; x++){
            int idx = y * width + x;

            // compute gradient direction
            float angle = atan2(Gy[idx], Gx[idx]) * 180.0f / M_PI; // convert to degree
            angle = (angle < 0) ? angle + 180 : angle; // Normalize to [0. 180]

            // Determine neighbors to compare
            unsigned char neighbor1 = 0, neighbor2 = 0;
            if((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle <=180)){
                neighbor1 = magnitude[idx - 1];         // left
                neighbor2 = magnitude[idx + 1];         // right
            } else if(angle >= 22.5 && angle < 67.5){
                neighbor1 = magnitude[idx - width + 1];         // top right
                neighbor2 = magnitude[idx + width - 1];         // bottom lrft
                
            } else if(angle >= 67.5 && angle < 112.5){
                neighbor1 = magnitude[idx - width];      // top
                neighbor2 = magnitude[idx + width];      // bottom
            } else if(angle >= 112.5 && angle < 157.5){
                neighbor1 = magnitude[idx - width - 1];         // top right
                neighbor2 = magnitude[idx + width + 1];         // bottom lrft
            }

            // suppress non maximum pixels
            if (magnitude[idx] >= neighbor1 && magnitude[idx] >= neighbor2) {
                output[idx] = magnitude[idx];
            } else {
                output [idx] = 0;
            }
             

        }
    }
}

