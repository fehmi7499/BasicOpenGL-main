#include <FloyedSteinberg.h>
#include <cmath>

#define ALPHA 7.0/16.0
#define BETA 3.0/16.0
#define GAMMA 5.0/16.0
#define DELTA 1.0/16.0

void floyedSteinberg(unsigned char* greyScale, unsigned char* floyedSteinberg, int width, int height){
    unsigned char * diff = new unsigned char[width * height]; 

    diff[0] = greyScale[0];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            floyedSteinberg[i * width + j] = trunc(diff[i * width + j]);
            char error = greyScale[i * width + j] - floyedSteinberg[i * width + j];
            diff[i * width + j + 1] = greyScale[i * width + j + 1] + error * ALPHA;
            if (i < height - 1) {
                diff[(i + 1) * width + j - 1] = greyScale[(i + 1) * width + j - 1] + error * BETA; 
                diff[(i + 1) * width + j] = greyScale[(i + 1) * width + j] + error * GAMMA;
                diff[(i + 1) * width + j + 1] = greyScale[(i + 1) * width + j + 1] + error * DELTA;
            }
        }
    }
    delete [] diff;
}