#include <iostream>
#include <fstream>

using namespace std;

/**
 * Reads in the ppm file
 *
 * @param filename Name of the input file
 * @param img pointer to data of image
 * @param width pointer to width of image
 * @param height pointer to height of image
 * @return image data or nullptr if an error occurs
 */
unsigned char* read(char filename[], int *width, int *height) {
    ifstream input(filename, ios::in);
    if(input.is_open()) {
        string format;
        input >> format;

        // TEST: File format check
        if (format != "P3") {
            cerr << "Error: Input file has to be a P3 file (ASCII)" << endl;
            cerr << "File Format: " << format << endl;
            input.close();
            return nullptr;
        }

        // Read Header
        int max;
        input >> *width >> *height >> max;

        //Read data
        cout << "Reading image " << filename << "( width: " << *width << " | height: " << *height << " )" << endl;
        size_t size = (*width) * (*height) * 3; // 3 = RGB
        auto img = new unsigned char[size];
        unsigned int colorCode;
        for (int i=0; i < size; i++) {
            input >> colorCode;
            img[i] = (char) colorCode;
        }

        input.close();
        return img;
    }
    else {
        cerr << "Error: Unable to open " << filename << endl;
        return nullptr;
    }
}

/**
 * Writes the ppm file
 *
 * @param filename Name of the output file
 * @param img pointer to data of image
 * @param width pointer to width of image
 * @param height pointer to height of image
 */
void write(char filename[], unsigned char *img, int width, int height) {
    ofstream output(filename, ios::out);
    if(output.is_open()) {
        // Write Header
        char format[3] = "P3";
        output << format << endl
               << width << " " << height << endl
               << 255 << endl;

        //Write data
        unsigned int colorCode;
        cout << "Writing image " << filename << endl;
        for (int r=0; r < height; r++) {
            for (int c=0; c < width; c++) {
                for (int rgb=0; rgb < 3; rgb++) {
                    colorCode = (unsigned) img[(r*width*3 + c*3) + rgb];
                    output << colorCode << " ";
                }
            }
            output << endl;
        }
        output.close();
    }
    else {
        cerr << "Error: Unable to write " << filename << endl;
    }
}