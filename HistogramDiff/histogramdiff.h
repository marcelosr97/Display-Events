#include <opencv2/opencv.hpp>
#include <inttypes.h>
#include <Eigen/Dense>

using namespace std;

template<int32_t width, int32_t height, int64_t delta_t>
class HistogramDiff{
    public:
        /* Constructor */
        HistogramDiff();
        /* Show Events */
        void ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp);
    private:
        /* Histogram ArrayX */
        Eigen::ArrayXi histdiff;
        /* Images to show */
        cv::Mat histdiff_img;
        /* Variables */
        int64_t elapsed_t = 0;
        int64_t recording_duration = 0;
};

template<int32_t width, int32_t height, int64_t delta_t>
HistogramDiff<width, height, delta_t>::HistogramDiff(void){
    histdiff = Eigen::ArrayXi::Zero(height*width + width);
    /* Tmp RGB images and asignation */
    cv::Mat tmp_histdiff_img(height, width, CV_8UC3, cv::Scalar(0,0,0)); 
    histdiff_img = tmp_histdiff_img;
    /* Show first image */
    cv::imshow("Histogram Diff", histdiff_img);    
}

template<int32_t width, int32_t height, int64_t delta_t>
void HistogramDiff<width, height, delta_t>::ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp){

    /*Update HistogramDiff */
    const bool pol = p ? 1 : 0;
    if(p) histdiff(y*width + x)++; 
    else histdiff(y*width + x)--; 

    /* Check if an image must be shown */
    if(timestamp - elapsed_t > delta_t) {
        /* Get the maximum value of the histogram */
        auto max_value = histdiff.maxCoeff();
        auto min_value = histdiff.minCoeff();
        if(min_value < 0){
            min_value*=-1;
            if(min_value > max_value) max_value = min_value;
        }
        for(uint32_t index = 0; index < (height-1)*width + (width-1); index++){
            if(histdiff(index) == 0) continue;
            else{
                if(histdiff(index) >= 0){
                    histdiff_img.data[index*3] = (uint8_t)((double)histdiff(index)/(double)max_value * 255);    /* BLUE CHANNEL*/
                    histdiff_img.data[index*3+1] = 0;                                                           /* GREEN CHANNEL */
                    histdiff_img.data[index*3+2] = 0;                                                           /* RED CHANNEL */
                }
                else {
                    histdiff_img.data[index*3] = 0;                                                             /* BLUE CHANNEL*/
                    histdiff_img.data[index*3+1] = 0;                                                           /* GREEN CHANNEL */
                    histdiff_img.data[index*3+2] = (uint8_t)(-(double)histdiff(index)/(double)max_value * 255); /* RED CHANNEL */
                }
            }
        }
        /* Print images */
        cv::imshow("Histogram Diff", histdiff_img); 
        /* Reset images */
        memset(histdiff_img.data, 0, (height-1)*width*3 + (width-1)*3+2); 
        histdiff = Eigen::ArrayXi::Zero(height*width + width);
        elapsed_t = timestamp;
        }
}