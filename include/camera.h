#ifndef CAMERA_H
#define CAMERA_H
#include <vector>
#include <armadillo>
#include <GL/freeglut.h>
#include <vector>
using namespace std;
using namespace arma;

class Camera
{
private:
    //Extrinsic params
    mat rotationMatrix;
    mat translationMatrix;
    double transMatrix[16];

    //Intrinsice params
    mat cameraMatrix;
    
    GLuint displayList;
    const GLfloat size;
    const double width;
    const double height;
    vector<vec> rays;   //A vector store the normalized set of ray dirs
    vec normalize(vec input);
    int raySize;

public:
static mat buildRotationMatrix(double w, double x, double y, double z) {
        double c = cos(w);
        double s = sin(w);
        
        mat res = 
        {
            x*x+c*(1-x*x), x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0,
            x*y*(1-c)+z*s, y*y+c*(1-y*y), z*y*(1-c)-x*s, 0,
            x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z+c*(1-z*z), 0,
            0, 0, 0, 1
        };
        res.reshape(4,4);
        return res.t();
    }
    static mat buildTranslationMatrix(double x, double y, double z)
    {
        mat res=
        {
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1
        };
        res.reshape(4,4);
        return res.t();
    }

    static vec findNearestPoint(vector<vec> line1, vector<vec>line2)
    {
        //vec p_0 = {line1[0](0), line1[0](1), line1[0](2)};
        //vec q_0 = {line2[0](0), line2[0](1), line2[0](2)};
        vec p_0 = line1[0];
        vec q_0 = line2[0];
        vec u = line1[1];
        vec v = line2[1];


        vec w_0 = p_0 - q_0;
        double s_c = (dot(u,v)*dot(v,w_0)-dot(u,w_0))/(1-dot(u,v)*dot(u,v));
        double t_c = (dot(v,w_0)-dot(u,v)*dot(u, w_0))/(1-dot(u, v)*dot(u, v));
        //cout<<"s_c="<<s_c<<endl;

        vec p_s = q_0 + t_c * v;
        vec q_c = p_0 + s_c * u;
        return (p_s + q_c)/2;

    }

    Camera():size(5),width(640),height(480){
    raySize = 0;
    };

    void clearRay();
    void drawRays() const;//Draw a ray
    void addRay(double u, double v);
    void draw() const;
    void init();
    void setMatrices(mat rm, mat tm, mat cm);
    int numOfRays() const{ return rays.size();}
    vector<vec> drawQuad(int i, int j);    //Draw a quad with first two rays
    ~Camera();

    void drawRay();

    vector<vec> getWCRay(int n);


    

};
#endif
