/**
* @author George Profenza ,based on code from:
* @author http://github.com/raspberrypi/userland
* @author Samarth Manoj Brahmbhatt, University of Pennsyalvania -> cv::Mat wrapper
* @author Tasanakorn  -> opencv_demo.c
* @author Jason Van Cleave (jvcleave) -> captureApplication mmal openframeworks demo
**/
#pragma once

#include "ofMain.h"

#include "interface/mmal/mmal.h"
#include "interface/mmal/mmal_logging.h"
#include "interface/mmal/mmal_buffer.h"
#include "interface/mmal/mmal_parameters_camera.h"
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
    typedef struct
    {
       int enable;       /// Turn colourFX on or off
       int u,v;          /// U and V to use
    } MMAL_PARAM_COLOURFX_T;
    typedef struct
    {
       double x;
       double y;
       double w;
       double h;
    } PARAM_FLOAT_RECT_T;

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

    //settings
    //*
    int setSaturation(int saturation);
    int setSharpness(int sharpness);
    int setContrast(int contrast);
    int setBrightness(int brightness);
    int setISO(int ISO);
    int setExposureMeteringMode(MMAL_PARAM_EXPOSUREMETERINGMODE_T m_mode);
    int setVideoStabilisation(int vstabilisation);
    int setExposureCompensation(int exp_comp);
    int setExposureMode(MMAL_PARAM_EXPOSUREMODE_T mode);
    int setAWBMode(MMAL_PARAM_AWBMODE_T awb_mode);//TODO
    int setAWBGains(float r_gain,float b_gain);//TODO
    int setImageFX(MMAL_PARAM_IMAGEFX_T imageFX);//TODO example
    int setColourFX(MMAL_PARAM_COLOURFX_T *colourFX);//TODO example
    int setRotation(int rotation);
    int setFlips(int hflip,int vflip);
//    int setROI(PARAM_FLOAT_RECT_T rect);
    int setROI(ofRectangle rect);
    int setShutterSpeed(int speed);//TODO

    int mmal_status_to_int(MMAL_STATUS_T status)
    {
       if (status == MMAL_SUCCESS)
          return 0;
       else
       {
          switch (status)
          {
          case MMAL_ENOMEM :   ofLogError("Out of memory"); break;
          case MMAL_ENOSPC :   ofLogError("Out of resources (other than memory)"); break;
          case MMAL_EINVAL:    ofLogError("Argument is invalid"); break;
          case MMAL_ENOSYS :   ofLogError("Function not implemented"); break;
          case MMAL_ENOENT :   ofLogError("No such file or directory"); break;
          case MMAL_ENXIO :    ofLogError("No such device or address"); break;
          case MMAL_EIO :      ofLogError("I/O error"); break;
          case MMAL_ESPIPE :   ofLogError("Illegal seek"); break;
          case MMAL_ECORRUPT : ofLogError("Data is corrupt \attention FIXME: not POSIX"); break;
          case MMAL_ENOTREADY :ofLogError("Component is not ready \attention FIXME: not POSIX"); break;
          case MMAL_ECONFIG :  ofLogError("Component is not configured \attention FIXME: not POSIX"); break;
          case MMAL_EISCONN :  ofLogError("Port is already connected "); break;
          case MMAL_ENOTCONN : ofLogError("Port is disconnected"); break;
          case MMAL_EAGAIN :   ofLogError("Resource temporarily unavailable. Try again later"); break;
          case MMAL_EFAULT :   ofLogError("Bad address"); break;
          default :            ofLogError("Unknown status error"); break;
          }

          return 1;
       }
    }
    //*/
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
