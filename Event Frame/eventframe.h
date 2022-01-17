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
    cv::Mat tmp(height,width,CV_8UC1, cv::Scalar(130));
    img = tmp;
    cv::imshow("Display Events", img);
}

template<int32_t width, int32_t height, int64_t delta_t>
void EventFrame<width, height, delta_t>::ShowEvents(uint16_t x, uint16_t y, int16_t p, int64_t timestamp){

    /* Colour of the pixel */
    uint8_t colour = 0;
    colour = p ? 255 : 0;

    /* Build the image */
    img.data[y*width + x] = colour; // y * width * (nº channels) + x * (nº channels) + channel

    // check if image must be showed
    if(timestamp - elapsed_t> delta_t) {
        cv::imshow("Display Events", img); //print the image
        memset(img.data, 130, (height-1)*width*1 + (width-1)*1); //reset the image
        elapsed_t = timestamp;
    }
}