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
        cv::Mat sae_pos; // create the grayscale image for positive sae
        cv::Mat sae_neg; // create the grayscale image for negative sae
        /* Variables */
        int64_t elapsed_t = 0;
        int64_t recording_duration = 0;
};

template<int32_t width, int32_t height, int64_t delta_t>
SAE<width, height, delta_t>::SAE(void){
    cv::Mat tmp_sae_pos(height, width, CV_8UC1, cv::Scalar(0)); // create the grayscale image for positive sae
    cv::Mat tmp_sae_neg(height, width, CV_8UC1, cv::Scalar(0)); // create the grayscale image for negative sae
    sae_pos = tmp_sae_pos;
    sae_neg = tmp_sae_neg;
    // show first image
    cv::imshow("Positive SAE", sae_pos);    
    cv::imshow("Negative SAE", sae_neg);    
}

template<int32_t width, int32_t height, int64_t delta_t>
void SAE<width, height, delta_t>::ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp){

        /* Colour of the pixel */
        uint8_t colour = 0;
        colour = (uint8_t)((timestamp - elapsed_t) * 255/delta_t);

        // y * width * (nº canales) + x * (nº canales) + canal
        if(p) {
            sae_pos.data[y*width + x] = (int)colour;
        }
        else {
            sae_neg.data[y*width + x] = (int)colour;
        }

        // check if image is ready
        if(timestamp - elapsed_t > delta_t) {
            cv::imshow("Positive SAE", sae_pos); //print the image
            cv::imshow("Negative SAE", sae_neg); //print the image
            memset(sae_pos.data, 0, (height-1)*width*1 + (width-1)*1); //reset the image
            memset(sae_neg.data, 0, (height-1)*width*1 + (width-1)*1); //reset the image
            elapsed_t = timestamp;
        }
}