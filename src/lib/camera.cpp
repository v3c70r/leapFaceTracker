#include <camera.h>



vec Camera::normalize(vec input)
{
    return input/norm(input, 2);
}

Camera::~Camera()
{
    glDeleteLists(displayList, 1);
}

void Camera::init()
{

    vec p0 = {0 , 0, -2.0 * size};
    vec p1 = { size, size, 0};
    vec p2 = { -size, size, 0};
    vec p3 = { size, -size, 0};
    vec p4 = { -size, -size, 0};

    vec n1 = cross(p3-p1, p0 - p1);
    vec n2 = cross(p1-p2, p0 - p2);
    vec n3 = cross(p4-p2, p0 - p2);
    vec n4 = cross(p3-p4, p0 - p4);

    displayList = glGenLists(1);

    glNewList(displayList, GL_COMPILE);
    glBegin(GL_TRIANGLES);
        glColor3f(0.9f, 0.9f, 0.9f);
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-size, size, 0.0);
        glVertex3f(-size, -size, 0.0);
        glVertex3f(size, size, 0.0);

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(size, size, 0.0);
        glVertex3f(-size, -size, 0.0);
        glVertex3f(size, -size, 0.0);

        
        glNormal3d(n1(0), n1(1), n1(2));  //first face
        glVertex3f(size, size, 0.0f);
        glVertex3f(size, -size, 0.0f);
        glVertex3f(0, 0, -2.0*size);

        glNormal3d(n2(0), n2(1), n2(2));  //second face
        glVertex3f(size, size, 0.0f);
        glVertex3f(-size, size, 0.0f);
        glVertex3f(size, size, 0.0f);
        glVertex3f(0, 0, -2.0*size);
        
        glNormal3f(n3(0), n3(1), n3(2));  //third face
        glVertex3f(-size, size, 0.0f);
        glVertex3f(-size, -size, 0.0f);
        glVertex3f(0, 0, -2.0*size);

        glNormal3f(n4(0), n4(1), n4(2));  //fourth face
        glVertex3f(-size, -size, 0.0f);
        glVertex3f(size, -size, 0.0f);
        glVertex3f(0, 0, -2.0*size);

    glEnd();

    //Axis
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f,0.0f);
        glVertex3f(3.0*size, 0.0, 0.0);
        glVertex3f(-3.0*size, 0.0, 0.0);

        glColor3f(0.0f, 1.0f,0.0f);
        glVertex3f(0.0,3.0*size , 0.0);
        glVertex3f(0.0,-3.0*size , 0.0);

        glColor3f(0.0f, 0.0f,1.0f);
        glVertex3f(0.0,0,3.0*size );
        glVertex3f(0.0,0,-3.0*size );
    glEnd();
    glEndList(); 
    raySize = 0;
}

void Camera::addRay(double u, double v)
{
    vec dir = {(u-cameraMatrix(0,2))/cameraMatrix(0,0),
                (v-cameraMatrix(1,2))/cameraMatrix(1,1),
                1
    };
    dir = dir/norm(dir,2);
    rays.push_back(-1*dir);
    raySize++;
}

void Camera::clearRay()
{
    rays.clear();
    raySize = 0;
}
void Camera::drawRays() const
{
    //Transform to x, y based stuff
    //double x =  u - width/2; 
    //double y = v - width/2;

    //double prinX = cameraMatrix(0,2) - width/2;
    //double priny = cameraMatrix(1,2) - height/2;
    //find a point at ( (u-c_x)/f_x, (v - c_y)/f_y, 1)
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < (int)rays.size(); i++)
    {
    vec dir = 500*rays[i];

    glPushMatrix();
    glMultMatrixd(transMatrix);
    glBegin(GL_LINES);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(dir(0), dir(1), dir(2));
    glEnd();

    glPopMatrix();
    }
    

}

void Camera::draw() const
{
    glPushMatrix();
    glMultMatrixd(transMatrix);
    glCallList(displayList);
    glPopMatrix();
}

void Camera::setMatrices(mat rm, mat tm, mat cm)
{
    rotationMatrix = rm;
    translationMatrix = tm;
    cameraMatrix = cm;


    mat translationM = rotationMatrix * translationMatrix;
    for (int i=0; i < 16; i++)
            transMatrix[i] = translationM(i); 
}

vector<vec> Camera::getWCRay(int n)
{
    //Return the ray P_0 + s_U of nth ray
    vector<vec> res;
    if (n > (int)rays.size())
        exit(0);
    vec origin = zeros(4);
    origin(3) = 1;
    origin = rotationMatrix * translationMatrix * origin;
    vec resOrigin = {origin(0), origin(1), origin(2)};

    res.push_back(resOrigin);
    vec temp = {rays[n](0), rays[n](1), rays[n](2), 1};


    vec dir = rotationMatrix * translationMatrix * temp - origin;
    vec resDir = {dir(0), dir(1), dir(2)} ;
    resDir = resDir/norm(resDir, 2);
    res.push_back(resDir);
    return res;
}

vector<vec> Camera::drawQuad(int i, int j)
{
    const double near = 10;
    const double far = 500;
    vector<vec> res;

    vector<vec> ray1 = this->getWCRay(i);
    vector<vec> ray2 = this->getWCRay(j);
    vec near1 = ray1[0]+near*ray1[1];
    vec near2 = ray2[0]+near*ray2[1];

    vec far1 = ray1[0]+far*ray1[1];
    vec far2 = ray2[0]+far*ray2[1];



    res.push_back(near2);
    res.push_back(near1);
    res.push_back(far1);
    res.push_back(far2);

    glBegin(GL_QUADS);
        for (unsigned i=0; i < res.size(); i++)
        {
            glVertex3d(res[i](0), res[i](1), res[i](2));
            //cout<<res[i](0)<<res[i](1)<<res[i](2)<<std::endl;
        }
    glEnd();
    return res;
}
