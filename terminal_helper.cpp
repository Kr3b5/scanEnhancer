#include "terminal_helper.h"
#include <iostream>

using namespace std;

/**
 * Prints the help section
 */
void tout_help(){
    cout << "usage: scanEnhancer <path input file> <path output file> [filters]" << endl;
    cout << "  filter (needs at least one filter):" << endl;
    cout << "    -g <radius>                    apply gauss filter with radius" << endl;
    cout << "    -t <radius> <C> <text mode>    apply mean threshold filter with radius and offset C" << endl;
    cout << "           > text mode = [0 = off | 1 = on] - colors all pixels that are not removed black" << endl;
}

/**
 * Prints the start of mean threshold filter
 *
 * @param r radius
 * @param C constant C
 */
void tout_startMT(int r, int C, int txt_mode){
    cout << "Start mean threshold filter ( radius = " << r << " | C = " << C << "| text mode = " <<  txt_mode <<  ")..." << endl;
}

/**
 * Prints the start of gauss filter
 *
 * @param r radius
 */
void tout_startGF(int r){
    cout << "Start gauss filter ( radius = " << r << " )..." << endl;
}

/**
 * Prints the time needed for processing the filters
 *
 * @param t_start start time
 * @param t_end end time
 */
void tout_time(double t_start, double t_end){
    cout << "Time required for processing " << (t_end-t_start) << " s" << endl;
}

/**
 * Prints the start of gauss filter
 *
 * @param r radius
 */
void tout_err_txtmode(int txt_mode){
    cerr << "Error: Wrong number for textmode specified (Your input: " << txt_mode << "). [0 = off | 1 = on]" << endl;
    exit(1);
}

// Checker

/**
 * Prints the start of gauss filter
 *
 * @param r radius
 */
bool check_txtmode(int mode) {
    if(mode == 0 || mode == 1) return true;
    else{
        tout_err_txtmode(mode);
        return false;
    }
}