
/*Find correspondence point in each frame*/
#ifndef FACETRACKER_H
#define FACETRACKER_H
#include <FaceTracker/Tracker.h>
#include <opencv/highgui.h>
#include <iostream>

class FaceTracker
{
private:
    FACETRACKER::Tracker model;
    char ftFile[256],conFile[256],triFile[256];
    cv::Mat tri;
    cv::Mat con;
public:
    FaceTracker();
    std::vector<cv::Point> getPoints(cv::Mat img);
    void draw(cv::Mat &image,cv::Mat &shape,cv::Mat &con,cv::Mat &tri,cv::Mat &visi);
};
    
#endif
