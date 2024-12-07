#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <ImageProcesseing.h>
#include <Halftone.h>
#include <FloyedSteinberg.h>
#include <cmath>
#include <vector>

#include <iostream>


int main() {
    int width, height, comps;

    unsigned char* img = stbi_load("res/textures/Lenna.png", &width, &height, &comps, 0);

    if(img){
        std::cout << "Image loaded succefully: "
                  << width << "x" << height << " comps: " << comps << std::endl;
        
        //------------------------------------------------------Grayscale--------------------------------------------------------
        // buffer for the grayscale image
        unsigned char* grayscaleImg = new unsigned char[width * height];

        // convert every pixel to grayscale
        for (int h = 0; h < height; h++){
            for (int w = 0; w < width; w++){
                int index = (h * width + w) * 4;
                unsigned char r = img[index];
                unsigned char g = img[index + 1];
                unsigned char b = img[index + 2];

                //Grayscale formula
                unsigned char gray = static_cast<unsigned char>(0.2989f * r + 0.587f * g + 0.114f * b);
                grayscaleImg[h * width + w] = gray;
            }
            
        }
        if(stbi_write_png("Grayscale.png", width, height, 1, grayscaleImg, width)) {
            std::cout << "Grayscale Image saved as Grayscale.png\n";
        
        } else{
            std::cerr << "Failed to save Grayscale image\n";
        }

        //---------------------------------------------------------- GaussianBlur ----------------------------------------------------------------------------
        // buffer for GaussianBlur image
        unsigned char* img1 = stbi_load("../bin/Grayscale.png", &width, &height, &comps, 0);
        unsigned char* blurredImg = new unsigned char[width * height];
        applyGaussianBlur(img1, blurredImg, width, height);
        if(stbi_write_png("GaussianBlur.png", width, height, 1, blurredImg, width)){
            std::cout << "GaussianBlur Image saved as GaussianBlur.png\n";
        
        } else{
            std::cerr << "Failed to save GaussianBlur image\n";
        }
        //---------------------------------------------------------- Gradient Calculation -> sobel filter -----------------------------------------------------
        // buffers for sobel gradients
        unsigned char* img2 = stbi_load("../bin/GaussianBlur.png", &width, &height, &comps, 0);
        float* Gx = new float[width * height];
        float* Gy = new float[width * height];
        
        //apply sobel filters
        computeSobelGradients(img2, Gx, Gy, width, height);

        //compute gradient magnitude and direction
        unsigned char* magnitude = new unsigned char[width * height];
        for (int y = 1; y < height - 1; y++){
            for (int x = 1; x < width - 1; x++){
                int idx = y * width + x;
                float gradMag = sqrt(Gx[idx] * Gx[idx] + Gy[idx] * Gy[idx]);
                magnitude[idx] = static_cast<unsigned char>(std::min(255.0f, gradMag));
            }
        }
        //------------------------------------------------------------- Non-Maximum Suppression -------------------------------------------------
        //Apply Non Maximum suppression
        unsigned char* nmsOutPut = new unsigned char[width * height];
        nonMaximumSuppression(magnitude, Gx, Gy, nmsOutPut, width, height);

        //save non maximum suppression output
        if(stbi_write_png("NonMaximumSuppression.png", width, height, 1, magnitude, width)){
            std::cout << "NonMaximumSuppression Image saved as NonMaximumSuppression.png\n";
        
        } else{
            std::cerr << "Failed to save NonMaximumSuppression image\n";
        }

        //--------------------------------------------------------------- Double thresholding -----------------------------------------------------
        unsigned char* edgeOutput = new unsigned char[width * height];
        doubleThreshold(nmsOutPut, edgeOutput, width, height, 50, 100);  // Low=50, High=100
        //Edge tracking by Hysteresis
        edgeTrackingByHysteresis(edgeOutput, width, height);
        //Save final edge-detected Image
        if(stbi_write_png("CannyEdgeDetection.png", width, height, 1, edgeOutput, width)){
            std::cout << "Canny Edge Detection Image saved as CannyEdgeDetection.png\n";
        } else {
            std::cerr << "Failed to save Canny Edge Detecion image\n";
        }

        //------------------------------------------------------------ Halftone ------------------------------------------------------------------
        unsigned char * halftoneImg = new unsigned char[width * height * 4];
        halftone(img1, halftoneImg, width, height);
        if(stbi_write_png("Halftone.png", width * 2, height * 2, 1, halftoneImg, width * 2)){
            std::cout << "Halftone Image saved as Halftone.png\n";
        } else {
            std::cerr << "Failed to save Halftone image\n";
        }

        //------------------------------------------------------------ Floyed Svteinberg ---------------------------------------------------------------------------
        unsigned char * floyedSteinbergImg = new unsigned char [width * height];
        floyedSteinberg(img1, floyedSteinbergImg, width, height);
        if(stbi_write_png("FloyedSteinberg.png", width, height, 1, floyedSteinbergImg, width)){
            std::cout << "FloyedSteinberg Image saved as FloyedSteinberg.png\n";
        } else {
            std::cerr << "Failed to save FloyedSteinberg image\n";
        }

        delete[] grayscaleImg;
        delete[] blurredImg;
        delete[] Gx;
        delete[] Gy;
        delete[] magnitude;
        delete[] nmsOutPut;
        delete[] edgeOutput;
        delete[] halftoneImg;
        delete[] floyedSteinbergImg;
        stbi_image_free(img);
        stbi_image_free(img1);
        stbi_image_free(img2);
    } else {
        std::cerr << "Failed to load image\n";
    }


    return 0;
    
}