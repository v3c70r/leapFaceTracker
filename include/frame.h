#ifndef FRAME_H
#define FRAME_H
#include <cv.h>
using namespace cv;
class Frame
{
public:
   IplImage* leftFrame;
   IplImage* rightFrame;
   Frame(int width, int height)
   {
       leftFrame = cvCreateImage( cvSize(width, height *2),IPL_DEPTH_8U, 3);
       rightFrame = cvCreateImage( cvSize(width, height *2),IPL_DEPTH_8U, 3);
   }

};
#endif
