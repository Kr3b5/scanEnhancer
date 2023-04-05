using namespace std;

unsigned char* mean_threshold(const unsigned char *img, int width, int height, int radius, int C, int txt_mode);
unsigned char* gauss(const unsigned char *img, int width, int height, int radius);
bool radius_check(int radius, int width, int height);