#include <FaceTracker/faceTracker.h>
FaceTracker::FaceTracker()
{
    //load ftFile, conFile and triFile
    std::strcpy(ftFile,"../model/face2.tracker");
    std::strcpy(conFile,"../model/face.con");
    std::strcpy(triFile,"../model/face.tri");
    model.Load(ftFile);
    tri = FACETRACKER::IO::LoadTri(triFile);
    con = FACETRACKER::IO::LoadCon(conFile);
}

std::vector<cv::Point> FaceTracker::getPoints(cv::Mat img)
{
    std::vector<cv::Point> res;
    /*Return points in an image*/
    //Parameters
    bool fcheck = false; double scale = 1; int fpd = -1; bool show = false;
    std::vector<int> wSize1(1); wSize1[0] = 7;
    std::vector<int> wSize2(3); wSize2[0] = 11; wSize2[1] = 9; wSize2[2] = 7;
    int nIter = 5; double clamp=3,fTol=0.01; 
    cv::Mat gray,im;   std::string text;
    bool failed = true;

    //grab image, resize and flip
    //if(!I)continue; 
    if(scale == 1)im = img; 
    else cv::resize(img,im,cv::Size(scale*img.cols,scale*img.rows));
    //cv::flip(im,im,1); 
    cv::cvtColor(im,gray,CV_BGR2GRAY);

    //track this image
    std::vector<int> wSize; if(failed)wSize = wSize2; else wSize = wSize1; 
    if(model.Track(gray,wSize,fpd,nIter,clamp,fTol,fcheck) == 0){   //If track
      int idx = model._clm.GetViewIdx(); 
      failed = false;
      int n = model._shape.rows/2;
      for(int i = 0; i < n; i++){    
          if(model._clm._visi[idx].at<int>(i,0) == 0) continue;
          res.push_back(cv::Point(model._shape.at<double>(i,0), model._shape.at<double>(i+n,0)));
      }

      //draw(im,model._shape,con,tri,model._clm._visi[idx]); 
    }else{
      if(show){cv::Mat R(im,cvRect(0,0,150,50)); R = cv::Scalar(0,0,255);}
      model.FrameReset(); failed = true;
    }     
    return res;
}

void FaceTracker::draw(cv::Mat &image,cv::Mat &shape,cv::Mat &con,cv::Mat &tri,cv::Mat &visi)
{
  int i,n = shape.rows/2; cv::Point p1,p2; cv::Scalar c;

  ////draw triangulation
  //c = CV_RGB(0,0,0);
  //for(i = 0; i < tri.rows; i++){
  //  if(visi.at<int>(tri.at<int>(i,0),0) == 0 ||
  //     visi.at<int>(tri.at<int>(i,1),0) == 0 ||
  //     visi.at<int>(tri.at<int>(i,2),0) == 0)continue;
  //  p1 = cv::Point(shape.at<double>(tri.at<int>(i,0),0),
  //  	   shape.at<double>(tri.at<int>(i,0)+n,0));
  //  p2 = cv::Point(shape.at<double>(tri.at<int>(i,1),0),
  //  	   shape.at<double>(tri.at<int>(i,1)+n,0));
  //  //cv::line(image,p1,p2,c);
  //  p1 = cv::Point(shape.at<double>(tri.at<int>(i,0),0),
  //  	   shape.at<double>(tri.at<int>(i,0)+n,0));
  //  p2 = cv::Point(shape.at<double>(tri.at<int>(i,2),0),
  //  	   shape.at<double>(tri.at<int>(i,2)+n,0));
  //  //cv::line(image,p1,p2,c);
  //  p1 = cv::Point(shape.at<double>(tri.at<int>(i,2),0),
  //  	   shape.at<double>(tri.at<int>(i,2)+n,0));
  //  p2 = cv::Point(shape.at<double>(tri.at<int>(i,1),0),
  //  	   shape.at<double>(tri.at<int>(i,1)+n,0));
  //  //cv::line(image,p1,p2,c);
  //}
  ////draw connections
  //c = CV_RGB(0,0,255);
  //for(i = 0; i < con.cols; i++){
  //  if(visi.at<int>(con.at<int>(0,i),0) == 0 ||
  //     visi.at<int>(con.at<int>(1,i),0) == 0)continue;
  //  p1 = cv::Point(shape.at<double>(con.at<int>(0,i),0),
  //  	   shape.at<double>(con.at<int>(0,i)+n,0));
  //  p2 = cv::Point(shape.at<double>(con.at<int>(1,i),0),
  //  	   shape.at<double>(con.at<int>(1,i)+n,0));
  //  //cv::line(image,p1,p2,c,1);
  //}
  //draw points
  for(i = 0; i < n; i++){    
    if(visi.at<int>(i,0) == 0)continue;
    p1 = cv::Point(shape.at<double>(i,0),shape.at<double>(i+n,0));
    std::cout<<p1<<std::endl;
    c = CV_RGB(255,0,0); cv::circle(image,p1,2,c);
  }
  return;
}

