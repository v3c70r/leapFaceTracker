#ifndef FRAME_H
#define FRAME_H
#include <opencv2/opencv.hpp>
using namespace cv;
class Frame
{
public:
   cv::Mat leftFrame;
   cv::Mat rightFrame;
   Frame(int width, int height)
   {
       std::cout<<"Creating Frame\n";
       leftFrame.create(240*2, 640, CV_8U);
       rightFrame.create(240*2, 640, CV_8U);
       std::cout<<"Done\n";
   }

};
#endif
