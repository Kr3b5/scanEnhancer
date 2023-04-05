#include <iostream>
#include "filter.h"
#include <cmath>

#define PI           3.14159265358979323846

/**
 * Mean threshold filter
 *
 * @param img pointer to image data
 * @param width width of image
 * @param height height of image
 * @param radius radius for the filter
 * @param C constant C for the filter
 * @return enhanced image data
 */
unsigned char* mean_threshold(const unsigned char *img, int width, int height, int radius, int C, int txt_mode) {
    if(!radius_check(radius, width, height)) return const_cast<unsigned char *>(img);

    auto *filter_img = new unsigned char[(width*height*3)];

#pragma omp parallel for
    for (int r=0; r < height; r++) {
        for (int c=0; c < width; c++) {

            int sum = 0;
            for (int x=-radius; x <= radius; x++) {
                for (int y=-radius; y <= radius; y++) {
                    for (int rgb=0; rgb < 3; rgb++) {
                        if ((r+x) < height && (r+x) >= 0 && (c+y) < width && (c+y) >= 0) sum += img[(r+x)*width*3 + (c+y)*3 + rgb];
                    }
                }
            }

            int px_pos = r*width*3 + c*3;

            unsigned char px_intensity = (img[px_pos] + img[px_pos+1] + img[px_pos+2]) / 3;
            auto mean = (unsigned char) (sum / ((radius*2+1) * (radius*2+1) * 3));

            if (px_intensity > (mean - C)) {
                filter_img[px_pos] = filter_img[px_pos+1] = filter_img[px_pos+2] = 255;
            } else {
                if(txt_mode == 1){
                    filter_img[px_pos] = filter_img[px_pos+1] = filter_img[px_pos+2] = 1;
                }else{
                    filter_img[px_pos] = img[px_pos];
                    filter_img[px_pos+1] = img[px_pos+1];
                    filter_img[px_pos+2] = img[px_pos+2];
                }
            }
        }
    }

    delete[] img;
    return filter_img;
}


/**
 * Gauss filter
 *
 * @param img pointer to image data
 * @param width width of image
 * @param height height of image
 * @param radius radius for the filter
 * @return enhanced image data
 */
unsigned char* gauss(const unsigned char *img, int width, int height, int radius) {
    if(!radius_check(radius, width, height)) return const_cast<unsigned char *>(img);

    float sigma = (float)radius/3;
    float k[(radius*2+1) * (radius*2+1)];
    
    float weight = 0;
    // no significant improvement if you use omp parallel (20 measurements * 3 different radius)
    for (int x=-radius; x <= radius; x++) {
        for (int y=-radius; y <= radius; y++) {
            auto t = (float)((1 / (2*PI*sigma*sigma)) * exp(-(x*x + y*y)/(2*sigma*sigma)));
            k[(x+radius)*(2*radius+1) + (y+radius)] = t;
            weight += t;
        }
    }

    auto filter_img = new unsigned char[width*height*3];
#pragma omp parallel for
    for (int r=0; r < height; r++) {
        for (int c=0; c < width; c++) {
            for (int rgb=0; rgb < 3; rgb++) {
                float sum = 0;
                for (int x=-radius; x <= radius; x++) {
                    for (int y=-radius; y <= radius; y++) {
                        if (r+x < height && r+x >= 0 && c+y < width && c+y >= 0) {
                            sum += img[(r+x)*width*3 + (c+y)*3 + rgb] * k[(x+radius)*(2*radius+1) + (y+radius)];
                        }
                    }
                }
                filter_img[r*width*3 + c*3 + rgb] =  (unsigned char) (sum / weight);
            }
        }
    }
    delete[] img;
    return filter_img;
}

// Helper Methods

/**
 * Radius checker
 *
 * The radius must be a number between 1 and half of the smallest image side
 *
 * @param radius given radius for the filter
 * @param width width of image
 * @param height height of image
 * @return "true" or "false"
 */
bool radius_check(int radius, int width, int height){
    bool correct = true;
    if( radius > (width/2) || radius > (height/2) ){
        int max = (width > height) ? (width/2) : (height/2);
        cerr << "Error: The radius is too large for the image. It can be maximum " << max << endl;
        correct = false;
    }else if( radius < 1 ){
        cerr << "Error: The radius must be at least 1!" << endl;
        correct = false;
    }
    return correct;
}

