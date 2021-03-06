#include <opencv2/opencv.hpp>
#include <inttypes.h>

using namespace std;

template<int32_t width, int32_t height, int64_t delta_t>
class SAE{
    public:
        /* Constructor */
        SAE();
        /* Show Events */
        void ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp);
    private:
        /* Images to show */
        cv::Mat sae_pos; 
        cv::Mat sae_neg; 
        /* Variables */
        int64_t elapsed_t = 0;
        int64_t recording_duration = 0;
};

template<int32_t width, int32_t height, int64_t delta_t>
SAE<width, height, delta_t>::SAE(void){
    /* Tmp RGB images and asignation */
    cv::Mat tmp_sae_pos(height, width, CV_8UC3, cv::Scalar(0,0,0)); 
    cv::Mat tmp_sae_neg(height, width, CV_8UC3, cv::Scalar(0,0,0)); 
    sae_pos = tmp_sae_pos;
    sae_neg = tmp_sae_neg;
    /* Show first image */
    cv::imshow("Positive SAE", sae_pos);
    cv::moveWindow("Positive SAE", 40,30);        
    cv::imshow("Negative SAE", sae_neg);   
    cv::moveWindow("Negative SAE", 40+width+10,30);        
}

template<int32_t width, int32_t height, int64_t delta_t>
void SAE<width, height, delta_t>::ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp){

        /* Colour of the pixel */
        uint8_t colour = 0;
        colour = (uint8_t)((timestamp - elapsed_t) * 255/delta_t);

        /* y * width * max channels + x * max channels + channel */
        if(p) {
            sae_pos.data[y*width*3 + x*3] = colour;             /* BLUE CHANNEL */
            sae_pos.data[y*width*3 + x*3 + 1] = 0;              /* GREEN CHANNEL */
            sae_pos.data[y*width*3 + x*3 + 2] = 255 - colour;   /* RED CHANNEL */

        }
        else {
            sae_neg.data[y*width*3 + x*3] = colour;             /* RED CHANNEL */
            sae_neg.data[y*width*3 + x*3 + 1] = 0;              /* GREEN CHANNEL */
            sae_neg.data[y*width*3 + x*3 + 2] = 255 - colour;   /* BLUE CHANNEL */        
        }

        /* Check if an image must be printed */
        if(timestamp - elapsed_t > delta_t) {
            /* Print images */
            cv::imshow("Positive SAE", sae_pos); 
            cv::imshow("Negative SAE", sae_neg); 
            /* Reset images */
            memset(sae_pos.data, 0, (height-1)*width*3 + (width-1)*3+2); 
            memset(sae_neg.data, 0, (height-1)*width*3 + (width-1)*3+2);
            elapsed_t = timestamp;
        }
}