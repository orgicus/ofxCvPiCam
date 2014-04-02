/*
 *  ofxCvPiCam.cpp
 *  openFrameworksLib
 *
 *  Created by jason van cleave on 5/16/13.
 *  Copyright 2013 jasonvancleave.com. All rights reserved.
 *
 */

#include "ofxCvPiCam.h"

using namespace cv;
using namespace std;

int ofxCvPiCam::width = 0;
int ofxCvPiCam::height = 0;
MMAL_POOL_T * ofxCvPiCam::camera_video_port_pool = NULL;
Mat ofxCvPiCam::image = Mat();

static void color_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
    MMAL_BUFFER_HEADER_T *new_buffer;
    mmal_buffer_header_mem_lock(buffer);
    unsigned char* pointer = (unsigned char *)(buffer -> data);
    int w = ofxCvPiCam::width, h = ofxCvPiCam::height;
    Mat y(h, w, CV_8UC1, pointer);
    pointer = pointer + (h*w);
    Mat u(h/2, w/2, CV_8UC1, pointer);
    pointer = pointer + (h*w/4);
    Mat v(h/2, w/2, CV_8UC1, pointer);
    mmal_buffer_header_mem_unlock(buffer);
    mmal_buffer_header_release(buffer);
    if (port->is_enabled) {
        MMAL_STATUS_T status;
        new_buffer = mmal_queue_get(ofxCvPiCam::camera_video_port_pool->queue);
        if (new_buffer)
            status = mmal_port_send_buffer(port, new_buffer);
        if (!new_buffer || status != MMAL_SUCCESS)
            ofLogVerbose() << "Unable to return a buffer to the video port\n";
    }
    Mat image(h, w, CV_8UC3);
    resize(u, u, Size(), 2, 2, INTER_LINEAR);
    resize(v, v, Size(), 2, 2, INTER_LINEAR);
    int from_to[] = {0, 0};
    mixChannels(&y, 1, &image, 1, from_to, 1);
    from_to[1] = 1;
    mixChannels(&v, 1, &image, 1, from_to, 1);
    from_to[1] = 2;
    mixChannels(&u, 1, &image, 1, from_to, 1);
    cvtColor(image, image, CV_YCrCb2BGR);
    ofxCvPiCam::set_image(image);
}
static void gray_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
    MMAL_BUFFER_HEADER_T *new_buffer;
    mmal_buffer_header_mem_lock(buffer);
    unsigned char* pointer = (unsigned char *)(buffer -> data);
    ofxCvPiCam::set_image(Mat(ofxCvPiCam::height, ofxCvPiCam::width, CV_8UC1, pointer));
    mmal_buffer_header_release(buffer);
    if (port->is_enabled) {
        MMAL_STATUS_T status;
        new_buffer = mmal_queue_get(ofxCvPiCam::camera_video_port_pool->queue);
        if (new_buffer)
            status = mmal_port_send_buffer(port, new_buffer);
        if (!new_buffer || status != MMAL_SUCCESS)
            ofLogVerbose() << ("Unable to return a buffer to the video port\n");
    }
}

void ofxCvPiCam::setup(int _w,int _h,bool _color)
{
    color = _color;
    width = _w;
    height = _h;
    camera = 0;
    preview = 0;
    camera_preview_port = NULL;
    camera_video_port = NULL;
    camera_still_port = NULL;
    preview_input_port = NULL;
    camera_preview_connection = 0;
    bcm_host_init();
    status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &camera);
    if (status != MMAL_SUCCESS) {
        ofLog(OF_LOG_ERROR, "Error: creating camera \n");
    }
    camera_preview_port = camera->output[MMAL_CAMERA_PREVIEW_PORT];
    camera_video_port = camera->output[MMAL_CAMERA_VIDEO_PORT];
    camera_still_port = camera->output[MMAL_CAMERA_CAPTURE_PORT];
    {
        MMAL_PARAMETER_CAMERA_CONFIG_T cam_config = {{ MMAL_PARAMETER_CAMERA_CONFIG, sizeof (cam_config)}, width, height, 0, 0,width, height, 3, 0, 1, MMAL_PARAM_TIMESTAMP_MODE_RESET_STC };
        mmal_port_parameter_set(camera->control, &cam_config.hdr);
    }
    format = camera_video_port->format;
    format->encoding = MMAL_ENCODING_I420;
    format->encoding_variant = MMAL_ENCODING_I420;

    format->es->video.width = width;
    format->es->video.height = height;
    format->es->video.crop.x = 0;
    format->es->video.crop.y = 0;
    format->es->video.crop.width = width;
    format->es->video.crop.height = height;
    format->es->video.frame_rate.num = 30;
    format->es->video.frame_rate.den = 1;
    camera_video_port->buffer_size = width * height * 3 / 2;
    camera_video_port->buffer_num = 1;
    status = mmal_port_format_commit(camera_video_port);
    if (status != MMAL_SUCCESS) {
        //printf("Error: unable to commit camera video port format (%u)\n", status);
        ofLog(OF_LOG_ERROR, "Error: unable to commit camera video port format \n");
    }
    // create pool form camera video port
    camera_video_port_pool = (MMAL_POOL_T *) mmal_port_pool_create(camera_video_port,
    camera_video_port->buffer_num, camera_video_port->buffer_size);
    if(color) {
        status = mmal_port_enable(camera_video_port, color_callback);
        if (status != MMAL_SUCCESS)
            ofLog(OF_LOG_ERROR, "Error: unable to enable camera video port \n");
        else
            ofLogVerbose() << "Attached color callback" << endl;
    }
    else {
        status = mmal_port_enable(camera_video_port, gray_callback);
        if (status != MMAL_SUCCESS)
            ofLog(OF_LOG_ERROR, "Error: unable to enable camera video port \n");
        else
            ofLogVerbose() << "Attached gray callback" << endl;
    }
    status = mmal_component_enable(camera);
    // Send all the buffers to the camera video port
    int num = mmal_queue_length(camera_video_port_pool->queue);
    int q;
    for (q = 0; q < num; q++) {
        MMAL_BUFFER_HEADER_T *buffer = mmal_queue_get(camera_video_port_pool->queue);
        if (!buffer) {
            ofLogVerbose() << "Unable to get a required buffer " << q << " from pool queue\n";
        }
        if (mmal_port_send_buffer(camera_video_port, buffer) != MMAL_SUCCESS) {
            ofLogVerbose() << "Unable to send a buffer to encoder output port " << q;
        }
    }
    if (mmal_port_parameter_set_boolean(camera_video_port, MMAL_PARAMETER_CAPTURE, 1) != MMAL_SUCCESS) {
        //printf("%s: Failed to start capture\n", __func__);
        ofLog(OF_LOG_ERROR, "Failed to start capture");
    }
    cout << "Capture started" << endl;
}
ofxCvPiCam::ofxCvPiCam()
{
    ofLogVerbose() << "ofxCvPiCam()";
}
ofxCvPiCam::~ofxCvPiCam()
{
    ofLogVerbose() << "~ofxCvPiCam";
}
