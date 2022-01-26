#include <metavision/sdk/driver/camera.h>
#include <metavision/sdk/base/events/event_cd.h>
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "histogramdiff.h"

//defines
#define width 640
#define height 480
#define fps 25
#define delta_t 40000

HistogramDiff<width, height, delta_t> histogram;

using namespace std::chrono;

// this class will be used to analyze the events
class EventAnalyzer {
public:
    // variables
    Metavision::timestamp recording_duration = 0;

    // functions
    Metavision::timestamp get_recording_duration(void);

    // this function will be associated to the camera callback
    // it is used to compute statistics on the received events
    void analyze_events(const Metavision::EventCD *begin, const Metavision::EventCD *end) {
        for (const Metavision::EventCD *ev = begin; ev != end; ++ev) {
            histogram.ShowEvents(ev->x, ev->y, ev->p, ev->t);
            recording_duration = ev->t;
        }
    }
};

Metavision::timestamp EventAnalyzer::get_recording_duration(void){return recording_duration;}

// main loop
int main(int argc, char *argv[]) {

    EventAnalyzer event_analyzer; // create the event analyzer
    Metavision::Camera cam = Metavision::Camera::from_file(argv[1]); // create the camera from raw file

    // add the event callback. This callback will be called periodically to provide access to the most recent events
    cam.cd().add_callback([&event_analyzer](const Metavision::EventCD *ev_begin, const Metavision::EventCD *ev_end) {
        event_analyzer.analyze_events(ev_begin, ev_end);
    });
  
    // start the camera
    cam.start();

    // start the clock to get the process time
    high_resolution_clock::time_point init_cam = high_resolution_clock::now();

    // keep running while the camera is on or the video is not finished
    while (cam.is_running()) {
        // if the user presses the `q` key, quit the loop
        if ((cv::waitKey(25) & 0xff) == 'q') {
            break;
        }
    }

    // get the time
    high_resolution_clock::time_point finish_cam = high_resolution_clock::now();
    //stop the camera
    cam.stop();

    // calculate the process time
    duration<double> process_duration = duration_cast<duration<double>>(finish_cam - init_cam); 

    // print process time and recording time
    std::cout<<"Process took: " << process_duration.count() << " seconds \n";
    std::cout<<"Recording took: "<< (float)event_analyzer.get_recording_duration()/1000000 << " seconds\n";

}