#include <opencv2/opencv.hpp>
#include <inttypes.h>

using namespace std;

template<int32_t width, int32_t height, int64_t delta_t>
class AbsoluteSAE{
    public:
        /* Constructor */
        AbsoluteSAE();
        /* Show Events */
        void ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp);
    private:
        /* Images to show */
        cv::Mat sae;
        /* Variables */
        int64_t elapsed_t = 0;
        int64_t recording_duration = 0;
};

template<int32_t width, int32_t height, int64_t delta_t>
AbsoluteSAE<width, height, delta_t>::AbsoluteSAE(void){
    /* Tmp RGB images and asignation */
    cv::Mat tmp_sae(height, width, CV_8UC3, cv::Scalar(0,0,0)); 
    sae = tmp_sae;
    /* Show first image */
    cv::imshow("SAE", sae);    
}

template<int32_t width, int32_t height, int64_t delta_t>
void AbsoluteSAE<width, height, delta_t>::ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp){

        /* Colour of the pixel */
        uint8_t colour = 0;
        colour = (uint8_t)((timestamp - elapsed_t) * 255/delta_t);

        /* y * width * max channels + x * max channels + channel */
        sae.data[y*width*3 + x*3] = (int)colour;            /* BLUE CHANNEL */
        sae.data[y*width*3 + x*3 + 1] = (int)0;             /* GREEN CHANNEL */
        sae.data[y*width*3 + x*3 + 2] = 255 - (int)colour;  /* RED CHANNEL */

        /* Check if an image must be printed */
        if(timestamp - elapsed_t > delta_t) {
            /* Print images */
            cv::imshow("SAE", sae); 
            /* Reset images */
            memset(sae.data, 0, (height-1)*width*3 + (width-1)*3+2); 
            elapsed_t = timestamp;
        }
}