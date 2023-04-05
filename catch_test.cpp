#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE // for fast compilation
#include <catch2/catch_test_macros.hpp>
#include "filter.h"
#include "fileHelper.h"

using namespace std;

// Helper Methods

bool equality_test(const unsigned char *img_1, const unsigned char *img_2, int size){
    bool equality = true;
    if(img_1 != nullptr && img_2 != nullptr){
        for (int i=0; i < size; i++) {
            if (abs(*(img_1+i) - *(img_2+i)) > 2) {     // Prevention of rounding errors
                equality = false;
            }
        }
    }else{
        equality = false;
    }
    return equality;
}


// TESTCASES

TEST_CASE("Read image data from .ppm file") {
    char input[] = "../img/test_img.ppm";

    int width, height;
    auto img = read(input, &width, &height);
    REQUIRE(img != nullptr);
}

TEST_CASE("Write image data to .ppm file") {
    char input[] = "../img/test_img.ppm";
    char output[] = "../img/test_write.ppm";

    int width, height;
    auto img = read(input, &width, &height);
    write(output, img, width, height);
    auto img_write = read(output, &width, &height);
    remove(output);

    bool equality = equality_test(img, img_write, (width*height*3));
    delete[] img;
    delete[] img_write;

    REQUIRE(equality);
}

TEST_CASE("Filter - Gauss") {
    char input[] = "../img/test_img.ppm";
    char correct[] = "../img/test_gauss.ppm";

    int width, height;
    auto img_in = read(input, &width, &height);
    img_in = gauss(img_in, width, height, 3);

    auto img_correct = read(correct, &width, &height);

    bool equality = equality_test(img_in, img_correct, (width*height*3));
    delete[] img_in;
    delete[] img_correct;

    REQUIRE(equality);
}

TEST_CASE("Filter - Threshold mean") {
    char input[] = "../img/test_img.ppm";
    char correct[] = "../img/test_thresholdmean.ppm";

    int width, height;
    auto img_in = read(input, &width, &height);
    img_in = mean_threshold(img_in, width, height, 10, 6 , 0);

    auto img_correct = read(correct, &width, &height);

    bool equality = equality_test(img_in, img_correct, (width*height*3));
    delete[] img_in;
    delete[] img_correct;

    REQUIRE(equality);
}

TEST_CASE("Filter - Threshold mean - text mode") {
    char input[] = "../img/test_img.ppm";
    char correct[] = "../img/test_thresholdmean_txtmode.ppm";

    int width, height;
    auto img_in = read(input, &width, &height);
    img_in = mean_threshold(img_in, width, height, 10, 6 , 1);

    auto img_correct = read(correct, &width, &height);

    bool equality = equality_test(img_in, img_correct, (width*height*3));
    delete[] img_in;
    delete[] img_correct;

    REQUIRE(equality);
}

