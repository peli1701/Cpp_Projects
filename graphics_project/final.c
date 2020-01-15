/*
 *  Lighting
 *
 *  Demonstrates basic lighting using a sphere and a cube.
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */

#include "CSCIx229.h"

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int move=1;       //  Move light
int th=0;;         //  Azimuth of view angle
int ph=20;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=0;      //  Lighting
double asp=2;     //  Aspect ratio
double dim=4;   //  Size of world  
double Fpx = 0;
double Fpy = 0;
double Fpz = 5.0;
double xCoord = 0;
double zCoord = 0;
int fp_mode = 0;    //  first person navigation
int view = 0.0;

// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[30]; //textures
#define PI 3.1426



/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);

   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

static void myProject()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if(fp_mode) {
      gluPerspective(fov,asp,dim/2,2*dim);
   }
   else {
      if (mode)
         gluPerspective(fov,asp,dim/4,4*dim);
      //  Orthogonal projection
      else
         glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   }  
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

static void home1(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   glPushMatrix();
   float white[] = {1, 1, 1, 1};
   float black[] = {0,0,0,1};

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   // Save transformation
   //mof();
   // Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[0]);
   // Cube
   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(2.0, 0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1, 1);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0, 2.0); glVertex3f(+1,+1,-1);

   // Right
   glNormal3d(1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(2.0, 0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0, 2.0); glVertex3f(+1,+1,+1);

   // Left
   glNormal3d(-1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1,-1);

   // Top
   glNormal3d(0, 1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);

   // Bottom
   glNormal3d(0, -1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   glEnd();

   // Roof
   glBindTexture(GL_TEXTURE_2D,texture[1]);

   glBegin(GL_TRIANGLES);


   glNormal3d(0, 0, 1);
   // Front
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(2.0, 0.0);glVertex3f(-1, 1, 1);
   glTexCoord2f(2.0, 2.0);glVertex3f(1, 1, 1);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1, 1, -1);
   glTexCoord2f(2.0, 2.0); glVertex3f(1, 1, -1);

   //  End
   glEnd();

   // ROOF
   glBegin(GL_QUADS);

   //  Right
   glNormal3d(1, 1, 0);

   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, 1, 1);
   glTexCoord2f(0.0, 1.0); glVertex3f(1, 1, -1);

   //  Left
   glNormal3d(-1, 1, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(2.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1, 1, 1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1, 1, -1);
   glEnd();

   // Door
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.35, -.3, 1.01);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, -.3, 1.01);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, -1, 1.01);
   glTexCoord2f(0.0, 1.0); glVertex3f(.35, -1, 1.01);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.35, -.3, -1.01);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, -.3, -1.01);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, -1, -1.01);
   glTexCoord2f(0.0, 1.0); glVertex3f(.35, -1, -1.01);

   glEnd();

   // Windows
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);

   // Window
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.6, .6,  1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.2, .6, 1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.2, .2, 1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-.6, .2, 1.1);

   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, .6,  1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.3, .6, 1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(.3, .2, 1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(.7, .2, 1.1);

   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.6, .6, -1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.2, .6, -1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.2, .2, -1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-.6, .2, -1.1);

   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, .6,  -1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.3, .6, -1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(.3, .2, -1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(.7, .2, -1.1);

   glEnd();
   //  Undo transformations
   glPopMatrix();
}
static void home2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   glPushMatrix();
   float white[] = {1, 1, 1, 1};
   float black[] = {0,0,0,1};

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   // Save transformation
   //glPushMatrix();
   // Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[18]);
   // Cube
   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(2.0, 0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1, 1);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0, 2.0); glVertex3f(+1,+1,-1);

   // Right
   glNormal3d(1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(2.0, 0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0, 2.0); glVertex3f(+1,+1,+1);

   // Left
   glNormal3d(-1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1,-1);

   // Top
   glNormal3d(0, 1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);

   // Bottom
   glNormal3d(0, -1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   glEnd();

   // Roof
   glBindTexture(GL_TEXTURE_2D,texture[1]);

   glBegin(GL_TRIANGLES);


   glNormal3d(0, 0, 1);
   // Front
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(2.0, 0.0);glVertex3f(-1, 1, 1);
   glTexCoord2f(2.0, 2.0);glVertex3f(1, 1, 1);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1, 1, -1);
   glTexCoord2f(2.0, 2.0); glVertex3f(1, 1, -1);

   //  End
   glEnd();

   // ROOF
   glBegin(GL_QUADS);

   //  Right
   glNormal3d(1, 1, 0);

   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, 1, 1);
   glTexCoord2f(0.0, 1.0); glVertex3f(1, 1, -1);

   //  Left
   glNormal3d(-1, 1, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(2.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1, 1, 1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1, 1, -1);
   glEnd();

   // Door
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.35, -.3, 1.01);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, -.3, 1.01);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, -1, 1.01);
   glTexCoord2f(0.0, 1.0); glVertex3f(.35, -1, 1.01);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.35, -.3, -1.01);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, -.3, -1.01);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, -1, -1.01);
   glTexCoord2f(0.0, 1.0); glVertex3f(.35, -1, -1.01);

   glEnd();

   // Windows
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);

   // Window
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.6, .6,  1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.2, .6, 1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.2, .2, 1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-.6, .2, 1.1);

   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, .6,  1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.3, .6, 1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(.3, .2, 1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(.7, .2, 1.1);

   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.6, .6, -1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.2, .6, -1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.2, .2, -1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-.6, .2, -1.1);

   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, .6,  -1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.3, .6, -1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(.3, .2, -1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(.7, .2, -1.1);

   glEnd();
   //  Undo transformations
   glPopMatrix();
}

static void home3(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   glPushMatrix();
   float white[] = {1, 1, 1, 1};
   float black[] = {0,0,0,1};

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   // Save transformation
   //glPushMatrix();
   // Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[19]);
   // Cube
   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(2.0, 0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1, 1);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0, 2.0); glVertex3f(+1,+1,-1);

   // Right
   glNormal3d(1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(2.0, 0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0, 2.0); glVertex3f(+1,+1,+1);

   // Left
   glNormal3d(-1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1,-1);

   // Top
   glNormal3d(0, 1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);

   // Bottom
   glNormal3d(0, -1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   glEnd();

   // Roof
   glBindTexture(GL_TEXTURE_2D,texture[1]);

   glBegin(GL_TRIANGLES);


   glNormal3d(0, 0, 1);
   // Front
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(2.0, 0.0);glVertex3f(-1, 1, 1);
   glTexCoord2f(2.0, 2.0);glVertex3f(1, 1, 1);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1, 1, -1);
   glTexCoord2f(2.0, 2.0); glVertex3f(1, 1, -1);

   //  End
   glEnd();

   // ROOF
   glBegin(GL_QUADS);

   //  Right
   glNormal3d(1, 1, 0);

   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, 1, 1);
   glTexCoord2f(0.0, 1.0); glVertex3f(1, 1, -1);

   //  Left
   glNormal3d(-1, 1, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(2.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1, 1, 1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1, 1, -1);
   glEnd();

   // Door
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.35, -.3, 1.01);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, -.3, 1.01);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, -1, 1.01);
   glTexCoord2f(0.0, 1.0); glVertex3f(.35, -1, 1.01);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.35, -.3, -1.01);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, -.3, -1.01);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, -1, -1.01);
   glTexCoord2f(0.0, 1.0); glVertex3f(.35, -1, -1.01);

   glEnd();

   // Windows
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);

   // Window
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.6, .6,  1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.2, .6, 1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.2, .2, 1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-.6, .2, 1.1);

   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, .6,  1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.3, .6, 1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(.3, .2, 1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(.7, .2, 1.1);

   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.6, .6, -1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.2, .6, -1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.2, .2, -1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-.6, .2, -1.1);

   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, .6,  -1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.3, .6, -1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(.3, .2, -1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(.7, .2, -1.1);

   glEnd();
   //  Undo transformations
   glPopMatrix();
}

static void home4(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   glPushMatrix();
   float white[] = {1, 1, 1, 1};
   float black[] = {0,0,0,1};

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   // Save transformation
   //glPushMatrix();
   // Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[18]);
   // Cube
   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(2.0, 0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1, 1);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0, 2.0); glVertex3f(+1,+1,-1);

   // Right
   glNormal3d(1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(2.0, 0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0, 2.0); glVertex3f(+1,+1,+1);

   // Left
   glNormal3d(-1, 0, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1,+1,-1);

   // Top
   glNormal3d(0, 1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);

   // Bottom
   glNormal3d(0, -1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   glEnd();

   // Roof
   glBindTexture(GL_TEXTURE_2D,texture[1]);

   glBegin(GL_TRIANGLES);


   glNormal3d(0, 0, 1);
   // Front
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(2.0, 0.0);glVertex3f(-1, 1, 1);
   glTexCoord2f(2.0, 2.0);glVertex3f(1, 1, 1);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(2.0, 0.0); glVertex3f(-1, 1, -1);
   glTexCoord2f(2.0, 2.0); glVertex3f(1, 1, -1);

   //  End
   glEnd();

   // ROOF
   glBegin(GL_QUADS);

   //  Right
   glNormal3d(1, 1, 0);

   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, 1, 1);
   glTexCoord2f(0.0, 1.0); glVertex3f(1, 1, -1);

   //  Left
   glNormal3d(-1, 1, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 2, -1);
   glTexCoord2f(2.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(2.0, 2.0); glVertex3f(-1, 1, 1);
   glTexCoord2f(0.0, 2.0); glVertex3f(-1, 1, -1);
   glEnd();

   // Door
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.35, -.3, 1.01);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, -.3, 1.01);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, -1, 1.01);
   glTexCoord2f(0.0, 1.0); glVertex3f(.35, -1, 1.01);

   // Back
   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.35, -.3, -1.01);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, -.3, -1.01);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, -1, -1.01);
   glTexCoord2f(0.0, 1.0); glVertex3f(.35, -1, -1.01);

   glEnd();

   // Windows
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);

   // Window
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.6, .6,  1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.2, .6, 1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.2, .2, 1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-.6, .2, 1.1);

   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, .6,  1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.3, .6, 1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(.3, .2, 1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(.7, .2, 1.1);

   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.6, .6, -1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(-.2, .6, -1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(-.2, .2, -1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(-.6, .2, -1.1);

   glNormal3d(0, 0, -1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, .6,  -1.1);
   glTexCoord2f(0.5, 0.0); glVertex3f(.3, .6, -1.1);
   glTexCoord2f(0.5, 0.5); glVertex3f(.3, .2, -1.1);
   glTexCoord2f(0.0, 0.5); glVertex3f(.7, .2, -1.1);

   glEnd();
   //  Undo transformations
   glPopMatrix();
}



static void twintower1(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);


   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[5]);
      // Save transformation

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 3, -0);
   glVertex3f(0, 3, 0);
   glVertex3f(-0, 3, -0.5);
   glVertex3f(-.5, 3, -.5);

   glEnd();
   glPopMatrix();
}

static void twintower2(){
   glPushMatrix();

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[5]);
      // Save transformation
   //glPushMatrix();

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(.5,-1, 0);
   glTexCoord2f(1.0, 0.0); glVertex3f(.5, 3, 0);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, 3, 0);
   glTexCoord2f(0.0, 1.0); glVertex3f(1,-1, 0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(1, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(.5, -1, 0);
   glTexCoord2f(1.0, 0.0); glVertex3f(.5, 3, 0);
   glTexCoord2f(1.0, 1.0); glVertex3f(.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(.5, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(1, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(1, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(1, -1, -.5);

   // Top 
   glVertex3f(1, 3, -.5);
   glVertex3f(.5, 3, -.5);
   glVertex3f(.5, 3, 0);
   glVertex3f(1, 3, 0);
   
   glEnd();
   glPopMatrix();
}

static void pointyTower(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      // Save transformation
   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[6]);

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(0,-1, 1);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, 2, 1);
   glTexCoord2f(1.0, 1.0); glVertex3f(.5, 2, 1);
   glTexCoord2f(0.0, 1.0); glVertex3f(.5,-1, 1);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(0, -1, .5);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, 2, .5);
   glTexCoord2f(1.0, 1.0); glVertex3f(.5, 2, .5);
   glTexCoord2f(0.0, 1.0); glVertex3f(.5, -1, .5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(0, -1, .5);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, 2, .5);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, 2, 1);
   glTexCoord2f(0.0, 1.0); glVertex3f(0, -1, 1);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(.5, -1, .5);
   glTexCoord2f(1.0, 0.0); glVertex3f(.5, 2, .5);
   glTexCoord2f(1.0, 1.0); glVertex3f(.5, 2, 1);
   glTexCoord2f(0.0, 1.0); glVertex3f(.5, -1, 1);

   // Top 
   glVertex3f(0, 2, 1);
   glVertex3f(.5, 2, 1);
   glVertex3f(.5, 2, .5);
   glVertex3f(0, 2, .5);

   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_TRIANGLES);

    // Front
   glTexCoord2f(0.0, 0.0); glVertex3f(.25, 2.4, .75);
   glTexCoord2f(1.0, 0.0);glVertex3f(0, 2, 1);
   glTexCoord2f(1.0, 1.0);glVertex3f(.5, 2, 1);
   
   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(.25, 2.4, .75);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, 2, .5);
   glTexCoord2f(1.0, 1.0); glVertex3f(.5, 2, .5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(.25, 2.4, .75);
   glTexCoord2f(1.0, 0.0); glVertex3f(.5, 2, .5);
   glTexCoord2f(1.0, 1.0); glVertex3f(.5, 2, 1);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(.25, 2.4, .75);
   glTexCoord2f(1.0, 0.0); glVertex3f(0, 2, .5);
   glTexCoord2f(1.0, 1.0); glVertex3f(0, 2, 1);

   glEnd();
   glPopMatrix();

}

static void boxTower(){
   glPushMatrix();

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[6]);
      // Save transformation
   //glPushMatrix();

   glBegin(GL_QUADS);

   // First Layer

   // Front
   glTexCoord2f(0.0, 0.0); glVertex3f(.8,-1, -1.8);
   glTexCoord2f(1.0, 0.0); glVertex3f(.8, .01, -1.8);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.2, .01, -1.8);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.2,-1, -1.8);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(.8,-1, -1.05);
   glTexCoord2f(1.0, 0.0); glVertex3f(.8, .01, -1.05);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.2, .01, -1.05);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.2,-1, -1.05);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(.8, -1, -1.8);
   glTexCoord2f(1.0, 0.0); glVertex3f(.8, .01, -1.8);
   glTexCoord2f(1.0, 1.0); glVertex3f(.8, .01, -1.05);
   glTexCoord2f(0.0, 1.0); glVertex3f(.8, -1, -1.05);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.2, -1, -1.8);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.2, .01, -1.8);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.2, .01, -1.05);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.2, -1, -1.05);

   // Base
   glTexCoord2f(0.0, 0.0); glVertex3f(.8, .01, -1.8);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.2, .01, -1.8);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.2, .01, -1.05);
   glTexCoord2f(0.0, 1.0); glVertex3f(.8, .01, -1.05);


   // Second Layer

   // Top

   glTexCoord2f(0.0, 0.0); glVertex3f(.7, 1.5, -1.6);
   glTexCoord2f(1.0, 0.0); glVertex3f(.7, -1, -1.6);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.1, -1, -1.6);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.1, 1.5, -1.6);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, 1.5, -1.2);
   glTexCoord2f(1.0, 0.0); glVertex3f(.7, -1, -1.2);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.1, -1, -1.2);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.1, 1.5, -1.2);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, 1.5, -1.6);
   glTexCoord2f(1.0, 0.0); glVertex3f(.7, -1, -1.6);
   glTexCoord2f(1.0, 1.0); glVertex3f(.7, -1, -1.2);
   glTexCoord2f(0.0, 1.0); glVertex3f(.7, 1.5, -1.2);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.1, 1.5, -1.6);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.1, -1, -1.6);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.1, -1, -1.2);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.1, 1.5, -1.2);

   // Base
   glTexCoord2f(0.0, 0.0); glVertex3f(.7, 1.5, -1.6);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.1, 1.5, -1.6);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.1, 1.5, -1.2);
   glTexCoord2f(0.0, 1.0); glVertex3f(.7, 1.5, -1.2);

   glEnd();
   glPopMatrix();
}

static void genericTower1(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){


   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[9]);
      // Save transformation

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 3, -0);
   glVertex3f(0, 3, 0);
   glVertex3f(-0, 3, -0.5);
   glVertex3f(-.5, 3, -.5);

   glEnd();
   glPopMatrix();
}

static void genericTower2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[10]);
      // Save transformation
   //glPushMatrix();

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 3, -0);
   glVertex3f(0, 3, 0);
   glVertex3f(-0, 3, -0.5);
   glVertex3f(-.5, 3, -.5);

   glEnd();
   glPopMatrix();
}

static void genericTower3(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[11]);
      // Save transformation

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 3, -0);
   glVertex3f(0, 3, 0);
   glVertex3f(-0, 3, -0.5);
   glVertex3f(-.5, 3, -.5);

   glEnd();
   glPopMatrix();
}

static void genericTower4(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[12]);
      // Save transformation

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 3, -0);
   glVertex3f(0, 3, 0);
   glVertex3f(-0, 3, -0.5);
   glVertex3f(-.5, 3, -.5);

   glEnd();
   glPopMatrix();
}

static void genericTower5(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[13]);
      // Save transformation

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 3, -0);
   glVertex3f(0, 3, 0);
   glVertex3f(-0, 3, -0.5);
   glVertex3f(-.5, 3, -.5);

   glEnd();
   glPopMatrix();
}


static void genericTower6(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[14]);
      // Save transformation

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 3, -0);
   glVertex3f(0, 3, 0);
   glVertex3f(-0, 3, -0.5);
   glVertex3f(-.5, 3, -.5);

   glEnd();
   glPopMatrix();
}

static void genericTower7(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[15]);
      // Save transformation

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 3, -0);
   glVertex3f(0, 3, 0);
   glVertex3f(-0, 3, -0.5);
   glVertex3f(-.5, 3, -.5);

   glEnd();
   glPopMatrix();
}

static void genericTower8(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[16]);
      // Save transformation

   glBegin(GL_QUADS);

   // Front
   glNormal3d(0, 0, 1);
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 3, -0);
   glVertex3f(0, 3, 0);
   glVertex3f(-0, 3, -0.5);
   glVertex3f(-.5, 3, -.5);

   glEnd();
   glPopMatrix();
}

static void airTrafficBuilding(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[25]);
   // Save transformation

   glBegin(GL_QUADS);

   // Front
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 2.5, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 2.5, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0,-1, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 2.5, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 2.5, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 2.5, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 2.5, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, -1, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 2.5, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 2.5, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, -1, -.5);

   // Top 
   glVertex3f(-.5, 2.5, -0);
   glVertex3f(0, 2.5, 0);
   glVertex3f(-0, 2.5, -0.5);
   glVertex3f(-.5, 2.5, -.5);

   glEnd();

   // Window part

   glBindTexture(GL_TEXTURE_2D,texture[15]);

   glBegin(GL_QUADS);

   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, 2.5, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3.2, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3.2, -0);
   glTexCoord2f(0.0, 1.0);glVertex3f(-0, 2.5, -0);

   // Back
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, 2.5, -.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3.2, -.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3.2, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, 2.5, -.5);

   // Right
   glTexCoord2f(0.0, 0.0); glVertex3f(-0, 2.5, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-0, 3.2, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-0, 3.2, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-0, 2.5, -.5);

   // Left
   glTexCoord2f(0.0, 0.0); glVertex3f(-.5, 2.5, -0);
   glTexCoord2f(1.0, 0.0); glVertex3f(-.5, 3.2, -0);
   glTexCoord2f(1.0, 1.0); glVertex3f(-.5, 3.2, -.5);
   glTexCoord2f(0.0, 1.0); glVertex3f(-.5, 2.5, -.5);

   // Top

   glVertex3f(-.5, 3.2, -0);
   glVertex3f(0, 3.2, 0);
   glVertex3f(-0, 3.2, -0.5);
   glVertex3f(-.5, 3.2, -.5);



   glEnd();
   glPopMatrix();
}

static void fence(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){


      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      glScaled(dx,dy,dz);

      glBindTexture(GL_TEXTURE_2D,texture[21]);

      glBegin(GL_QUADS);

      // Front

      glTexCoord2f(0.0, 0.0); glVertex3f(6.0,-1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(6.0, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(10, 1, 1.5);
      glTexCoord2f(0.0, 1.0);glVertex3f(10,-1, 1.5);

      // Back

      glTexCoord2f(0.0, 0.0); glVertex3f(6.0,-1, 1.4);
      glTexCoord2f(1.0, 0.0); glVertex3f(6.0, 1, 1.4);
      glTexCoord2f(1.0, 1.0); glVertex3f(10, 1, 1.4);
      glTexCoord2f(0.0, 1.0);glVertex3f(10,-1, 1.4);

      // Left
      glTexCoord2f(0.0, 0.0); glVertex3f(6.0,-1, 1.4);
      glTexCoord2f(1.0, 0.0); glVertex3f(6.0, 1, 1.4);
      glTexCoord2f(1.0, 1.0); glVertex3f(6.0, 1, 1.5);
      glTexCoord2f(0.0, 1.0);glVertex3f(6.0,-1, 1.5);

      // Right
      glTexCoord2f(0.0, 0.0); glVertex3f(10,-1, 1.4);
      glTexCoord2f(1.0, 0.0); glVertex3f(10, 1, 1.4);
      glTexCoord2f(1.0, 1.0); glVertex3f(10, 1, 1.5);
      glTexCoord2f(0.0, 1.0);glVertex3f(10,-1, 1.5);

      // Top
      glTexCoord2f(0.0, 0.0); glVertex3f(6.0, 1, 1.4);
      glTexCoord2f(1.0, 0.0); glVertex3f(10, 1, 1.4);
      glTexCoord2f(1.0, 1.0); glVertex3f(10, 1, 1.5);
      glTexCoord2f(0.0, 1.0);glVertex3f(6.0, 1, 1.5);


      // Rear

      glEnd();
      glPopMatrix();
}

static void fence2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      glScaled(dx,dy,dz);

      glBegin(GL_QUADS);

      // left

      glTexCoord2f(0.0, 0.0); glVertex3f(6.0,-1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(6.0, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(6.0, 1, .4);
      glTexCoord2f(0.0, 1.0);glVertex3f(6.0, -1, .4);

      // right

      glTexCoord2f(0.0, 0.0); glVertex3f(6.2,-1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(6.2, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(6.2, 1, .4);
      glTexCoord2f(0.0, 1.0);glVertex3f(6.2, -1, .4);

      // top

      glTexCoord2f(0.0, 0.0); glVertex3f(6.0, 1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(6.2, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(6.2, 1, .4);
      glTexCoord2f(0.0, 1.0);glVertex3f(6.0, 1, .4);


      // left

      glTexCoord2f(0.0, 0.0); glVertex3f(6.0,-1, -2);
      glTexCoord2f(1.0, 0.0); glVertex3f(6.0, 1, -2);
      glTexCoord2f(1.0, 1.0); glVertex3f(6.0, 1, -.9);
      glTexCoord2f(0.0, 1.0);glVertex3f(6.0, -1, -.9);

      //right
      glTexCoord2f(0.0, 0.0); glVertex3f(6.2,-1, -2);
      glTexCoord2f(1.0, 0.0); glVertex3f(6.2, 1, -2);
      glTexCoord2f(1.0, 1.0); glVertex3f(6.2, 1, -.9);
      glTexCoord2f(0.0, 1.0);glVertex3f(6.2, -1, -.9);

      //top
      glTexCoord2f(0.0, 0.0); glVertex3f(6.0, 1, -2);
      glTexCoord2f(1.0, 0.0); glVertex3f(6.2, 1, -2);
      glTexCoord2f(1.0, 1.0); glVertex3f(6.2, 1, -.9);
      glTexCoord2f(0.0, 1.0);glVertex3f(6.0, 1, -.9);

      // Back


      glEnd();
      glPopMatrix();

}

static void fence3(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){


      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      glScaled(dx,dy,dz);

      glBindTexture(GL_TEXTURE_2D,texture[21]);

      glBegin(GL_QUADS);

      // left

      glTexCoord2f(0.0, 0.0); glVertex3f(10,-1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(10, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(10, 1, -2);
      glTexCoord2f(0.0, 1.0);glVertex3f(10, -1, -2);

      glTexCoord2f(0.0, 0.0); glVertex3f(9.8,-1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(9.8, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(9.8, 1, -2);
      glTexCoord2f(0.0, 1.0);glVertex3f(9.8, -1, -2);

      glTexCoord2f(0.0, 0.0); glVertex3f(10,1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(9.8, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(9.8, 1, -2);
      glTexCoord2f(0.0, 1.0); glVertex3f(10, 1, -2);

      // right

      glTexCoord2f(0.0, 0.0); glVertex3f(-10,-1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(-10, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(-10, 1, -2);
      glTexCoord2f(0.0, 1.0);glVertex3f(-10, -1, -2);

      glTexCoord2f(0.0, 0.0); glVertex3f(-9.8,-1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(-9.8, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(-9.8, 1, -2);
      glTexCoord2f(0.0, 1.0);glVertex3f(-9.8, -1, -2);

      glTexCoord2f(0.0, 0.0); glVertex3f(-10,1, 1.5);
      glTexCoord2f(1.0, 0.0); glVertex3f(-9.8, 1, 1.5);
      glTexCoord2f(1.0, 1.0); glVertex3f(-9.8, 1, -2);
      glTexCoord2f(0.0, 1.0);glVertex3f(-10, 1, -2);




      glEnd();
      glPopMatrix();

}
static void hangar(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,1,0,0);
      //glRotated(th2, 1, 0, 0);
      glScaled(dx,dy,dz);

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texture[21]);
      // Save transformation

      glBegin(GL_QUAD_STRIP);

      double i;
      double radius = 1, height = 1;

      // Tube
      for(i = 0.0; i <= 2.1*PI; i+=.05){
         glVertex3d(radius * cos(i), height, radius * sin(i));
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }

      glEnd();

      glBindTexture(GL_TEXTURE_2D,texture[22]);

      glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);

      // Top
      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), height, radius * sin(i));
      }
      glEnd();

      //Bottom
      glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, -height, 0.0);

      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
      glEnd();

      glPopMatrix();
}

static void airplane(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,0,1);
      //glRotated(th2, 1, 0, 0);
      glScaled(dx,dy,dz);

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texture[26]);
      // Save transformation

      double i;
      double radius = 1, height = 1;

      glBegin(GL_QUAD_STRIP);
      for(i = 0.0; i <= 2.1*PI; i+=.05){
         glVertex3d(radius * cos(i), height, radius * sin(i));
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }

      glEnd();

      glBindTexture(GL_TEXTURE_2D,texture[22]);

      glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);

      // Top
      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), height, radius * sin(i));
      }
      glEnd();

      //Bottom
      glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, -height, 0.0);

      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
      glEnd();

      glPopMatrix();
}

static void cockpit(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texture[26]);

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      //glRotated(th2, 1, 0, 0);
      glScaled(dx,dy,dz);

      glutSolidCone(.23,.6,10,1);

      glEnd();
      glPopMatrix();


}

static void wing1(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      //glRotated(th2, 1, 0, 0);
      glScaled(dx,dy,dz);

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texture[26]);

      glBegin(GL_QUADS);

      // left

      glTexCoord2f(0.0, 0.0); glVertex3f(0,0, 0);
      glTexCoord2f(1.0, 0.0); glVertex3f(.35, 0, 0);
      glTexCoord2f(1.0, 1.0); glVertex3f(.35, 0, 1.25);
      glTexCoord2f(0.0, 1.0);glVertex3f(0, 0, 1);

      glEnd();
      glPopMatrix();


}

static void wing2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      //glRotated(th2, 1, 0, 0);
      glScaled(dx,dy,dz);

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texture[26]);

      glBegin(GL_QUADS);

      // left

      glTexCoord2f(0.0, 0.0); glVertex3f(0,0, 0);
      glTexCoord2f(1.0, 0.0); glVertex3f(.35, 0, 0);
      glTexCoord2f(1.0, 1.0); glVertex3f(.35, 0, -1.25);
      glTexCoord2f(0.0, 1.0);glVertex3f(0, 0, -1);

      glEnd();
      glPopMatrix();


}

static void fin(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      //glRotated(th2, 1, 0, 0);
      glScaled(dx,dy,dz);

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texture[26]);

      glBegin(GL_TRIANGLES);

      // left

      glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 0);
      glTexCoord2f(1.0, 0.0);glVertex3f(.5, 0, 0);
      glTexCoord2f(1.0, 1.0);glVertex3f(.25, .5, 0);

      glEnd();
      glPopMatrix();


}

static void mountain1(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      //glRotated(th2, 1, 0, 0);
      glScaled(dx,dy,dz);

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texture[27]);

      glBegin(GL_TRIANGLES);

      // front
      glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, .5);
      glTexCoord2f(1.0, 0.0);glVertex3f(.5, -1, .5);
      glTexCoord2f(1.0, 1.0);glVertex3f(0, 0, 0);

      // back
      glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
      glTexCoord2f(1.0, 0.0);glVertex3f(.5, -1, -.5);
      glTexCoord2f(1.0, 1.0);glVertex3f(0, 0, 0);

      // right
      glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, .5);
      glTexCoord2f(1.0, 0.0);glVertex3f(-.5, -1, -.5);
      glTexCoord2f(1.0, 1.0);glVertex3f(0, 0, 0);

      // left
      glTexCoord2f(0.0, 0.0); glVertex3f(.5, -1, .5);
      glTexCoord2f(1.0, 0.0);glVertex3f(.5, -1, -.5);
      glTexCoord2f(1.0, 1.0);glVertex3f(0, 0, 0);




      glEnd();
      glPopMatrix();


}
static void mountain2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      //glRotated(th2, 1, 0, 0);
      glScaled(dx,dy,dz);

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texture[28]);

      glBegin(GL_TRIANGLES);

      // front
      glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, .5);
      glTexCoord2f(1.0, 0.0);glVertex3f(.5, -1, .5);
      glTexCoord2f(1.0, 1.0);glVertex3f(0, 0, 0);

      // back
      glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, -.5);
      glTexCoord2f(1.0, 0.0);glVertex3f(.5, -1, -.5);
      glTexCoord2f(1.0, 1.0);glVertex3f(0, 0, 0);

      // right
      glTexCoord2f(0.0, 0.0); glVertex3f(-.5, -1, .5);
      glTexCoord2f(1.0, 0.0);glVertex3f(-.5, -1, -.5);
      glTexCoord2f(1.0, 1.0);glVertex3f(0, 0, 0);

      // left
      glTexCoord2f(0.0, 0.0); glVertex3f(.5, -1, .5);
      glTexCoord2f(1.0, 0.0);glVertex3f(.5, -1, -.5);
      glTexCoord2f(1.0, 1.0);glVertex3f(0, 0, 0);

      glEnd();
      glPopMatrix();


}

static void tree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glPushMatrix();

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      //glRotated(th2, 1, 0, 0);
      glScaled(dx,dy,dz);

      glColor3f(0.5f, 0.35f, 0.05f);
      double i;
      double radius = .15, height = .25;

      glBegin(GL_QUAD_STRIP);
      for(i = 0.0; i <= 2.1*PI; i+=.05){
         glVertex3d(radius * cos(i), height, radius * sin(i));
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }

      glEnd();
      glPopMatrix();


      glPushMatrix();
      glTranslated(x,-.50,z);
      glRotated(-90,1,0,0);
      glColor3f(0, 1, 0);
      glutSolidCone(.25,.50,10,1);

      glEnd();
      glPopMatrix();
      glFlush();
}

static void lake(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();
   glColor3f(1, 1, 1);
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glScaled(dx,dy,dz);

   glColor3f(0, .4, 2);
   
   glBegin(GL_POLYGON);
   double radius = 1.3;
   double segments = 100;
   double angle = 2.0 * PI/segments;
   double angle1 = 0;
   int xc, yc;
   xc = 0; yc = 0;
   //glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
   glVertex2d(radius * cos(0.0) , radius * sin(0.0));
   for (int i = 0; i < segments; i++) {
        glVertex2d(radius * cos(angle1), radius *sin(angle1));
        angle1 += angle;
        //glTexCoord2f(angle1/4, angle/4);
   }


   glEnd();
   glPopMatrix();

}

static void island(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   glPushMatrix();
   glColor3f(0.5f, 0.35f, 0.05f);
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glScaled(dx,dy,dz);

   //glColor3f(1, 1, 1);
   
   glutSolidSphere(1, 20, 20);

   glColor3f(1, 1, 1);
   glEnd();
   glPopMatrix();

}


static void bridge(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   //glColor3f(1, 1, 1);
   glPushMatrix();
   glColor3f(1, 0.5f, 0.0f);
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glScaled(dx,dy,dz);

   //glColor3f(1, 1, 1);
   
   
   double i;
   double radius = .1, height = .5;

   glBegin(GL_QUAD_STRIP);
   for(i = 0.0; i <= 2.1*PI; i+=.05){
   glVertex3d(radius * cos(i), height, radius * sin(i));
   glVertex3d(radius * cos(i), -height, radius * sin(i));
   }

   glEnd();

   glColor3f(1, 1, 1);
   glEnd();
   glPopMatrix();

}

static void crossbeam(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   //glColor3f(1, 1, 1);
   glPushMatrix();
   glColor3f(1, 0.5f, 0.0f);
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   //glColor3f(1, 1, 1);
   
   
   double i;
   double radius = .08, height = .3;

   glBegin(GL_QUAD_STRIP);
   for(i = 0.0; i <= 2.1*PI; i+=.05){
   glVertex3d(radius * cos(i), height, radius * sin(i));
   glVertex3d(radius * cos(i), -height, radius * sin(i));
   }

   glEnd();

   glColor3f(1, 1, 1);
   glEnd();
   glPopMatrix();

}

static void bridgewire(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   //glColor3f(1, 1, 1);
   glPushMatrix();
   glColor3f(1, 0.5f, 0.0f);
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glScaled(dx,dy,dz);

   double i;
   double radius = .03, height = .5;

   glBegin(GL_QUAD_STRIP);
   for(i = 0.0; i <= 2.1*PI; i+=.05){
   glVertex3d(radius * cos(i), height, radius * sin(i));
   glVertex3d(radius * cos(i), -height, radius * sin(i));
   }

   glEnd();

   glColor3f(1, 1, 1);
   glEnd();
   glPopMatrix();

}

static void boat(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   //glColor3f(1, 1, 1);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glBegin(GL_QUADS);

   glColor3f(1, 0, 0);

   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 2);
   glTexCoord2f(1.0, 0.0); glVertex3f(.8, -1, 2);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, -.5, 2);
   glTexCoord2f(0.0, 1.0);glVertex3f(-1.2, -.5, 2);

   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 1.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(.8, -1, 1.5);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, -.5, 1.5);
   glTexCoord2f(0.0, 1.0);glVertex3f(-1.2, -.5, 1.5);

   glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 1.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-1, -1, 2);
   glTexCoord2f(1.0, 1.0); glVertex3f(-1.2, -.5, 2);
   glTexCoord2f(0.0, 1.0);glVertex3f(-1.2, -.5, 1.5);

   glTexCoord2f(0.0, 0.0); glVertex3f(.8,-1, 1.5);
   glTexCoord2f(1.0, 0.0); glVertex3f(.8, -1, 2);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, -.5, 2);
   glTexCoord2f(0.0, 1.0);glVertex3f(1, -.5, 1.5);

   glColor3f(0.5f, 0.35f, 0.05f);

   glTexCoord2f(0.0, 0.0); glVertex3f(-1.2, -.5, 2);
   glTexCoord2f(1.0, 0.0); glVertex3f(1, -.5, 2);
   glTexCoord2f(1.0, 1.0); glVertex3f(1, -.5, 1.5);
   glTexCoord2f(0.0, 1.0);glVertex3f(-1.2, -.5, 1.5);

   glEnd();
   glPopMatrix();

}

static void exhaust(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

   //glColor3f(1, 1, 1);
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   double i;
   double radius = .1, height = .2;

   glColor3f(1, 1, 0);


   glBegin(GL_QUAD_STRIP);
      for(i = 0.0; i <= 2.1*PI; i+=.05){
         glVertex3d(radius * cos(i), height, radius * sin(i));
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }

      glEnd();

      glBindTexture(GL_TEXTURE_2D,texture[22]);

      glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);

      // Top
      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), height, radius * sin(i));
      }
      glEnd();

      //Bottom
      glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, -height, 0.0);

      for(i = 0.0; i < 2*PI*radius*4; i+=.125) {
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
  

   glEnd();
   glPopMatrix();

}
static void grid(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){

      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

      glBindTexture(GL_TEXTURE_2D,texture[4]);
      // Save transformation

      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      glScaled(dx,dy,dz);

      glPushMatrix();


                     // Ground

      glColor3f(1,1,1);
      glEnable(GL_TEXTURE_2D);
      glBegin(GL_QUADS);

      glTexCoord2f(0,0); glVertex3f(-10, -1, -10);
      glTexCoord2f(1,0); glVertex3f(+10, -1, -10);
      glTexCoord2f(1,1); glVertex3f(+10, -1, +10);
      glTexCoord2f(0,1); glVertex3f(-10, -1, +10);
      glEnd();

      /*
      glBindTexture(GL_TEXTURE_2D,texture[17]);

      
      glBegin(GL_QUADS);

      // Rear
      glTexCoord2f(0,0); glVertex3f(-10, -10, -10);
      glTexCoord2f(1,0); glVertex3f(10, -10, -10);
      glTexCoord2f(1,1); glVertex3f(10, 20, -10);
      glTexCoord2f(0,1); glVertex3f(-10, 20, -10);

      glEnd();


                     // Skybox


      // Right

      glBindTexture(GL_TEXTURE_2D,texture[23]);
      glBegin(GL_QUADS);

      glTexCoord2f(0,0); glVertex3f(10, -10, 10);
      glTexCoord2f(1,0); glVertex3f(10, -10, -10);
      glTexCoord2f(1,1); glVertex3f(10, 20, -10);
      glTexCoord2f(0,1); glVertex3f(10, 20, 10);

      glEnd();

      // Left

       glBindTexture(GL_TEXTURE_2D,texture[22]);

      glBegin(GL_QUADS);

      glTexCoord2f(0,0); glVertex3f(-10, -10, 10);
      glTexCoord2f(1,0); glVertex3f(-10, -10, -10);
      glTexCoord2f(1,1); glVertex3f(-10, 20, -10);
      glTexCoord2f(0,1); glVertex3f(-10, 20, 10);

      glEnd();
      */
      
                     // Mote

      // Right
      glBindTexture(GL_TEXTURE_2D,texture[7]);
      glBegin(GL_QUADS);


      glTexCoord2f(0,0); glVertex3f(4, -.9, -4);
      glTexCoord2f(1,0); glVertex3f(4.3, -.9, -4);
      glTexCoord2f(1,1); glVertex3f(4.3, -.9, +4);
      glTexCoord2f(0,1); glVertex3f(4, -.9, +4);

      // Left
      glTexCoord2f(0,0); glVertex3f(-4, -.9, -4);
      glTexCoord2f(1,0); glVertex3f(-4.3, -.9, -4);
      glTexCoord2f(1,1); glVertex3f(-4.3, -.9, +4);
      glTexCoord2f(0,1); glVertex3f(-4, -.9, +4);

      // Front
      glTexCoord2f(0,0); glVertex3f(-4.3, -.9, 4.3);
      glTexCoord2f(1,0); glVertex3f(4.3, -.9, 4.3);
      glTexCoord2f(1,1); glVertex3f(4.3, -.9, +4);
      glTexCoord2f(0,1); glVertex3f(-4.3, -.9, +4);

      // Back
      glTexCoord2f(0,0); glVertex3f(-4.3, -.9, -4.3);
      glTexCoord2f(1,0); glVertex3f(4.3, -.9, -4.3);
      glTexCoord2f(1,1); glVertex3f(4.3, -.9, -4);
      glTexCoord2f(0,1); glVertex3f(-4.3, -.9, -4);

      glEnd();
                  // Roadway

      glBindTexture(GL_TEXTURE_2D,texture[8]);
      // Front Road
      glBegin(GL_QUADS);

      glTexCoord2f(0.0, 0.0); glVertex3f(-10,-.95, .4);
      glTexCoord2f(1.0, 0.0); glVertex3f(10, -.95, .4);
      glTexCoord2f(1.0, 1.0); glVertex3f(10, -.95, .1);
      glTexCoord2f(0.0, 1.0);glVertex3f(-10,-.95, .1);

      glEnd();

      // Back Road

      glBegin(GL_QUADS);

      glTexCoord2f(0.0, 0.0); glVertex3f(-10,-.95, -.6);
      glTexCoord2f(1.0, 0.0); glVertex3f(10, -.95, -.6);
      glTexCoord2f(1.0, 1.0); glVertex3f(10, -.95, -.9);
      glTexCoord2f(0.0, 1.0);glVertex3f(-10,-.95, -.9);

      glEnd();

      // Right Road

      glBegin(GL_QUADS);

      glTexCoord2f(0,0); glVertex3f(2, -.95, -4);
      glTexCoord2f(1,0); glVertex3f(2.3, -.95, -4);
      glTexCoord2f(1,1); glVertex3f(2.3, -.95, +4);
      glTexCoord2f(0,1); glVertex3f(2, -.95, +4);

      glEnd();

      // Left Road

      glBegin(GL_QUADS);

      glTexCoord2f(0,0); glVertex3f(-2, -.95, -4);
      glTexCoord2f(1,0); glVertex3f(-2.3, -.95, -4);
      glTexCoord2f(1,1); glVertex3f(-2.3, -.95, +4);
      glTexCoord2f(0,1); glVertex3f(-2, -.95, +4);
      glEnd();

      glPopMatrix();

                  // Airport

      glBindTexture(GL_TEXTURE_2D,texture[24]);
      glBegin(GL_QUADS);

      // Grass

      glTexCoord2f(0,0); glVertex3f(-4, -.93, -4);
      glTexCoord2f(1,0); glVertex3f(4.3, -.93, -4);
      glTexCoord2f(1,1); glVertex3f(4.3, -.93, -9);
      glTexCoord2f(0,1); glVertex3f(-4, -.93, -9);

      glEnd();

      hangar(-3.3, -1, -7.9, .5, 1, .5, 90);

      hangar(-2, -1, -7.9, .5, 1, .5, 90);

      hangar(2.4, -1, -7.9, .5, 1, .5, 90);

      hangar(3.7, -1, -7.9, .5, 1, .5, 90);

      airplane(0, -.7, -5.25, .2, .8, .2, 90);
      wing1(-.5, -.6, -5.1, 1, 1, .8, 35);
      wing1(.5, -.6, -5.1, .7, 1, .5, 35);

      wing2(-.5, -.6, -5.3, 1, 1, .8, -35);
      wing2(.4, -.6, -5.3, .7, 1, .6, -35);

      cockpit(-.7, -.7, -5.25, 1, 1, 1, -90);

      fin(.3, -.7, -5.25, .8, .9, 1, 0);



      airTrafficBuilding(.5, -.5, -7, 1, .5, 1, 0);
      genericTower4(1.7, -.7, -8, 6, .3, 1, 0);

      // Runway
      glBindTexture(GL_TEXTURE_2D,texture[8]);
      glBegin(GL_QUADS);

      glTexCoord2f(0,0); glVertex3f(-3.8, -.90, -6);
      glTexCoord2f(1,0); glVertex3f(4.1, -.90, -6);
      glTexCoord2f(1,1); glVertex3f(4.1, -.90, -6.5);
      glTexCoord2f(0,1); glVertex3f(-3.8, -.90, -6.5);

      glTexCoord2f(0,0); glVertex3f(-2.8, -.90, -5.5);
      glTexCoord2f(1,0); glVertex3f(3.2, -.90, -5.5);
      glTexCoord2f(1,1); glVertex3f(3.2, -.90, -5);
      glTexCoord2f(0,1); glVertex3f(-2.8, -.90, -5);

      glEnd();

                  // City 
      // Center
      twintower1(0, 0, 0, 1, 1, 1, 0);
      twintower2();
      boxTower();

      // Front
      pointyTower(0, 0, 0, 1, 1, 1, 0);
      genericTower2(-.5, -.3, 1, 1, .8, 1, 0);
      genericTower5(1.5, -.3, 1, 1, .8, 1, 0);
      genericTower1(.5, -.4, 2, 1, .6, 1, 0);
      genericTower4(-.5, -.4, 2, 1, .6, 1, 0);
      genericTower8(1.5, -.4, 2, 1, .6, 1, 0);
      genericTower7(.5, -.6, 3, 1, .4, 1, 0);
      genericTower8(-.5, -.6, 3, 1, .4, 1, 0);
      twintower1(1.5, -.6, 3, 1, .4, 1, 0);

      // Back
      genericTower7(1.5, -.2, -1, 1, .8, 1, 0);
      genericTower8(-.5, -.2, -1, 2, .8, 1, 0);
      twintower1(1.5, -.4, -2, 1, .6, 1, 0);
      genericTower1(-.5, -.4, -2, 1, .6, 1, 0);
      genericTower4(1.5, -.6, -3, 1, .4, 1, 0);
      genericTower7(.5, -.6, -3, 2, .4, 1, 0);
      genericTower4(-1, -.6, -3, 1, .4, 1, 0);
   

      // Left Center
      genericTower7(-.8, -.2, 0, 1, .8, 1, 0);
      genericTower3(-1.5, -.2, 0, 1, .8, 1, 0);
      genericTower2(-2.5, -.3, 0, 1, .7, 1, 0);
      genericTower5(-3.3, -.3, 0, 1, .7, 1, 0);

      // Right Center
      genericTower8(1.8, -.2, 0, 1, .8, 1, 0);
      genericTower1(3, -.2, 0, 1, .8, 1, 0);
      genericTower4(3.8, -.3, 0, 1, .7, 1, 0);

      // Right Front
      pointyTower(2.5, -.3, 0, 1, .7, 1, 0);
      genericTower6(3.8, -.3, 1, 1, .7, 1, 0);
      genericTower3(3, -.4, 2.2, 1, .6, 1, 0);
      genericTower2(3.8, -.4, 2.2, 1, .6, 1, 0);
      genericTower5(3, -.6, 3.5, 1, .4, 1, 0);
      genericTower7(3.8, -.6, 3.5, 1, .4, 1, 0);

      // Left Front
      genericTower5(-2.5, -.4, 1, 1, .6, 1, 0);
      twintower1(-3.3, -.4, 1, 1, .6, 1, 0);
      genericTower8(-2.5, -.5, 2.2, 1, .5, 1, 0);
      genericTower2(-3.3, -.5, 2.2, 1, .5, 1, 0);
      genericTower7(-2.5, -.6, 3.5, 1, .4, 1, 0);
      genericTower6(-3.3, -.6, 3.5, 1, .4, 1, 0);

      // Right Back
      genericTower5(3, -.4, -1, 1, .6, 1, 0);
      twintower1(3.8, -.4, -1, 1, .6, 1, 0);
      genericTower8(3, -.5, -2.2, 1, .5, 1, 0);
      genericTower2(3.8, -.5, -2.2, 1, .5, 1, 0);
      genericTower7(3, -.6, -3.5, 1, .4, 1, 0);
      genericTower6(3.8, -.6, -3.5, 1, .4, 1, 0);

      // Left Back
      pointyTower(-2.8, -.3, -2, 1, .7, 1, 0);
      genericTower6(-3.3, -.3, -1, 1, .7, 1, 0);
      genericTower3(-2.5, -.4, -2.2, 1, .6, 1, 0);
      genericTower2(-3.3, -.4, -2.2, 1, .6, 1, 0);
      genericTower5(-2.5, -.6, -3.5, 1, .4, 1, 0);
      genericTower7(-3.3, -.6, -3.5, 1, .4, 1, 0);

               // Suburbs Right
      // Center
      home2(6.5, -.7, -.3, .2, .3, .2, 0);
      home1(7.0, -.7, -.3, .2, .3, .2, 0);
      home2(7.5, -.7, -.3, .2, .3, .2, 0);
      home1(8, -.7, -.3, .2, .3, .2, 0);
      home2(8.5, -.7, -.3, .2, .3, .2, 0);
      home1(9, -.7, -.3, .2, .3, .2, 0);
      home2(9.5, -.7, -.3, .2, .3, .2, 0);

      // Rear
      home1(6.5, -.7, -1.3, .2, .3, .2, 0);
      home2(7.0, -.7, -1.3, .2, .3, .2, 0);
      home1(7.5, -.7, -1.3, .2, .3, .2, 0);
      home2(8, -.7, -1.3, .2, .3, .2, 0);
      home1(8.5, -.7, -1.3, .2, .3, .2, 0);
      home2(9, -.7, -1.3, .2, .3, .2, 0);
      home1(9.5, -.7, -1.3, .2, .3, .2, 0);

      // Front 
      home1(6.5, -.7, .9, .2, .3, .2, 0);
      home2(7.0, -.7, .9, .2, .3, .2, 0);
      home1(7.5, -.7, .9, .2, .3, .2, 0);
      home2(8, -.7, .9, .2, .3, .2, 0);
      home1(8.5, -.7, .9, .2, .3, .2, 0);
      home2(9, -.7, .9, .2, .3, .2, 0);
      home1(9.5, -.7, .9, .2, .3, .2, 0);

      fence(0, -.9, 0, 1, .1, 1, 0);
      fence(0, -.9, -3.5, 1, .1, 1, 0);
      //fence(0, -.9, -1, 1, .1, 1, -90);

      fence2(0, -.9, 0, 1, .1, 1, 0);

      // Suburbs Left

      // Center
      home4(-6.5, -.7, -.3, .2, .3, .2, 0);
      home3(-7.0, -.7, -.3, .2, .3, .2, 0);
      home4(-7.5, -.7, -.3, .2, .3, .2, 0);
      home3(-8, -.7, -.3, .2, .3, .2, 0);
      home4(-8.5, -.7, -.3, .2, .3, .2, 0);
      home3(-9, -.7, -.3, .2, .3, .2, 0);
      home4(-9.5, -.7, -.3, .2, .3, .2, 0);

      // Rear
      home3(-6.5, -.7, -1.3, .2, .3, .2, 0);
      home4(-7.0, -.7, -1.3, .2, .3, .2, 0);
      home3(-7.5, -.7, -1.3, .2, .3, .2, 0);
      home4(-8, -.7, -1.3, .2, .3, .2, 0);
      home3(-8.5, -.7, -1.3, .2, .3, .2, 0);
      home4(-9, -.7, -1.3, .2, .3, .2, 0);
      home3(-9.5, -.7, -1.3, .2, .3, .2, 0);

      // Front 
      home3(-6.5, -.7, .9, .2, .3, .2, 0);
      home4(-7.0, -.7, .9, .2, .3, .2, 0);
      home3(-7.5, -.7, .9, .2, .3, .2, 0);
      home4(-8, -.7, .9, .2, .3, .2, 0);
      home3(-8.5, -.7, .9, .2, .3, .2, 0);
      home4(-9, -.7, .9, .2, .3, .2, 0);
      home3(-9.5, -.7, .9, .2, .3, .2, 0);

      fence(-16, -.9, 0, 1, .1, 1, 0);
      fence(-16, -.9, -3.5, 1, .1, 1, 0);
      fence2(-12.2, -.9, 0, 1, .1, 1, 0);

      fence3(0, -.9, 0, 1, .1, 1, 0);


      // Mountains
      //mountain1(-8, 0, -3.5, 1, 1, 1, 0);
      mountain1(-9, 1, -9, 2, 2, 2, 0);
      mountain2(-8, .5, -9, 1.5, 1.5, 1.5, 0);
      mountain1(-6.9, .4, -9, 1.4, 1.4, 1.4, 0);
      mountain2(-5.5, 1, -9, 2, 2, 2, 0);

      mountain2(-9, .5, -7, 1.5, 1.5, 1.5, 0);
      mountain1(-8, .4, -7, 1.4, 1.4, 1.4, 0);
      mountain2(-6.9, .3, -7, 1.3, 1.3, 1.3, 0);
      mountain1(-5.5, .5, -7, 1.5, 1.5, 1.5, 0);
      
      mountain1(9, 1, -9, 2, 2, 2, 0);
      mountain2(8, .5, -9, 1.5, 1.5, 1.5, 0);
      mountain1(6.9, .4, -9, 1.4, 1.4, 1.4, 0);
      mountain2(5.5, 1, -9, 2, 2, 2, 0);

      mountain2(9, .5, -7, 1.5, 1.5, 1.5, 0);
      mountain1(8, .4, -7, 1.4, 1.4, 1.4, 0);
      mountain2(6.9, .3, -7, 1.3, 1.3, 1.3, 0);
      mountain1(5.5, .5, -7, 1.5, 1.5, 1.5, 0);

      // front

      mountain1(-9, 1, 9, 2, 2, 2, 0);
      mountain2(-8, .5, 9, 1.5, 1.5, 1.5, 0);
      mountain1(-6.9, .4, 9, 1.4, 1.4, 1.4, 0);
      mountain2(-5.5, 1, 9, 2, 2, 2, 0);

      mountain2(-9, .5, 7, 1.5, 1.5, 1.5, 0);
      mountain1(-8, .4, 7, 1.4, 1.4, 1.4, 0);
      mountain2(-6.9, .3, 7, 1.3, 1.3, 1.3, 0);
      mountain1(-5.5, .5, 7, 1.5, 1.5, 1.5, 0);
      
      mountain1(9, 1, 9, 2, 2, 2, 0);
      mountain2(8, .5, 9, 1.5, 1.5, 1.5, 0);
      mountain1(6.9, .4, 9, 1.4, 1.4, 1.4, 0);
      mountain2(5.5, 1, 9, 2, 2, 2, 0);

      mountain2(9, .5, 7, 1.5, 1.5, 1.5, 0);
      mountain1(8, .4, 7, 1.4, 1.4, 1.4, 0);
      mountain2(6.9, .3, 7, 1.3, 1.3, 1.3, 0);
      mountain1(5.5, .5, 7, 1.5, 1.5, 1.5, 0);

      tree(-4.7, -.75, -1.2, 1, 1, 1, 0);
      tree(-5.5, -.75, -1.2, 1, 1, 1, 0);
      tree(-4.7, -.75, .7, 1, 1, 1, 0);
      tree(-5.5, -.75, .7, 1, 1, 1, 0);

      tree(-4.7, -.75, -2, 1, 1, 1, 0);
      tree(-4.7, -.75, -3, 1, 1, 1, 0);
      tree(-4.7, -.75, -4, 1, 1, 1, 0);
      tree(-4.7, -.75, -5, 1, 1, 1, 0);

      tree(-4.7, -.75, 1.5, 1, 1, 1, 0);
      tree(-4.7, -.75, 2.3, 1, 1, 1, 0);
      tree(-4.7, -.75, 3.2, 1, 1, 1, 0);
      tree(-4.7, -.75, 4, 1, 1, 1, 0);

      tree(4.7, -.75, -1.2, 1, 1, 1, 0);
      tree(5.5, -.75, -1.2, 1, 1, 1, 0);
      tree(4.7, -.75, .7, 1, 1, 1, 0);
      tree(5.5, -.75, .7, 1, 1, 1, 0);

      tree(4.7, -.75, -2, 1, 1, 1, 0);
      tree(4.7, -.75, -3, 1, 1, 1, 0);
      tree(4.7, -.75, -4, 1, 1, 1, 0);
      tree(4.7, -.75, -5, 1, 1, 1, 0);

      tree(4.7, -.75, 1.5, 1, 1, 1, 0);
      tree(4.7, -.75, 2.3, 1, 1, 1, 0);
      tree(4.7, -.75, 3.2, 1, 1, 1, 0);
      tree(4.7, -.75, 4, 1, 1, 1, 0);

      tree(-9, -.75, -5, 1, 1, 1, 0);
      tree(-6, -.75, -5, 1, 1, 1, 0);
      tree(-9, -.75, -2.5, 1, 1, 1, 0);
      tree(-6, -.75, -2.5, 1, 1, 1, 0);

      tree(9, -.75, -5, 1, 1, 1, 0);
      tree(6, -.75, -5, 1, 1, 1, 0);
      tree(9, -.75, -2.5, 1, 1, 1, 0);
      tree(6, -.75, -2.5, 1, 1, 1, 0);

      tree(-9, -.75, 5, 1, 1, 1, 0);
      tree(-6, -.75, 5, 1, 1, 1, 0);
      tree(-9, -.75, 2.5, 1, 1, 1, 0);
      tree(-6, -.75, 2.5, 1, 1, 1, 0);

      tree(9, -.75, 5, 1, 1, 1, 0);
      tree(6, -.75, 5, 1, 1, 1, 0);
      tree(9, -.75, 2.5, 1, 1, 1, 0);
      tree(6, -.75, 2.5, 1, 1, 1, 0);

      lake(-7.5, -.9, -3.7, 1, 1, 1, 90);
      lake(7.5, -.9, -3.7, 1, 1, 1, 90);

      lake(-7.5, -.9, 3.7, 1, 1, 1, 90);
      lake(7.5, -.9, 3.7, 1, 1, 1, 90);

      island(-7.5, -1, -3.8, .4, .4, .4, 0);
      home1(-7.5, -.55, -3.8, .1, .1, .1, 0);

      island(7.5, -1, -3.8, .4, .4, .4, 0);
      home2(7.5, -.55, -3.8, .1, .1, .1, 0);

      island(-7.5, -1, 3.8, .4, .4, .4, 0);
      home3(-7.5, -.55, 3.8, .1, .1, .1, 0);

      island(7.5, -1, 3.8, .4, .4, .4, 0);
      home4(7.5, -.55, 3.8, .1, .1, .1, 0);
      
      glEnd();
      
      glColor3f(1, 1, 1);

      glBindTexture(GL_TEXTURE_2D,texture[7]);
      glBegin(GL_QUADS);

      // Ocean

      glTexCoord2f(0,0); glVertex3f(-4, -.93, 5);
      glTexCoord2f(1,0); glVertex3f(4.3, -.93, 5);
      glTexCoord2f(1,1); glVertex3f(4.3, -.93, 10);
      glTexCoord2f(0,1); glVertex3f(-4, -.93, 10);

      glEnd();


      // Bridge

      glDisable(GL_TEXTURE_2D);

      bridge(-.2, -.5, 4.2, 1, 1, 1, 0);
      bridge(.4, -.5, 4.2, 1, 1, 1, 0);
      
      crossbeam(.1, -.1, 4.2, 1.2, 1, 1, 90);
      crossbeam(.1, -.5, 4.2, 1.2, 1, 1, 90);

      bridgewire(-.1, -.5, 4.6, 1.2, 1, 1, -60);
      bridgewire(.3, -.5, 4.6, 1.2, 1, 1, -60);

      bridgewire(-.1, -.5, 5.5, 1.2, 1, 1, 60);
      bridgewire(.3, -.5, 5.5, 1.2, 1, 1, 60);

      bridge(-.2, -.5, 6, 1, 1, 1, 0);
      bridge(.4, -.5, 6, 1, 1, 1, 0);

      crossbeam(.1, -.1, 6, 1, 1, 1, 90);
      crossbeam(.1, -.5, 6, 1, 1, 1, 90);

      bridgewire(-.1, -.5, 6.5, 1.2, 1, 1, -60);
      bridgewire(.3, -.5, 6.5, 1.2, 1, 1, -60);

      bridgewire(-.1, -.5, 7.5, 1.2, 1, 1, 60);
      bridgewire(.3, -.5, 7.5, 1.2, 1, 1, 60);

      bridge(-.2, -.5, 8, 1, 1, 1, 0);
      bridge(.4, -.5, 8, 1, 1, 1, 0);

      crossbeam(.1, -.1, 8, 1, 1, 1, 90);
      crossbeam(.1, -.5, 8, 1, 1, 1, 90);

      bridgewire(-.1, -.5, 8.5, 1.2, 1, 1, -60);
      bridgewire(.3, -.5, 8.5, 1.2, 1, 1, -60);

      bridge(-.2, -.5, 10, 1, 1, 1, 0);
      bridge(.4, -.5, 10, 1, 1, 1, 0);

      crossbeam(.1, -.1, 10, 1, 1, 1, 90);
      crossbeam(.1, -.5, 10, 1, 1, 1, 90);

      bridgewire(-.1, -.5, 9.5, 1.2, 1, 1, 60);
      bridgewire(.3, -.5, 9.5, 1.2, 1, 1, 60);      

      //glEnable(GL_TEXTURE_2D);
      //glBindTexture(GL_TEXTURE_2D,texture[8]);
      glColor3f(.5, .5, .5);

       glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex3f(-.15, -.75, 4);
      glTexCoord2f(1,0); glVertex3f(.35, -.75, 4);
      glTexCoord2f(1,1); glVertex3f(.35, -.75, 10);
      glTexCoord2f(0,1); glVertex3f(-.15, -.75, 10);

      glTexCoord2f(0,0); glVertex3f(-.15, -1, 3.5);
      glTexCoord2f(1,0); glVertex3f(.35, -1, 3.5);
      glTexCoord2f(1,1); glVertex3f(.35, -.75, 4);
      glTexCoord2f(0,1); glVertex3f(-.15, -.75, 4);

      glEnd();

      boat(1, -.05, 8, 1, 1, 1, 90);

      boat(-4, -.05, 7, 1, 1, 1, 90);

      exhaust(-2.25, -.4, 7, 1, 1, 1, 0);
      exhaust(-2.25, -.4, 6.4, 1, 1, 1, 0);
      exhaust(-2.25, -.4, 7.7, 1, 1, 1, 0);


      exhaust(2.75, -.4, 8, 1, 1, 1, 0);
      exhaust(2.75, -.4, 7.4, 1, 1, 1, 0);
      exhaust(2.75, -.4, 8.6, 1, 1, 1, 0);




}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=9;  //  Length of axes
   //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   //  Set view angle
   //  Perspective - set eye position
  if(fp_mode){

      xCoord = +2*dim*Sin(view);
      zCoord = -2*dim*Cos(view);

      gluLookAt(Fpx,Fpy,Fpz, xCoord+Fpx,Fpy,zCoord+Fpz, 0,1,0);
   }
   else{
   //  Set view angle
   //  Perspective - set eye position
      if (mode)
      {
         double Ex = -2*dim*Sin(th)*Cos(ph);
         double Ey = +2*dim        *Sin(ph);
         double Ez = +2*dim*Cos(th)*Cos(ph);

         gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
      }
      //  Orthogonal - set world orientation
      else
      {
         glRotatef(ph,1,0,0);
         glRotatef(th,0,1,0);
      }
   }

   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)

      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);

   //  Flat or smooth shading
   grid(0,0,0,  0.3, 0.3, 0.3, 0);

   
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
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

   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }

   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   if(!fp_mode){
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
      th -= 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
         th += 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP)
         ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN)
         ph -= 5;
      //  PageUp key - increase dim
      else if (key == GLUT_KEY_PAGE_DOWN)
         dim += 0.1;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_UP && dim>1)
         dim -= 0.1;
      //  Smooth color model
      else if (key == GLUT_KEY_F1)
         smooth = 1-smooth;
      //  Local Viewer
      else if (key == GLUT_KEY_F2)
         local = 1-local;
      else if (key == GLUT_KEY_F3)
         distance = (distance==1) ? 5 : 1;
      //  Toggle ball increment
      else if (key == GLUT_KEY_F8)
         inc = (inc==10)?3:10;
      //  Flip sign
      else if (key == GLUT_KEY_F9)
         one = -one;
       th %= 360;
       ph %= 360;
   }

   //  Update projection
   myProject();
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

   else if (ch == 'f' || ch == 'F'){
      fp_mode = 1 - fp_mode;
   }
   else if(fp_mode){
      if (ch == 'w' || ch == 'W'){
         Fpx += xCoord*0.045; //Update the eye vector based on the camera vector
         Fpz += zCoord*0.045;
      }
      //  Left arrow key - decrease angle by 5 degrees
      else if (ch == 's' || ch == 'S'){
         Fpx -= xCoord*0.045;
         Fpz -= zCoord*0.045;
      }
      //  Up arrow key - increase elevation by 5 degrees
      else if (ch == 'a' || ch == 'A'){
         view -= 4;
      }
      //  Down arrow key - decrease elevation by 5 degrees
      else if (ch == 'd' || ch == 'D'){
         view += 4;
      }
      view %= 360;
   }
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;

   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   myProject();
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   myProject();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(1100,900);
   glutCreateWindow("Peter Lindee - Final Project: City");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   texture[0] = LoadTexBMP("brick.bmp");
   texture[1] = LoadTexBMP("roof.bmp");
   texture[2] = LoadTexBMP("window.bmp");
   texture[3] = LoadTexBMP("door.bmp");
   texture[4] = LoadTexBMP("ground.bmp");
   texture[5] = LoadTexBMP("skyscraper1.bmp");
   texture[6] = LoadTexBMP("skyscraper5.bmp");
   texture[7] = LoadTexBMP("water.bmp");
   texture[8] = LoadTexBMP("road.bmp");
   texture[9] = LoadTexBMP("skyscraper2.bmp");
   texture[10] = LoadTexBMP("skyscraper3.bmp");
   texture[11] = LoadTexBMP("skyscraper4.bmp");
   texture[12] = LoadTexBMP("skyscraper6.bmp");
   texture[13] = LoadTexBMP("skyscraper7.bmp");
   texture[14] = LoadTexBMP("skyscraper8.bmp");
   texture[15] = LoadTexBMP("skyscraper9.bmp");
   texture[16] = LoadTexBMP("skyscraper10.bmp");
   texture[17] = LoadTexBMP("skybox_bk.bmp");
   texture[18] = LoadTexBMP("stucco.bmp");
   texture[19] = LoadTexBMP("house3.bmp");
   texture[20] = LoadTexBMP("house4.bmp");
   texture[21] = LoadTexBMP("wooden_fence.bmp");
   texture[22] = LoadTexBMP("desertsky_lf.bmp");
   texture[23] = LoadTexBMP("desertsky_rt.bmp");
   texture[24] = LoadTexBMP("grass.bmp");
   texture[25] = LoadTexBMP("concrete.bmp");
   texture[26] = LoadTexBMP("airplane.bmp");
   texture[27] = LoadTexBMP("mountain.bmp");
   texture[28] = LoadTexBMP("mountain2.bmp");

   ErrCheck("init");
   glutMainLoop();
   return 0;
}

