#include <opencv2/opencv.hpp>
#include <inttypes.h>

using namespace std;

template<int32_t width, int32_t height, int64_t delta_t>
class EventFrame{
    public:
        /* Constructor */
        EventFrame();
        /* Show Events */
        void ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp);
    private:
        /* Image to show */
        cv::Mat img;
        /* Variables */
        int64_t elapsed_t = 0;
};

template<int32_t width, int32_t height, int64_t delta_t>
EventFrame<width, height, delta_t>::EventFrame(void){
    cv::Mat tmp(height,width,CV_8UC3, cv::Scalar(130,130,130));
    img = tmp;
    cv::imshow("Event Frame", img);
}

template<int32_t width, int32_t height, int64_t delta_t>
void EventFrame<width, height, delta_t>::ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp){

    if(p){
    /* Build the image */
    img.data[y*width*3 + x*3] = (int) 255;      /* BLUE CHANNEL */
    img.data[y*width*3 + x*3 + 1] = (int) 0;    /* GREEN CHANNEL */
    img.data[y*width*3 + x*3 + 2] = (int) 0;    /* RED CHANNEL */
    }
    else {
    /* Build the image */
    img.data[y*width*3 + x*3] = (int) 0;        /* BLUE CHANNEL */
    img.data[y*width*3 + x*3 + 1] = (int) 0;    /* GREEN CHANNEL */
    img.data[y*width*3 + x*3 + 2] = (int) 255;  /* RED CHANNEL */
    }

    // check if image must be displayed
    if(timestamp - elapsed_t> delta_t) {
        cv::imshow("Event Frame", img); //print the image
        memset(img.data, 130, (height-1)*width*3 + (width-1)*3+2); //reset the image
        elapsed_t = timestamp;
    }
}