#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <omp.h>
#include "fileHelper.h"
#include "filter.h"
#include "terminal_helper.h"

using namespace std;

int main(int argc, char *argv[]) {
    char *in, *out;
    in = argv[1];
    out = argv[2];

    // TEST: Arguments
    if(argc < 5){
        tout_help();
        return EXIT_FAILURE;
    }

    int width, height;
    auto img = read(in, &width, &height);
    // TEST: image was read in
    if (!img) {
        cerr << "Error: could not read the image " << in << endl;
        return EXIT_FAILURE;
    }

    auto t_start = omp_get_wtime();
    // -g <radius>
    if( argc == 5 && strcmp(argv[3], "-g") == 0 && isdigit(*argv[4]) ){
        int r = atoi(argv[4]);
        tout_startGF(r);
        img = gauss(img, width, height, r);
    }
    // -t <radius> <C> <txt_mode>
    else if( argc == 7 && strcmp(argv[3], "-t") == 0 && isdigit(*argv[4]) && isdigit(*argv[5]) && isdigit(*argv[6]) ){
        int r = atoi(argv[4]);
        int C = atoi(argv[5]);
        int txt_mode = atoi(argv[6]);
        if(check_txtmode(txt_mode)){
            tout_startMT(r, C, txt_mode);
            img = mean_threshold(img, width, height, r, C, txt_mode);
        }
    }else if(argc == 9){
        // -g <radius> -t <radius> <C> <txt_mode>
        if( strcmp(argv[3], "-g") == 0 && strcmp(argv[5], "-t") == 0 ){
            int r_G = atoi(argv[4]);
            int r_T = atoi(argv[6]);
            int C = atoi(argv[7]);
            int txt_mode = atoi(argv[8]);
            if(check_txtmode(txt_mode)){
                tout_startGF(r_G);
                img = gauss(img, width, height, r_G);
                tout_startMT(r_T, C, txt_mode);
                img = mean_threshold(img, width, height, r_T, C, txt_mode);
            }
        }
        // -t <radius> <C> <txt_mode> -g <radius>
        else if( strcmp(argv[3], "-t") == 0 && strcmp(argv[7], "-g") == 0 ){
            int r_G = atoi(argv[8]);
            int r_T = atoi(argv[4]);
            int C = atoi(argv[5]);
            int txt_mode = atoi(argv[6]);
            if(check_txtmode(txt_mode)){
                tout_startMT(r_T, C, txt_mode);
                img = mean_threshold(img, width, height, r_T, C, txt_mode);
                tout_startGF(r_G);
                img = gauss(img, width, height, r_G);
            }
        }
    }else{
        tout_help();
        return EXIT_FAILURE;
    }
    auto t_end = omp_get_wtime();

    write(out, img, width, height);

    tout_time(t_start, t_end);

    delete[] img;
    return EXIT_SUCCESS;
}