// http://cs.lmu.edu/~ray/notes/openglexamples/
// this code was taken from the above location and heavily edited
// the code was previously a ball boucing rendering, this is included
// locally as balls.cpp

#define PI 3.14159265358979323846

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>
#include <iostream>
#include <fstream>

#define EULER 1
#define MOD_EULER 2
#define RK4 3

// Colors
GLfloat WHITE[] = {1, 1, 1};
GLfloat RED[] = {1, 0, 0};
GLfloat BLUE[] = {0, 0, 1};
GLfloat GREEN[] = {0, 1, 0};
GLfloat MAGENTA[] = {1, 0, 1};

// render updater
bool r_update = true;
int count = 0;
double h = 0.9; // the global stepsize

// A camera.  It moves horizontally in a circle centered at the origin of
// radius 10.  It moves vertically straight up and down.
class Camera {
  double theta;      // determines the x and z positions
  double y;          // the current y position
  double dTheta;     // increment in theta for swinging the camera around
  double dy;         // increment in y for moving the camera up/down
public:
  Camera(): theta(-1.5), y(4), dTheta(0.04), dy(0.2) {}
  double getX() {return 6 * cos(theta);}
  double getY() {return y;}
  double getZ() {return 6 * sin(theta);}
  void moveRight() {theta += dTheta;}
  void moveLeft() {theta -= dTheta;}
  void moveUp() {y += dy;}
  void moveDown() {if (y > dy) y -= dy;}
};

// A checkerboard class.  A checkerboard has alternating red and white
// squares.  The number of squares is set in the constructor.  Each square
// is 1 x 1.  One corner of the board is (0, 0) and the board stretches out
// along positive x and positive z.  It rests on the xz plane.  I put a
// spotlight at (5, 6, 7).
class Checkerboard {
  int displayListId;
  int width;
  int depth;
public:
  Checkerboard(int width, int depth): width(width), depth(depth) {}
  double centerx() {return width / 2;}
  double centerz() {return depth / 2;}

  void create() {
    displayListId = glGenLists(1);
    glNewList(displayListId, GL_COMPILE);
    GLfloat lightPosition[] = {15, 15, 6, 1}; // NOTE change this if needed
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glBegin(GL_QUADS);
    glNormal3d(0, 1, 0);
    for (int x = 0; x < width - 1; x++) {
      for (int z = 0; z < depth - 1; z++) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,(x + z) % 2 == 0 ? RED : WHITE);
        glVertex3d(x, 0, z);
        glVertex3d(x+1, 0, z);
        glVertex3d(x+1, 0, z+1);
        glVertex3d(x, 0, z+1);
      }
    }
    glEnd();
    glEndList();
  }

  void draw() {
    glCallList(displayListId);
  }
};


// My attempt at making a cart-pole like thing
// starts with the pendulum
// Started by modifying the ball class
// remember, the plane is x+ and z+
class CartPole {
  // double radius;
  GLfloat* color;
  // double maximumHeight;
  double x;
  double y;
  double z;
  double length;
  // int direction;
public:

  double m_1;
  double m_2;

  double x_dot;

  double F;
  double g;

  double angle; // from down
  double v;
  double mass;
  int updateNumb = 0;
  int updateGoal = 2;

  CartPole(GLfloat* c, double len, double x_i, double y_i, double z_i, double v_i, double x_d, double theta):
           color(c), length(len), x(x_i), y(y_i), z(z_i), v(v_i), x_dot(x_d), angle(theta) {}

  void rotatePendulum(double theta){
    //
    // Rotate the arm
    //
    glPushMatrix(); // begin arm matrix
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    // go to main point
    glTranslated(x, y, z);
    // for rotation...
    glTranslated(0.0, length/2, 0.0);
    // rotate at translation point, around the z-vector
    glRotated(theta,0.0,0.0,1.0);
    // now go back...
    glTranslated(0.0, -length/2, 0.0);
    // scale rect.p. to correct sizeof
    glScaled(0.05,(length),0.05);
    // generate the cube
    glutSolidCube(1.0);
    glPopMatrix(); // end arm matrix
    //
    // Rotate the ball
    //
    glPushMatrix(); // begin ball matrix
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    glTranslated(x, y-(length/2), z); // initial position
    // for rotation
    glTranslated(0.0, length, 0.0);
    // rotate at translation point, around the z-vector
    glRotated(theta,0.0,0.0,1.0);
    glTranslated(0.0, -length, 0.0);
    // make the sphere
    glutSolidSphere(0.1, 30, 30);
    glPopMatrix(); // end ball matrix
  }

  void updateSystem(double theta, double x_n){
    //
    // Rotate and traslate the arm
    //
    glPushMatrix(); // begin arm matrix
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    // go to main point
    glTranslated(x, y, z);
    // for rotation...
    glTranslated(0.0, length/2, 0.0);
    // rotate at translation point, around the z-vector
    glRotated(theta,0.0,0.0,1.0);
    // now go back...
    glTranslated(0.0, -length/2, 0.0);


    // scale rect.p. to correct sizeof
    glScaled(0.05,(length),0.05);
    // generate the cube
    glutSolidCube(1.0);
    glPopMatrix(); // end arm matrix


    //
    // Rotate and traslate the ball
    //
    glPushMatrix(); // begin ball matrix
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(x, y-(length/2), z); // initial position
    // for rotation
    glTranslated(0.0, length, 0.0);
    // rotate at translation point, around the z-vector
    glRotated(theta,0.0,0.0,1.0);
    glTranslated(0.0, -length, 0.0);

    // TODO translate to the correct location

    // make the sphere
    glutSolidSphere(0.1, 30, 30);
    glPopMatrix(); // end ball matrix


    //
    // translate the cart
    //
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MAGENTA);

    glTranslated(x, y + (length/2), z + 0.275);

    glScaled(1.0,0.5,0.5);

    glutSolidCube(1.0);
    glPopMatrix();

  }

  // just a plain rendering
  void render(){
    updateSystem(0.0,0.0);
  }

  void step(int i){
    switch(i){
      case EULER:
        euler();
        // logState("euler.csv");
        break;
      case MOD_EULER:
        modified_euler();
        // logState("modified_euler.csv");
        break;
      default:
        updateSystem(angle,x);
    }
  }

  void logState(std::string str){
    std::ofstream myfile;
    myfile.open(str,std::ios_base::app);
    myfile << count << "," << angle << "," << x << "\n";// << v << "," << x_dot << ",";
    myfile.close();
  }

  double Fx(double x_3, double x_4){
    // general terms
    m_1 = 1200.0;
    m_2 = 2.0;
    F = 0.0;
    g = 9.81;
    double mu = -0.05; // dampening factor

    // broken into parts to help make it readable / a bit more clear
    double alpha = ((cos(x_3 * PI/180.0) * cos(x_3 * PI/180.0) * m_2 * length) - length * (m_1 + m_2));
    double term1 = -1 * g * sin(x_3 * PI/180.0) * m_2 * length * cos(x_3 * PI/180.0);
    double term2 = -1 * length * (F + m_2 * length * (x_4 * PI/180.0) * (x_4 * PI/180.0) * sin(x_3 * PI/180.0));
    double result = ((term1 + term2)/alpha) + mu * x_dot; // TODO add friction here

    return result;
  }

  double Gx(double x_3, double x_4){
    // general terms
    m_1 = 1200.0;
    m_2 = 2.0;
    F = 0.0;
    g = 9.81;
    double mu = -0.05; // dampening factor

    // broken into parts to help make it readable / a bit more clear
    double alpha = ((cos(x_3 * PI/180.0) * cos(x_3 * PI/180.0) * m_2 * length) - length * (m_1 + m_2));
    double term1 = -1 * (m_1 + m_2) * (-1 * g * sin(x_3 * PI/180.0));
    double term2 = cos(x_3 * PI/180.0) * (F + m_2 * length * (x_4 * PI/180.0) * (x_4 * PI/180.0) * sin(x_3 * PI/180.0));
    double result = ((term1 + term2)/alpha) + mu * x_4; // This removes energy from the system

    return result;
  }

  // this is the kinematic step
  void euler(){
    // this is just the step size
    double h = 0.5;

    std::cout << "Euler, step: " << h*count << std::endl;

    // The form of the X vector, with the
    // variables being used in this class, is:
    //      |   x   |   | x_1 |          |     x_2      |
    //  X = | x_dot | = | x_2 | ... X' = | Fx(x_3, x_4) |
    //      | angle |   | x_3 |          |     x_4      |
    //      |   v   |   | x_4 |          | Gx(x_3, x_4) |

    // Simple Euler Method
    double x1 = x     + (h * x_dot);
    double x2 = x_dot + (h * Fx(angle,v));
    double x3 = angle + (h * v);
    double x4 = v     + (h * Gx(angle,v));

    // single euler method here
    x     = x1;
    x_dot = x2;
    angle = x3;
    v     = x4;

    std::cout << "X = [ " << x << "  " << x_dot << "  " << angle << "  " << v << "]" << std::endl;
    // this is for rendering the update
    updateSystem(angle,x);
  }

  void modified_euler(){
    // this is just the step size
    double h = 0.5;

    std::cout << "Improved Euler, step: " << h*count << std::endl;

    // The form of the X vector, with the
    // variables being used in this class, is:
    //      |   x   |   | x_1 |          |     x_2      |
    //  X = | x_dot | = | x_2 | ... X' = | Fx(x_3, x_4) |
    //      | angle |   | x_3 |          |     x_4      |
    //      |   v   |   | x_4 |          | Gx(x_3, x_4) |

    // Improved Euler Method
    // curr
    double K1_x1 = x     + (h * x_dot);
    double K1_x2 = x_dot + (h * Fx(angle,v));
    double K1_x3 = angle + (h * v);
    double K1_x4 = v     + (h * Gx(angle,v));
    // next
    double K2_x1 = (h * K1_x2);
    double K2_x2 = (h * Fx(K1_x3,K1_x4));
    double K2_x3 = (h * K1_x4);
    double K2_x4 = (h * Gx(K1_x3,K1_x4));

    // average
    x     = (K1_x1 + K2_x1)/2.0;
    x_dot = (K1_x2 + K2_x2)/2.0;
    angle = (K1_x3 + K2_x3)/2.0;
    v     = (K1_x4 + K2_x4)/2.0;

    std::cout << "X = [ " << x << "  " << x_dot << "  " << angle << "  " << v << "]" << std::endl;
    // this is for rendering the update
    updateSystem(angle,x);
  }

};



// Global variables: a camera, a checkerboard and some balls.
Checkerboard checkerboard(9, 9);

Camera camera;

// attempt at doing a pole like thing
// GLfloat* c, double len, double x_i, double y_i, double z_i, double v_i, double x_d, double theta
// color,   length,   x init,  y init,   z init, intial rot vel,  initial x vel,   intitial angle
CartPole cartpole1(GREEN,   2.0, 3.0, 2.0, 2.0, 0.0, 0.0, 90.0);
CartPole cartpole2(RED,     2.0, 3.0, 2.0, 3.0, 0.0, 0.0, 90.0);


// Application-specific initialization: Set up global lighting parameters
// and create display lists.
void init() {
  glEnable(GL_DEPTH_TEST);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
  glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
  glMaterialf(GL_FRONT, GL_SHININESS, 30);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  checkerboard.create();
}

// Draws one frame, the checkerboard then the balls, from the current camera
// position.
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(camera.getX(), camera.getY(), camera.getZ(), 4.0, 1.0, 4.0, 0.0, 1.0, 0.0);
  checkerboard.draw();

  // step foward with the carts
  cartpole1.step(EULER);
  cartpole2.step(MOD_EULER);

  count++;

  glFlush();
  glutSwapBuffers();
}

// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
  glMatrixMode(GL_MODELVIEW);
}

// Requests to draw the next frame.
void timer(int v) {
  glutPostRedisplay();
  // was 1000/60
  glutTimerFunc(1000/60, timer, v);
}

// Moves the camera according to the key pressed, then ask to refresh the
// display.
void special(int key, int, int) {
  switch (key) {
    case GLUT_KEY_LEFT: camera.moveLeft(); break;
    case GLUT_KEY_RIGHT: camera.moveRight(); break;
    case GLUT_KEY_UP: camera.moveUp(); break;
    case GLUT_KEY_DOWN: camera.moveDown(); break;
  }
  glutPostRedisplay();
}

// Initializes GLUT and enters the main loop.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(1000, 800);
  glutCreateWindow("Cart Pole Simulation");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutTimerFunc(100, timer, 0);
  init();
  glutMainLoop();
}
