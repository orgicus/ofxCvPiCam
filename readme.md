# Introduction

ofxCvPiCam is a minimal grabber for the Raspberry Pi Camera module.
It uses the MMAL interface to grab frames and convert them the cv::Mat type
which can be processed using the typical opencv calls.

I warmly recommend using [Kyle McDonalds' ofxCv](https://github.com/kylemcdonald/ofxCv) addon: 
it makes writing modern opencv code so much easier. The example included uses ofxCv's ```drawMat()```
to display cv::Mat objects in OpenFrameworks. You can of course use your prefered method.

# Installation

Either clone out the source code using git:

	> cd openFrameworks/addons/
	> git clone https://github.com/orgicus/ofxCvPiCam.git

Or download the source from GitHub [here](https://github.com/orgicus/ofxCvPiCam/archive/master.zip), unzip the folder, rename it from `ofxCv-master` to `ofxCv` and place it in your `openFrameworks/addons` folder.


## Working with ofxCvPiCam

First initialize the grabber by passing the width,height and color mode(boolean: color = true, grayscale = false).
Use the ```grab()``` method fetch a cv::Mat frame then carry on processing it

*Note* sometimes frames can be skipped so it's best to check if the returned matrix is not empty.

## Current issues

- imshow displays colour(BGR) frame correctly, drawMat seems to swap channels
- there is currently no camera settings interface

## Credits
This wrapper is mostly done by putting together the work of
[Samarth Manoj Brahmbhatt](http://www.seas.upenn.edu/~samarthb/academics/index.html), University of Pennsyalvania -> cv::Mat wrapper listed in his title: Practical OpenCV
[Jason Van Cleave](https://github.com/jvcleave) -> captureApplication mmal openframeworks demo