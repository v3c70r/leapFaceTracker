/*
 * Read frame data from stdin and convert frames into IplImage
 */

#ifndef FRAMEREADER_H
#define FRAMEREADER_H

#include<FaceTracker/faceTracker.h>
#include <opencv2/opencv.hpp>
#include <Leap.h>
#include <mutex>
#include <camera.h>

const cv::Mat cameraMatLeft = (cv::Mat_<float>(3, 3) << 138.8489, 0.0, 338.5789, 0, 138.1636, 235.8967, 0.0, 0.0, 1.000);
const cv::Mat cameraMatRight = (cv::Mat_<float>(3, 3) << 138.5785, 0.0, 326.8120, 0, 138.5308, 233.6496, 0.0, 0.0, 1.000);

const cv::Mat distCoefLeft = (cv::Mat_<float>(1, 4) << 0.04616, -0.01554, -0.00209, 0.00064);
const cv::Mat distCoefRight = (cv::Mat_<float>(1, 4) << 0.01361, -0.00621, -0.00397, -0.00129);

class ImgListener: public Leap::Listener{
        cv::Mat cvImgs[2];
        cv::Mat interpImgs[2];
        cv::Mat undistortedImgs[2];
        FaceTracker ft;
        std::mutex *pVecMutex;
        Camera *cameras[2];


    public:
        virtual void onConnect(const Leap::Controller&) override;
        virtual void onFrame(const Leap::Controller&) override;
        void setMutex(std::mutex *m) {pVecMutex = m;}
        void setCamera(Camera *left, Camera* right) {cameras[0] = left; cameras[1] = right;}

};
#endif
