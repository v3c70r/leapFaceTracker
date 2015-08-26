/*
 * Read frame data from stdin and convert frames into IplImage
 */

#ifndef FRAMEREADER_H
#define FRAMEREADER_H

#include <frame.h>
#include <cv.h>
#include <Leap.h>

class FrameReader
{
private:
    const int FRAME_WIDTH;
    const int FRAME_HEIGHT;
    const int FRAME_SIZE;
    FILE *f;
    unsigned char usbBuf[16384];
    bool quit;
    Leap::Controller ctrl;
public:
    FrameReader(FILE *file):FRAME_WIDTH(640),FRAME_HEIGHT(240),FRAME_SIZE(640*240),f(file)
    {
        quit = false;
    }
    Frame readFrame();   //Read a frame from in
    Frame readFrameSDK();
    void exit(){ quit = true;}

};
#endif
