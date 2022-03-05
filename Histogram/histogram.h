#include <opencv2/opencv.hpp>
#include <inttypes.h>
#include <Eigen/Dense>

using namespace std;

template<int32_t width, int32_t height, int64_t delta_t>
class Histogram{
    public:
        /* Constructor */
        Histogram();
        /* Show Events */
        void ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp);
    private:
        /* Histogram ArrayX */
        Eigen::ArrayXi hist_[2];
        /* Images to show */
        cv::Mat hist_pos;
        cv::Mat hist_neg;
        /* Variables */
        int64_t elapsed_t = 0;
        int64_t recording_duration = 0;
};

template<int32_t width, int32_t height, int64_t delta_t>
Histogram<width, height, delta_t>::Histogram(void){
    hist_[0] = Eigen::ArrayXi::Zero(height*width + width);
    hist_[1] = Eigen::ArrayXi::Zero(height*width + width);
    /* Tmp RGB images and asignation */
    cv::Mat tmp_hist_pos(height, width, CV_8UC3, cv::Scalar(0,0,0)); 
    cv::Mat tmp_hist_neg(height, width, CV_8UC3, cv::Scalar(0,0,0)); 
    hist_pos = tmp_hist_pos;
    hist_neg = tmp_hist_neg;
    // show first image
    cv::imshow("Positive Histogram", hist_pos);
    cv::moveWindow("Positive Histogram", 40,30);
    cv::imshow("Negative Histogram", hist_neg);
    cv::moveWindow("Negative Histogram", 40+width,30);    
}

template<int32_t width, int32_t height, int64_t delta_t>
void Histogram<width, height, delta_t>::ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp){

    /*Update Histogram */
    const bool pol = p ? 1 : 0;
    hist_[pol](y*width + x)++; 

    /* Check if an image must be shown */
    if(timestamp - elapsed_t > delta_t) {
        /* Get the maximum value of each histogram */
        auto max_pos = hist_[1].maxCoeff();
        auto max_neg = hist_[0].maxCoeff();
        for(uint32_t index = 0; index < (height-1)*width + (width-1); index++){
            if(hist_[0](index) == 0 || hist_[1](index) == 0) continue;
            else{
            hist_pos.data[index*3] = (uint8_t)((double)hist_[1](index)/(double)max_pos * 255);          /* BLUE CHANNEL*/
            hist_pos.data[index*3+1] = 0;                                                               /* GREEN CHANNEL */
            hist_pos.data[index*3+2] = 255 - (uint8_t)((double)hist_[1](index)/(double)max_pos * 255);  /* RED CHANNEL */

            hist_neg.data[index*3] = (uint8_t)((double)hist_[0](index)/(double)max_neg * 255);          /* BLUE CHANNEL*/
            hist_neg.data[index*3+1] = 0;                                                               /* GREEN CHANNEL */
            hist_neg.data[index*3+2] = 255 - (uint8_t)((double)hist_[0](index)/(double)max_neg * 255);  /* RED CHANNEL */
            }
        }
        /* Print images */
        cv::imshow("Positive Histogram", hist_pos); 
        cv::imshow("Negative Histogram", hist_neg); 
        /* Reset images */
        memset(hist_pos.data, 0, (height-1)*width*3 + (width-1)*3+2); 
        memset(hist_neg.data, 0, (height-1)*width*3 + (width-1)*3+2); 
        hist_[0] = Eigen::ArrayXi::Zero(height*width + width);
        hist_[1] = Eigen::ArrayXi::Zero(height*width + width);
        elapsed_t = timestamp;
        }
}