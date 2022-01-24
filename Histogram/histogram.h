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
        /* Histogram Matrix */
        Eigen::ArrayXi hist_[2];
        /* Images to show */
        cv::Mat hist_pos; // create the grayscale image for positive sae
        cv::Mat hist_neg; // create the grayscale image for negative sae
        /* Variables */
        int64_t elapsed_t = 0;
        int64_t recording_duration = 0;
};

template<int32_t width, int32_t height, int64_t delta_t>
Histogram<width, height, delta_t>::Histogram(void){
    hist_[0] = Eigen::ArrayXi::Zero(height*width + width);
    hist_[1] = Eigen::ArrayXi::Zero(height*width + width);
    cv::Mat tmp_hist_pos(height, width, CV_8UC1, cv::Scalar(0)); // create the grayscale image for positive sae
    cv::Mat tmp_hist_neg(height, width, CV_8UC1, cv::Scalar(0)); // create the grayscale image for negative sae
    hist_pos = tmp_hist_pos;
    hist_neg = tmp_hist_neg;
    // show first image
    cv::imshow("Positive Histogram", hist_pos);    
    cv::imshow("Negative Histogram", hist_neg);    
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
            hist_pos.data[index] = (uint8_t)((double)hist_[1](index)/(double)max_pos * 255);
            hist_neg.data[index] = (uint8_t)((double)hist_[0](index)/(double)max_neg * 255);
        }
        cv::imshow("Positive Histogram", hist_pos); //print the image
        cv::imshow("Negative Histogram", hist_neg); //print the image
        memset(hist_pos.data, 0, (height-1)*width + (width-1)); //reset the image
        memset(hist_neg.data, 0, (height-1)*width + (width-1)); //reset the image
        hist_[0] = Eigen::ArrayXi::Zero(height*width + width);
        hist_[1] = Eigen::ArrayXi::Zero(height*width + width);
        elapsed_t = timestamp;
        }
}