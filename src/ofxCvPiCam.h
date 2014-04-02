/**
* @author George Profenza ,based on code from:
* @author Samarth Manoj Brahmbhatt, University of Pennsyalvania -> cv::Mat wrapper
* @author Tasanakorn  -> opencv_demo.c
* @author Jason Van Cleave (jvcleave) -> captureApplication mmal openframeworks demo
**/
#pragma once

#include "ofMain.h"

#include "interface/mmal/mmal.h"
#include "interface/mmal/mmal_logging.h"
#include "interface/mmal/mmal_buffer.h"
#include "interface/mmal/util/mmal_util.h"
#include "interface/mmal/util/mmal_util_params.h"
#include "interface/mmal/util/mmal_default_components.h"
#include "interface/mmal/util/mmal_connection.h"

#include <opencv2/opencv.hpp>


#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT   1
#define MMAL_CAMERA_CAPTURE_PORT 2

class ofxCvPiCam
{
public:
    ofxCvPiCam();
    ~ofxCvPiCam();
    void setup();

    static cv::Mat image;
    static int width, height;
    static MMAL_POOL_T *camera_video_port_pool;
    static void set_image(cv::Mat _image) {image = _image;}
    void setup(int _w, int _h, bool _color);
    cv::Mat grab() {return image;}
    MMAL_COMPONENT_T *getCamera(){ return camera; }

private:
    bool color;
    MMAL_COMPONENT_T *camera;
    MMAL_COMPONENT_T *preview;
    MMAL_ES_FORMAT_T *format;
    MMAL_STATUS_T status;
    MMAL_PORT_T *camera_preview_port, *camera_video_port, *camera_still_port;
    MMAL_PORT_T *preview_input_port;
    MMAL_CONNECTION_T *camera_preview_connection;

};

static void color_callback(MMAL_PORT_T *, MMAL_BUFFER_HEADER_T *);
static void gray_callback(MMAL_PORT_T *, MMAL_BUFFER_HEADER_T *);

