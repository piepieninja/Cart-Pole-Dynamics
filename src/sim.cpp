// http://cs.lmu.edu/~ray/notes/openglexamples/

// This application shows balls bouncing on a checkerboard, with no respect
// for the laws of Newtonian Mechanics.  There's a little spotlight to make
// the animation interesting, and arrow keys move the camera for even more
// fun.

#define PI 3.14159265358979323846

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>


// Colors
GLfloat WHITE[] = {1, 1, 1};
GLfloat RED[] = {1, 0, 0};
GLfloat BLUE[] = {0, 0, 1};
GLfloat GREEN[] = {0, 1, 0};
GLfloat MAGENTA[] = {1, 0, 1};

// A camera.  It moves horizontally in a circle centered at the origin of
// radius 10.  It moves vertically straight up and down.
class Camera {
  double theta;      // determines the x and z positions
  double y;          // the current y position
  double dTheta;     // increment in theta for swinging the camera around
  double dy;         // increment in y for moving the camera up/down
public:
  Camera(): theta(-1), y(3), dTheta(0.04), dy(0.2) {}
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
    GLfloat lightPosition[] = {5, 6, 7, 1}; // NOTE change this if needed
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glBegin(GL_QUADS);
    glNormal3d(0, 1, 0);
    for (int x = 0; x < width - 1; x++) {
      for (int z = 0; z < depth - 1; z++) {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                     (x + z) % 2 == 0 ? RED : WHITE);
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
class Pendulum {
  // double radius;
  GLfloat* color;
  // double maximumHeight;
  double x;
  double y;
  double z;
  double length;
  // int direction;
public:

  double angle; // from down
  double v;
  double mass;
  int updateNumb = 0;
  int updateGoal = 2;

  Pendulum(GLfloat* c, double len, double x_i, double y_i, double z_i, double v_i):
           color(c), length(len), x(x_i), y(y_i), z(z_i), v(v_i) {}

  void rotate(double theta){
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

  // this is the kinematic step
  void update(){
    if (updateNumb == updateGoal){
      // damping factor is 0.005
      double acc = (-0.005 * v) - (9.81 / length) * sin(angle * PI/180.0);
      v += acc;
      angle = angle + v;
      rotate(angle);
      updateNumb = 0;
    } else {
      rotate(angle); // render but don't change
      updateNumb++;
    }
  }

};



// Global variables: a camera, a checkerboard and some balls.
Checkerboard checkerboard(9, 9);

Camera camera;

// attempt at doing a pole like thing
Pendulum pendulum1(RED, 2.0, 4.0, 2.0, 4.0, 20.0);
Pendulum pendulum2(GREEN, 2.0, 4.0, 2.0, 4.5, 22.0);
Pendulum pendulum3(BLUE,  2.0, 4.0, 2.0, 5.0, 24.0);
Pendulum pendulum4(RED, 2.0, 4.0, 2.0, 5.5, 26.0);
Pendulum pendulum5(GREEN, 2.0, 4.0, 2.0, 6.0, 28.0);
Pendulum pendulum6(BLUE,  2.0, 4.0, 2.0, 6.5, 30.0);



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
  gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
            4.0, 1.0, 4.0,
            0.0, 1.0, 0.0);
  checkerboard.draw();
  // double temp = pendulum.angle + 0.1;
  // pendulum.rotate(temp);
  // pendulum.angle  = temp;
  pendulum1.update();
  pendulum2.update();
  pendulum3.update();
  pendulum4.update();
  pendulum5.update();
  pendulum6.update();
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
  glutInitWindowSize(800, 600);
  glutCreateWindow("Cart Pole Simulation");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutTimerFunc(100, timer, 0);
  init();
  glutMainLoop();
}
