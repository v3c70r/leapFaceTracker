#include <frameReader.h>

void ImgListener::onConnect(const Leap::Controller& controller){
    std::cout<<"Connected"<<std::endl;
    controller.setPolicy(Leap::Controller::POLICY_IMAGES);

    cvImgs[0].create(240, 640, CV_8UC1);
    cvImgs[1].create(240, 640, CV_8UC1);
    interpImgs[0].create(480, 640, CV_8UC1);
    interpImgs[1].create(480, 640, CV_8UC1);
    undistortedImgs[0].create(480, 640, CV_8UC3);
    undistortedImgs[1].create(480, 640, CV_8UC3);




}

void ImgListener::onFrame(const Leap::Controller& controller)
{
    Leap::Frame frame = controller.frame();
    Leap::ImageList images = frame.images();
    for (int i=0; i<2; i++)
    {

        Leap::Image image = images[i];
        if (image.isValid()){
            memcpy(cvImgs[i].data, image.data(), sizeof(uchar)*image.height()*image.width()*image.bytesPerPixel());
            cv::resize(cvImgs[i], interpImgs[i], interpImgs[i].size());
            cv::Mat temp;
            cv::undistort(interpImgs[i], temp, cameraMatLeft, distCoefLeft);
            cv::cvtColor(temp,undistortedImgs[i],CV_GRAY2RGB);

        }


        pVecMutex->lock();

        cameras[0]->clearRay();
        cameras[1]->clearRay();
        std::vector<cv::Point> leftPoints = ft.getPoints(undistortedImgs[0]);
        std::vector<cv::Point> rightPoints = ft.getPoints(undistortedImgs[1]);

        for (int i=0; i<leftPoints.size(); i++){
            cv::circle(undistortedImgs[0], leftPoints[i], 2, 255);
            cameras[0]->addRay(leftPoints[i].x, leftPoints[i].y);
        }
        for (int i=0; i<leftPoints.size(); i++){

            cv::circle(undistortedImgs[1], rightPoints[i], 2, 255);
            cameras[1]->addRay(rightPoints[i].x, rightPoints[i].y);
        }
        std::cout<<cameras[0]->numOfRays()<<std::endl;
        std::cout<<cameras[1]->numOfRays()<<std::endl;
        pVecMutex->unlock();

        namedWindow( "leftImage", cv::WINDOW_AUTOSIZE );
        cv::imshow("leftImage", undistortedImgs[0]);

        namedWindow( "rightImage", cv::WINDOW_AUTOSIZE );
        cv::imshow("rightImage", undistortedImgs[1]);
        cv::waitKey(10);
    }
}

