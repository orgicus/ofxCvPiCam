#include "testApp.h"
#include <opencv2/opencv.hpp>

using namespace ofxCv;
using namespace cv;

Mat frame,frameProcessed;

//--------------------------------------------------------------
void testApp::setup(){
    namedWindow("test");
    cam.setup(320,240,true);//setup camera (w,h,color = true,gray = false);
    thresh = 127;
    ROI.set(0,0,320,240);
    gui.setup("PiCam settings");
    gui.setPosition(0,240);
    gui.setWidthElements(320);
    gui.add(saturation.set("saturation",0,-100,100));
    gui.add(sharpness.set("sharpness",0,-100,100));
    gui.add(contrast.set("contrast",0,-100,100));
    gui.add(brightness.set("brightness",50,0,100));
    gui.add(ISO.set("ISO",300,100,800));
    gui.add(exposureCompensation.set("exposure compensation",0,-10,10));
    gui.add(exposureMeteringMode.set("exposure metering mode",0,0,4));
    gui.add(exposureMode.set("exposure mode",0,0,13));
    gui.add(shutterSpeed.set("shutter speed",0,0,330000));//(in micro seconds)
    gui.add(awbMode.set("AutoWhiteBalance mode",0,0,10));
    gui.add(awbGainR.set("AutoWhiteBalance Rgain",0,0,1));
    gui.add(awbGainB.set("AutoWhiteBalance Bgain",0,0,1));

    guiXtra.setup("PiCam extra settings");
    guiXtra.setPosition(320,240);
    guiXtra.setWidthElements(320);
    guiXtra.add(vstabilisation.set("vstabilisation",false));
    guiXtra.add(rotation.set("rotation",0,0,359));
    guiXtra.add(hflip.set("flip horizontal",false));
    guiXtra.add(vflip.set("flip vertical",false));
    guiXtra.add(roiX.set("ROI x",0,0,1));
    guiXtra.add(roiY.set("ROI y",0,0,1));
    guiXtra.add(roiW.set("ROI w",1,0,1));
    guiXtra.add(roiH.set("ROI h",1,0,1));
    guiXtra.add(imageFX.set("image effect",0,0,23));

    saturation.addListener(this,&testApp::saturationChanged);
    sharpness.addListener(this,&testApp::sharpnessChanged);
    contrast.addListener(this,&testApp::contrastChanged);
    brightness.addListener(this,&testApp::brightnessChanged);
    ISO.addListener(this,&testApp::ISOChanged);
    vstabilisation.addListener(this,&testApp::vstabilisationChanged);
    exposureCompensation.addListener(this,&testApp::exposureCompensationChanged);
    rotation.addListener(this,&testApp::rotationChanged);
    hflip.addListener(this,&testApp::hflipChanged);
    vflip.addListener(this,&testApp::vflipChanged);
    roiX.addListener(this,&testApp::roiXChanged);
    roiY.addListener(this,&testApp::roiYChanged);
    roiW.addListener(this,&testApp::roiWChanged);
    roiH.addListener(this,&testApp::roiHChanged);
    exposureMeteringMode.addListener(this,&testApp::exposureMeteringModeChanged);
    exposureMode.addListener(this,&testApp::exposureModeChanged);
    awbMode.addListener(this,&testApp::awbModeChanged);
    awbGainR.addListener(this,&testApp::awbGainRChanged);
    awbGainB.addListener(this,&testApp::awbGainBChanged);
    imageFX.addListener(this,&testApp::imageFXChanged);

    exposureMeteringModes[0] = "average";
    exposureMeteringModes[1] = "spot";
    exposureMeteringModes[2] = "backlit";
    exposureMeteringModes[3] = "matrix";
    exposureMeteringModes[4] = "max";

    exposureModes[ 0] = "off";
    exposureModes[ 1] = "auto";
    exposureModes[ 2] = "night";
    exposureModes[ 3] = "night preview";
    exposureModes[ 4] = "backlight";
    exposureModes[ 5] = "spotlight";
    exposureModes[ 6] = "sports";
    exposureModes[ 7] = "snow";
    exposureModes[ 8] = "beach";
    exposureModes[ 9] = "very long";
    exposureModes[10] = "fixed fps";
    exposureModes[11] = "antishake";
    exposureModes[12] = "fireworks";
    exposureModes[13] = "max";

    awbModes[ 0] = "off";
    awbModes[ 1] = "auto";
    awbModes[ 2] = "sunlight";
    awbModes[ 3] = "cloudy";
    awbModes[ 4] = "shade";
    awbModes[ 5] = "tungsten";
    awbModes[ 6] = "fluorescent";
    awbModes[ 7] = "incandescent";
    awbModes[ 8] = "flash";
    awbModes[ 9] = "horizon";
    awbModes[10] = "max";

    imageFXLabels[ 0] = "none";
    imageFXLabels[ 1] = "negative";
    imageFXLabels[ 2] = "solarize";
    imageFXLabels[ 3] = "posterize";
    imageFXLabels[ 4] = "whiteboard";
    imageFXLabels[ 5] = "blackboard";
    imageFXLabels[ 6] = "sketch";
    imageFXLabels[ 7] = "denoise";
    imageFXLabels[ 8] = "emboss";
    imageFXLabels[ 9] = "oilpaint";
    imageFXLabels[10] = "hatch";
    imageFXLabels[11] = "gpen";
    imageFXLabels[12] = "pastel";
    imageFXLabels[13] = "whatercolour";
    imageFXLabels[14] = "film";
    imageFXLabels[15] = "blur";
    imageFXLabels[16] = "saturation";
    imageFXLabels[17] = "colour swap";
    imageFXLabels[18] = "washedout";
    imageFXLabels[19] = "posterize";
    imageFXLabels[20] = "colour point";
    imageFXLabels[21] = "colour balance";
    imageFXLabels[22] = "cartoon";
    imageFXLabels[23] = "max";

}

//--------------------------------------------------------------
void testApp::update(){
    frame = cam.grab();
}

//--------------------------------------------------------------
void testApp::draw(){
    if(!frame.empty()) {
        imshow("test",frame);
        threshold(frame,frameProcessed,thresh,255,0);
        drawMat(frame,0,0);
        drawMat(frameProcessed,320,0);
    }
    gui.draw();
    guiXtra.draw();
}

//settings
void testApp::saturationChanged(int &value){
    cam.setSaturation(value);
}
void testApp::sharpnessChanged(int &value){
    cam.setSharpness(value);
}
void testApp::contrastChanged(int &value){
    cam.setContrast(value);
}
void testApp::brightnessChanged(int &value){
    cam.setBrightness(value);
}
void testApp::ISOChanged(int &value){
    cam.setISO(value);
}
void testApp::vstabilisationChanged(bool &value){
    cam.setVideoStabilisation(value);
}
void testApp::exposureCompensationChanged(int &value){
    cam.setExposureCompensation(value);
}
void testApp::exposureMeteringModeChanged(int &value){
    exposureMeteringMode.setName(exposureMeteringModes[value]);                            //display the preset name in the UI
    if(value == exposureMeteringMode.getMax()) value = MMAL_PARAM_EXPOSUREMETERINGMODE_MAX;//the preset max value is different from the UI
    cam.setExposureMeteringMode((MMAL_PARAM_EXPOSUREMETERINGMODE_T)value);
}
void testApp::exposureModeChanged(int &value){
    exposureMode.setName(exposureModes[value]);//display the preset name in the UI
    if(value == exposureMode.getMax()) value = MMAL_PARAM_EXPOSUREMODE_MAX;//the preset max value is different from the UI
    cam.setExposureMode((MMAL_PARAM_EXPOSUREMODE_T)value);
}
void testApp::awbModeChanged(int &value){
    awbMode.setName(awbModes[value]);//display the preset name in the UI
    if(value == awbMode.getMax()) value = MMAL_PARAM_AWBMODE_MAX;//the preset max value is different from the UI
    cam.setAWBMode((MMAL_PARAM_AWBMODE_T)value);
}
void testApp::imageFXChanged(int &value){
    imageFX.setName(imageFXLabels[value]);//display the preset name in the UI
    if(value == imageFX.getMax()) value = MMAL_PARAM_IMAGEFX_MAX;//the preset max value is different from the UI
    cam.setImageFX((MMAL_PARAM_IMAGEFX_T)value);
}
void testApp::shutterSpeedChanged(int &value){
    cam.setShutterSpeed(value);
}
void testApp::rotationChanged(int &value){
    cam.setRotation(value);
}
void testApp::hflipChanged(bool &value){
    cam.setFlips(value,vflip.get());
}
void testApp::vflipChanged(bool &value){
    cam.setFlips(hflip.get(),value);
}
void testApp::roiXChanged(float &value){
    ROI.x = value;
    cam.setROI(ROI);
}
void testApp::roiYChanged(float &value){
    ROI.y = value;
    cam.setROI(ROI);
}
void testApp::roiWChanged(float &value){
    ROI.width = value;
    cam.setROI(ROI);
}
void testApp::roiHChanged(float &value){
    ROI.height = value;
    cam.setROI(ROI);
}
void testApp::awbGainRChanged(float &value){
    cam.setAWBGains(value,awbGainB.get());
}
void testApp::awbGainBChanged(float &value){
    cam.setAWBGains(awbGainR.get(),value);
}
