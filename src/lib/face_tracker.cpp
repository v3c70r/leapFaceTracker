#include <FaceTracker/Tracker.h>
#include <opencv/highgui.h>
#include<FaceTracker/faceTracker.h>
#include <frameReader.h>
#include <iostream>
#include <GL/freeglut.h>
#include <stdio.h>
#include <gvector3d.h>
#include <gmatrix3d.h>
#include <ggltools.h>
#include <camera.h>
#include <splitstring.h>
#include <armadillo>
#include <fstream>
#include <string>
#include <sstream>
#include <pthread.h>
#include <thread>

using namespace arma;
using namespace std;
////=============================================================================
cv::Mat cameraMatLeft = (Mat_<float>(3, 3) << 138.8489, 0.0, 338.5789, 0, 138.1636, 235.8967, 0.0, 0.0, 1.000);
cv::Mat cameraMatRight = (Mat_<float>(3, 3) << 138.5785, 0.0, 326.8120, 0, 138.5308, 233.6496, 0.0, 0.0, 1.000);

cv::Mat distCoefLeft = (Mat_<float>(1, 4) << 0.04616, -0.01554, -0.00209, 0.00064);
cv::Mat distCoefRight = (Mat_<float>(1, 4) << 0.01361, -0.00621, -0.00397, -0.00129);
FrameReader fr(stdin);
std::mutex vecMutex;


//=============================================================================
//ArchBall value
int gMouseX = -1;
int gMouseY = -1;

double zoom = 150.0;
//
bool gIsStartTrackBall = false;
bool isMoving = false;
bool displayRays = false;

GMatrix3d gRotMatrix;
GMatrix3d translationMatrix;

Camera leftCamera;
Camera rightCamera;

//Put this in a thread
void faceTrack()
{


    int key;
    FaceTracker ft;
    while(!feof(stdin))
    {
        Frame f = fr.readFrame();
        cv::Mat leftMat(f.leftFrame);
        cv::Mat rightMat(f.rightFrame);
        cv::Mat outLeft;
        cv::Mat outRight;
        undistort(leftMat, outLeft, cameraMatLeft, distCoefLeft);
        undistort(rightMat, outRight, cameraMatRight, distCoefRight);

        //Get points
        //std::cout<<cv::countNonZero(outLeft);
        //std::cout<<cv::countNonZero(outRight);
        //
        //vecMutex.lock();
        //std::cout<<"Adding Rays"<<std::endl;
        //std::cout<<leftCamera.numOfRays()<<std::endl;
        //std::cout<<leftCamera.numOfRays()<<std::endl;
        //leftCamera.addRay(10, 10);
        //rightCamera.addRay(10, 10);
        //leftCamera.addRay(10, 10);
        //rightCamera.addRay(10, 10);
        //vecMutex.unlock();
        
        vecMutex.lock();
        leftCamera.clearRay();
        rightCamera.clearRay();

        std::vector<cv::Point> leftPoints = ft.getPoints(outLeft);
        std::vector<cv::Point> rightPoints = ft.getPoints(outRight);
        //Draw points on frames

        for (int i=0; i<leftPoints.size(); i++){
            cv::circle(outLeft, leftPoints[i], 2, CV_RGB(255, 0, 0));
            leftCamera.addRay(leftPoints[i].x, leftPoints[i].y);
        }

        for (int i=0; i<rightPoints.size(); i++){
            cv::circle(outRight, rightPoints[i], 2, CV_RGB(255, 0, 0));
            rightCamera.addRay(rightPoints[i].x, rightPoints[i].y);
        }
        //std::cout<<leftCamera.numOfRays()<<std::endl;
        //std::cout<<leftCamera.numOfRays()<<std::endl;

        vecMutex.unlock();

        imshow("left",outLeft);
        imshow("right", outRight);
        //cvShowImage("left", leftMat);
        //cvShowImage("right", rightMat);
        key = cvWaitKey(0);
        if (key == 'q' || key == 0x1B)
            break;

    }
    //return NULL;
}

GVector3d gMousePtToSphereVec(int x, int y, int w, int h)
{
    GVector3d vec;
    vec.setX((2.0*x-w)/w);
    vec.setY(-(h-2.0*y)/h); 
    double r=vec.x()*vec.x()+vec.y()*vec.y();
    if(r>1)r=1;
    vec.setZ(sqrt(1-r));
    vec.normalize();
    return vec;
}
//Initialiation 

int width = 1024;
int height = 768;
void init()
{
    //OpenGL initialize functions goes here

    glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);

    cout<<"Vendor: "<<glGetString(GL_VENDOR)<<endl;
    cout<<"Version: "<<glGetString(GL_VERSION)<<endl;
    cout<<"GLSL: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<endl;

      //Light values and coordinates
     GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
     GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
     GLfloat lightPos[] = {20.0f, 20.0f, 50.0f};
     glEnable(GL_DEPTH_TEST);
     glFrontFace(GL_CCW);
     //glEnable(GL_CULL_FACE);
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     // Hidden surface removal // Counterclockwise polygons face out // Do not calculate inside of jet // Enable lighting
     glEnable(GL_LIGHTING);
     // Set up and enable light 0
     glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
     glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
     glEnable(GL_LIGHT0);
     // Enable color tracking
     glEnable(GL_COLOR_MATERIAL);
     // Set material properties to follow glColor values
     glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

     glClearColor(0.2f, 0.2f, 0.2f, 3.0f );

    
     //Rescale normals to unit length
     glEnable(GL_NORMALIZE);
     glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

     glShadeModel(GL_FLAT);

     mat leftCameraMatrix={
          138.8489, 0, 338.5789,
         0, 138.1636,235.8967, 
         0, 0 ,1.0000
     };
     leftCameraMatrix.reshape(3,3);
     leftCameraMatrix = leftCameraMatrix.t();


     leftCamera.setMatrices(eye(4,4), eye(4,4), leftCameraMatrix);


     //Rotation matrix
     mat rotMat = {
         0.999,     0.0021,     0.0169,     0.0,
         -0.0022,   1.0000,     0.0051,     0.0,
         -0.0169,   -0.0051,    0.9998,     0.0,
         0.0,   0.0,    0.0,    1.0

     };
     rotMat.reshape(4,4);
     rotMat = rotMat.t();

     //Camera matrices
     mat rightCameraMatrix={
       138.5785, 0, 326.8120,
         0, 138.5308, 233.6496,
         0, 0,1.0000
     };
     rightCameraMatrix.reshape(3,3);
     rightCameraMatrix = rightCameraMatrix.t();


     rightCamera.setMatrices(rotMat, Camera::buildTranslationMatrix(-40.30745,1.54703,-2.04556),rightCameraMatrix);
     //rightCamera.setMatrices(eye(4,4), eye(4,4), eye(4,4));

     leftCamera.init();
     rightCamera.init();
}
void drawWirePlane(GLfloat width)
{
    glColor3f(0.6f, 0.6f, 0.6f);
    for(float u=-width; u <= width; u+=width/50)
    {
        glBegin(GL_LINES);
        glVertex3f(-width, 0, u);
        glVertex3f(width, 0, u);
        
        glVertex3f(u, 0, -width);
        glVertex3f(u, 0, width);
        glEnd();
    }
}
//Display function
void display()
{

    vecMutex.lock();
    //std::cout<<leftCamera.numOfRays()<<std::endl;
    //std::cout<<rightCamera.numOfRays()<<std::endl;


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //gluLookAt(0.0f, 40.0f, -150.f, 0.0f, 40.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -zoom);
    glMultMatrixd(translationMatrix);  
    glMultMatrixd(gRotMatrix);  //Arch ball


    glPushMatrix();
    glTranslatef(0.0f, -80.0f, 0.0f);
    drawWirePlane(200.0);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    leftCamera.draw();
    rightCamera.draw();

    for ( int i=0; i < leftCamera.numOfRays(); i++)
    {
    glPushMatrix();
        vec tempVec = Camera::findNearestPoint(leftCamera.getWCRay(i), rightCamera.getWCRay(i));
        glTranslated(tempVec(0), tempVec(1), tempVec(2));
        glutSolidSphere(1, 10, 10);
    glPopMatrix();
    }
    //std::cout<<"++++++++++++++++++\n";
    vecMutex.unlock();

    if (displayRays)
    {
        leftCamera.drawRays();
        rightCamera.drawRays();
    }
    glPopMatrix();
    
    glRotatef(180.0, 0.0, 0.0, 1.0);
    glPopMatrix();


    glutSwapBuffers();


}

void mouseMoveEvent(int x, int y)
{
    if(gIsStartTrackBall)
    {
        if(x!=gMouseX||y!=gMouseY)
        {
            int w=glutGet(GLUT_WINDOW_WIDTH);
            int h=glutGet(GLUT_WINDOW_HEIGHT);
            GVector3d lastVec,currentVec,axis;
            double rotAngle;
            lastVec=gMousePtToSphereVec(gMouseX,h-gMouseY,w,h);
            currentVec=gMousePtToSphereVec(x,h-y,w,h);
            axis=lastVec.crossMult(currentVec);
            axis.normalize();
            rotAngle=acos(lastVec.dotMult(currentVec));
            rotAngle*=RADIAN_TO_ANGLE;
            gMouseX=x;gMouseY=y;

         //
            GMatrix3d m=GMatrix3d::createRotate(rotAngle,axis);
            gRotMatrix *=m;

            //glutPostRedisplay();
        }

    }
    if (isMoving)       //Moving the scence
    {
        if(x!=gMouseX||y!=gMouseY)
        {
            //int w=glutGet(GLUT_WINDOW_WIDTH);
            //int h=glutGet(GLUT_WINDOW_HEIGHT);

            int xTranslation = x - gMouseX;
            int yTranslation = gMouseY - y;

            //std::cout<<xTranslation<<"\t"<<yTranslation<<std::endl;


            GMatrix3d m = GMatrix3d::createTranslate((double)xTranslation*0.01, (double)yTranslation*0.01, 0.0);
            translationMatrix *=m;
            
        }
    }
}

void mouseEvent(int button, int state, int x, int y)
{
    if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            gMouseX = x;
            gMouseY = y;
            gIsStartTrackBall = true;
        }
        else if(state == GLUT_UP)
        {    
            gIsStartTrackBall = false;
        }            
    }
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            gMouseX = x;
            gMouseY = y;
            isMoving = true;
        }
        else if(state == GLUT_UP)
        {    
            isMoving = false;
        }            
    }
}       

void handleKeyPress(int key, int x, int y)
{ 
    switch(key)
    {
        case GLUT_KEY_UP:
            zoom -= 3.0f;
            break;
        case GLUT_KEY_DOWN:
            zoom += 3.0f;
            break;
        case GLUT_KEY_RIGHT:
            displayRays=false;
            break;
        case GLUT_KEY_LEFT:
            displayRays=true;
            break;
        case GLUT_KEY_F1:
        default:
            break;

            //Zoom out
    }
}

//void changeSize(int w, int h)
//{
//    //call while window changed
//    width = w;
//    height = h;
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(0.0, width, 0.0, height);
//}
//
void changeSize(int w, int h)
{
    GLfloat aspectRatio;


    //prevent a divide by zero
    if(h==0)
        h = 1;
    //Set Viewport
    glViewport(0, 0, w, h);

    //reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Establish clipping voluem(left, right, bottom, top, near, far)
    aspectRatio = (GLfloat)w / (GLfloat)h;
    /*
    if(w<=h)
        glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 * aspectRatio, 100.0, -100.0);
    else
        glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 100.0, -100.0);
        */
    gluPerspective(45.0f, aspectRatio, 1.0f, 900.0f);    //using perspective
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void timerFunction(int value)       
{
    //force it to refresh, no need of postRedisplay function any more
    glutPostRedisplay();
    glutTimerFunc(10,timerFunction,1);
}


int main(int argc, char **argv)
{
    //Init Opengl
    glutInit(&argc, argv);
    //Print contex info
    

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	//double buffer
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Camera Calib");
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutTimerFunc(10, timerFunction, 1);

    glutMouseFunc(mouseEvent);
    glutMotionFunc(mouseMoveEvent);
    glutSpecialFunc(handleKeyPress);
    init();
    thread first(faceTrack);
    //thread second(glutMainLoop);

    glutMainLoop();
    cout<<"Running in two threads\n";
    first.join();
    //second.join();
    return 0;
}

