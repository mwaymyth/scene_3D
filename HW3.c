/*
 *  Scene of 3D object
 *
 *  Demonstrates how to draw objects in 3D.
 *
 *  Key bindings:
 *  a          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdarg.h>
 #include <math.h>
 //  OpenGL with prototypes for glext
 #define GL_GLEXT_PROTOTYPES
 #ifdef __APPLE__
 #include <GLUT/glut.h>
 #else
 #include <GL/glut.h>
 #endif

 int th=0;         //  Azimuth of view angle
 int ph=0;         //  Elevation of view angle
 double zh=0;      //  Rotation of teapot
 int axes=1;       //  Display axes
 int mode=0;       //  What to display



 /*
  *  Convenience routine to output raster text
  *  Use VARARGS to make this more flexible
  */
 #define LEN 8192  //  Maximum length of text string
 void Print(const char* format , ...)
 {
    char    buf[LEN];
    char*   ch=buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args,format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
 }

 /*
  *  Draw a house
  *     at (x,y,z)
  *     dimensions (dx,dy,dz)
  *     rotated th about the y axis
  */
 static void house(double x,double y,double z,
                  double dx,double dy,double dz,
                  double th)
 {
   // Dimensions for house
   double wd = 1;
   double ht = 1;

   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-wd,-ht, 1);
   glVertex3f(wd,-ht, 1);
   glVertex3f(wd,ht, 1);
   glVertex3f(-wd,ht, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(wd,-ht,-1);
   glVertex3f(-wd,-ht,-1);
   glVertex3f(-wd,ht,-1);
   glVertex3f(wd,ht,-1);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(wd,-ht,+1);
   glVertex3f(wd,-ht,-1);
   glVertex3f(wd,ht,-1);
   glVertex3f(wd,ht,+1);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-wd,-ht,-1);
   glVertex3f(-wd,-ht,+1);
   glVertex3f(-wd,ht,+1);
   glVertex3f(-wd,ht,-1);
   //  Roof Right
   glColor3f(0,1,1);
   glVertex3f(wd+0.5,ht-0.5,+1);
   glVertex3f(wd+0.5,ht-0.5,-1);
   glVertex3f(0,+2,-1);
   glVertex3f(0,+2,+1);
   //  Roof Left
   glColor3f(0,1,1);
   glVertex3f(-wd -0.5,ht-0.5,-1);
   glVertex3f(-wd -0.5,ht-0.5,+1);
   glVertex3f(0,+2,+1);
   glVertex3f(0,+2,-1);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-wd,-ht,-1);
   glVertex3f(wd,-ht,-1);
   glVertex3f(wd,-ht,+1);
   glVertex3f(-wd,-ht,+1);
   // Chimney Front
   glColor3f(1,0,1);
   glVertex3f(+0.75,+1, -0.25);
   glVertex3f(+1,+1, -0.25);
   glVertex3f(+1,+1.5,-0.25);
   glVertex3f(+0.75,+1.5, -0.25);
   // Chimney Right
   glColor3f(1,1,0);
   glVertex3f(+1,+1, -0.25);
   glVertex3f(+1,+1, -0.5);
   glVertex3f(+1,+1.5,-0.5);
   glVertex3f(+1,+1.5, -0.25);
   // Chimney Back
   glColor3f(0,0,1);
   glVertex3f(+1,+1, -0.5);
   glVertex3f(+0.75,+1, -0.5);
   glVertex3f(+0.75,+1.5, -0.5);
   glVertex3f(+1,+1.5,-0.5);
   // Chimney Left
   glColor3f(0,1,0);
   glVertex3f(+0.75,+1, -0.5);
   glVertex3f(+0.75,+1, -0.25);
   glVertex3f(+0.75,+1.5, -0.25);
   glVertex3f(+0.75,+1.5, -0.5);
   // Chimney Top
   glColor3f(0,1,1);
   glVertex3f(+0.75,+1.5, -0.25);
   glVertex3f(+1,+1.5,-0.25);
   glVertex3f(+1,+1.5,-0.5);
   glVertex3f(+0.75,+1.5, -0.5);
   //  End
   glEnd();
   glBegin(GL_TRIANGLES);
   // Front triangle
   glColor3f(1,0,0);
   glVertex3f(wd,ht, 1);
   glVertex3f(0,+2, 1);
   glVertex3f(-wd,ht, 1);
   // Back triangle
   glColor3f(0,0,1);
   glVertex3f(-wd,ht,-1);
   glVertex3f(0, +2, -1);
   glVertex3f(wd,ht,-1);

   glEnd();
   //  Undo transformations
   glPopMatrix();
 }

 /*
  *  Draw solid airplane
  *    at (x,y,z)
  *    nose towards (dx,dy,dz)
  *    up towards (ux,uy,uz)
  */
 static void SolidPlane(double x,double y,double z,
                        double dx,double dy,double dz,
                        double ux,double uy, double uz)
 {
    // Dimensions used to size airplane
    const double wid=0.05;
    const double nose=+0.50;
    const double cone= 0.20;
    const double wing= 0.00;
    const double strk=-0.20;
    const double tail=-0.50;
    //  Unit vector in direction of flght
    double D0 = sqrt(dx*dx+dy*dy+dz*dz);
    double X0 = dx/D0;
    double Y0 = dy/D0;
    double Z0 = dz/D0;
    //  Unit vector in "up" direction
    double D1 = sqrt(ux*ux+uy*uy+uz*uz);
    double X1 = ux/D1;
    double Y1 = uy/D1;
    double Z1 = uz/D1;
    //  Cross product gives the third vector
    double X2 = Y0*Z1-Y1*Z0;
    double Y2 = Z0*X1-Z1*X0;
    double Z2 = X0*Y1-X1*Y0;
    //  Rotation matrix
    double mat[16];
    mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
    mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
    mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
    mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

    //  Save current transforms
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glMultMatrixd(mat);
    //  Nose (4 sided)
    glColor3f(0,0,1);
    glBegin(GL_TRIANGLES);
    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone, wid, wid);
    glVertex3d(cone,-wid, wid);

    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone, wid,-wid);
    glVertex3d(cone,-wid,-wid);

    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone, wid, wid);
    glVertex3d(cone, wid,-wid);

    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone,-wid, wid);
    glVertex3d(cone,-wid,-wid);
    glEnd();
    //  Fuselage (square tube)
    glBegin(GL_QUADS);
    glVertex3d(cone, wid, wid);
    glVertex3d(cone,-wid, wid);
    glVertex3d(tail,-wid, wid);
    glVertex3d(tail, wid, wid);

    glVertex3d(cone, wid,-wid);
    glVertex3d(cone,-wid,-wid);
    glVertex3d(tail,-wid,-wid);
    glVertex3d(tail, wid,-wid);

    glVertex3d(cone, wid, wid);
    glVertex3d(cone, wid,-wid);
    glVertex3d(tail, wid,-wid);
    glVertex3d(tail, wid, wid);

    glVertex3d(cone,-wid, wid);
    glVertex3d(cone,-wid,-wid);
    glVertex3d(tail,-wid,-wid);
    glVertex3d(tail,-wid, wid);

    glVertex3d(tail,-wid, wid);
    glVertex3d(tail, wid, wid);
    glVertex3d(tail, wid,-wid);
    glVertex3d(tail,-wid,-wid);
    glEnd();
    //  Wings (plane triangles)
    glColor3f(1,1,0);
    glBegin(GL_TRIANGLES);
    glVertex3d(wing, 0.0, wid);
    glVertex3d(tail, 0.0, wid);
    glVertex3d(tail, 0.0, 0.5);

    glVertex3d(wing, 0.0,-wid);
    glVertex3d(tail, 0.0,-wid);
    glVertex3d(tail, 0.0,-0.5);
    glEnd();
    //  Vertical tail (plane triangle)
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    glVertex3d(strk, 0.0, 0.0);
    glVertex3d(tail, 0.3, 0.0);
    glVertex3d(tail, 0.0, 0.0);
    glEnd();
    //  Undo transformations
    glPopMatrix();
 }

 /*
  *  OpenGL (GLUT) calls this routine to display the scene
  */
 void display()
 {
    const double len=1.5;  //  Length of axes
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();
    //  Set view angle
    glRotatef(ph,1,0,0);
    glRotatef(th,0,1,0);
    //  Draws 2 houses and a plane
    house(1,0,0 , 0.6,0.6,0.3 , 15);
    house(0,0,0.5 , 0.2,0.2,0.2 , 45);
    SolidPlane( -1, 1, 0 , 1,0,0 , 0, 5, 5);

    //  White
    glColor3f(1,1,1);
    //  Draw axes
    if (axes)
    {
       glBegin(GL_LINES);
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(len,0.0,0.0);
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(0.0,len,0.0);
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(0.0,0.0,len);
       glEnd();
       //  Label axes
       glRasterPos3d(len,0.0,0.0);
       Print("X");
       glRasterPos3d(0.0,len,0.0);
       Print("Y");
       glRasterPos3d(0.0,0.0,len);
       Print("Z");
    }
    //  Five pixels from the lower left corner of the window
    glWindowPos2i(5,25);
    //  Print the text string
    Print("Angle=%d,%d",th,ph);
    //  Render the scene
    glFlush();
    //  Make the rendered scene visible
    glutSwapBuffers();
 }

 /*
  *  GLUT calls this routine when an arrow key is pressed
  */
 void special(int key,int x,int y)
 {
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
       th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
       th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
       ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
       ph -= 5;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
 }

 /*
  *  GLUT calls this routine when a key is pressed
  */
 void key(unsigned char ch,int x,int y)
 {
    //  Exit on ESC
    if (ch == 27)
       exit(0);
    //  Reset view angle
    else if (ch == '0')
       th = ph = 0;
    //  Toggle axes
    else if (ch == 'a' || ch == 'A')
       axes = 1-axes;
    //  Switch display mode
    glutPostRedisplay();
 }

 /*
  *  GLUT calls this routine when the window is resized
  */
 void reshape(int width,int height)
 {
    const double dim=2.5;
    //  Ratio of the width to the height of the window
    double w2h = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Orthogonal projection
    glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
 }

 /*
  *  GLUT calls this toutine when there is nothing else to do
  */
 void idle()
 {
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(90*t,360);
    glutPostRedisplay();
 }

 /*
  *  Start up GLUT and tell it what to do
  */
 int main(int argc,char* argv[])
 {
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitWindowSize(600,600);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    //  Create the window
    glutCreateWindow("Objects");
    //  Tell GLUT to call "idle" when there is nothing else to do
    glutIdleFunc(idle);
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
 }
