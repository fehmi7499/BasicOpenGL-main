#include "Halftone.h"

#define BLACK 0
#define WHITE 255

void halftone(unsigned char* greyScale, unsigned char* halftone, int width, int height){
    
    for (int i = 0; i < width * height; i++) {
        int row = i / width;
        int col = i % width;
        int curr = greyScale[i];
        int pos1 = row * 2 * 2 * width + col * 2;
        int pos2 = (row * 2 + 1) * 2 * width + col * 2;
        int grade = WHITE / 5;

        if (curr < grade) {
            halftone[pos1] = BLACK;
            halftone[pos1 + 1] = BLACK;
            halftone[pos2] = BLACK;
            halftone[pos2 + 1] = BLACK;

        } else if (curr >= grade && curr < grade * 2 ) {
            halftone[pos1] = BLACK; 
            halftone[pos1 + 1] = BLACK;
            halftone[pos2] = WHITE;
            halftone[pos2 + 1] = BLACK;

        } else if (curr >= grade * 2 && curr < grade * 3) {
            halftone[pos1] = WHITE;
            halftone[pos1 + 1] = BLACK;
            halftone[pos2] = WHITE;
            halftone[pos2 + 1] = BLACK;

        } else if (curr >= grade * 3 && curr < grade * 4) {
            halftone[pos1] = BLACK;
            halftone[pos1 + 1] = WHITE;
            halftone[pos2] = WHITE;
            halftone[pos2 + 1] = WHITE;

        } else if (curr >= grade * 4) {
            halftone[pos1] = WHITE;
            halftone[pos1 + 1] = WHITE;
            halftone[pos2] = WHITE;
            halftone[pos2 + 1] = WHITE;
        }
    }
}