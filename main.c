#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "texture.h"

#define PI 3.14159265358979323846

//control variables
static double inc = 0,  
              fov = 85;

//distances from sun
static double d_mercurio = 1.8/10.0,
              d_venus = 3.2/10.0,
              d_terra = 4.5/10.0,
              d_marte = 6.8/10.0,
              d_jupiter = 15.0/10.0,
              d_saturno = 28.5/10.0,
              d_urano = 57.0/10.0,
              d_netuno = 90.0/10.0;

//radius of each planet
static double r_mercurio = 0.02,
              r_venus = 0.03,
              r_terra = 0.05,
              r_marte = 0.04,
              r_jupiter = 0.1,
              r_saturno = 0.09,
              r_urano = 0.07,
              r_netuno = 0.07;

//rotation time of each planet
float rot_mercurio = 0.0,
      rot_venus = 0.0,
      rot_terra = 0.0,
      rot_marte = 0.0,
      rot_jupiter = 0.0,
      rot_saturno = 0.0,
      rot_urano = 0.0,
      rot_netuno = 0.0;

//translation time of each planet
float t_mercurio = 0.0,
      t_venus = 0.0,
      t_terra = 0.0,
      t_marte = 0.0,
      t_jupiter = 0.0,
      t_saturno = 0.0,
      t_urano = 0.0,
      t_netuno = 0.0;

// set center of scene
GLfloat xref=0, yref=0, zref=0;

GLuint texture[10];

GLfloat origin[] = {0.f, 0.f, 0.f, 0.f};

void init() {
    //habilita a iluminação da cena
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_POSITION, origin);

    //habilita o uso de texturas
    glEnable(GL_TEXTURE_2D );
	glEnable(GL_COLOR_MATERIAL);
    glGenTextures(10, texture);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture[0] = LoadBitmap("media/Sol/Sol.bmp");
	texture[1] = LoadBitmap("media/Mercurio/Mercurio.bmp");
	texture[2] = LoadBitmap("media/Venus/Venus.bmp");
	texture[3] = LoadBitmap("media/Tierra/Tierra.bmp");
	texture[4] = LoadBitmap("media/Marte/Marte.bmp");
	texture[5] = LoadBitmap("media/Jupiter/Jupiter.bmp");
	texture[6] = LoadBitmap("media/Saturno/Saturno.bmp");
	texture[7] = LoadBitmap("media/Urano/Urano.bmp");
	texture[8] = LoadBitmap("media/Neptuno/Neptuno.bmp");
	texture[9] = LoadBitmap("media/Tierra/Luna.bmp");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void drawCircunferences(GLfloat x, GLfloat y, GLfloat radius){
	int N_points = 200; 
	GLfloat twoPI = 2.0f * PI;
	glLineWidth(0.5f);
	glPushMatrix();
	glRotatef((GLfloat) 90.0, 1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
    for(int i = 0; i <= N_points; i++) { 
        glVertex2f(
            /*cria os pontos de uma circunferência e os une por uma linha*/        
            x + (radius * cos(i * twoPI / N_points)), 
            y + (radius * sin(i * twoPI / N_points))
        );
    }
	glEnd();
	glPopMatrix();
}

void drawMoon(GLUquadric *qobj, GLfloat t_rot, GLfloat t_trans, float radius) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[9]);

    glRotatef(25, 0.0, 0.0, 0.0);
    glRotatef(t_rot, 0.0, 1.0, 0.0);
    glTranslatef(0.38, 0.0, 0.0);

    gluSphere(qobj, radius, 60, 60);
    
    glDisable(GL_TEXTURE_2D);
}

void drawPlanets(GLfloat t_rot, GLfloat t_trans, float radius, float dist2sun, int hasMoon, GLuint planet_texture) {

    GLUquadric *qobj = gluNewQuadric();

	gluQuadricOrientation(qobj, GLU_OUTSIDE);	
	gluQuadricDrawStyle(qobj, GLU_FILL);

    gluQuadricTexture(qobj,GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, planet_texture);
    
    glPushMatrix();
        glRotatef(0, 0.0, 0.0, 0.0); 
        glRotatef(t_trans, 0.0, 1.0, 0.0);
        glTranslatef(dist2sun, 0.0, 0.0);

        glPushMatrix();
            glRotatef(t_rot, 0.0, 1.0, 0.0);
            glRotated(-90, 1, 0, 0); 
            gluSphere(qobj, radius, 60, 60); //planet
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //if a planet has moon
        if(hasMoon) {
            drawMoon(qobj, 0.2, 0.5, 0.013);
		}

    gluDeleteQuadric(qobj);
    glPopMatrix();
}

void display(void) {
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    GLUquadric *qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);

    gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

    glEnable(GL_DEPTH_TEST);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
	
	glPushMatrix();
	glRotated(-90, 1, 0, 0);    
    gluSphere(qobj, 0.12, 60, 60); //sol
	gluDeleteQuadric(qobj);
	glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    drawCircunferences(0, 0, d_mercurio);
    drawPlanets(rot_mercurio, t_mercurio, r_mercurio, d_mercurio, 0, texture[1]);

    drawCircunferences(0, 0, d_venus);
    drawPlanets(rot_venus, t_venus, r_venus, d_venus, 0, texture[2]);

    drawCircunferences(0, 0, d_terra);
    drawPlanets(rot_terra, t_terra, r_terra, d_terra, 1, texture[3]);

    drawCircunferences(0, 0, d_marte);
    drawPlanets(rot_marte, t_marte, r_marte, d_marte, 0, texture[4]);

    drawCircunferences(0, 0, d_jupiter);
    drawPlanets(rot_jupiter, t_jupiter,  r_jupiter, 0, d_jupiter, texture[5]);

    drawCircunferences(0, 0, d_saturno);
    drawPlanets(rot_saturno, t_saturno, r_saturno, 0, d_saturno, texture[6]);

    drawCircunferences(0, 0, d_urano);
    drawPlanets(rot_urano, t_urano, r_urano, d_urano, 0, texture[7]);

    drawCircunferences(0, 0, d_netuno);
    drawPlanets(rot_netuno, t_netuno, r_netuno, d_netuno, 0, texture[8]);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (GLfloat) w/(GLfloat) h, 1.0, 40.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  15.0+inc, 5, 0, 
                xref, yref, zref, 
                0.0, 1.0, 0.0 );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );	//melhora a qualidade dos gráficos
    glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y) {
    switch (key) {
        case 's':
            xref += 0.2;
            reshape(1000,700);
            break;
        case 'w':
            xref -= 0.2;
            reshape(1000,700);
            break;
        case 'a':
            zref += 0.2;
            reshape(1000,700);
            break;
        case 'd':
            zref -= 0.2;
            reshape(1000,700);
            break;
        case '-':
            if(fov < 110)
                fov += 5;
            reshape(1000,700);
            break;
        case '+':
            if(fov > 0)
                fov -= 5;
            reshape(1000,700);
            break;
        case 'i':
            if(inc > 5)
                inc -= 1;
            else if(inc < 15)
                inc += 1;
            reshape(1000,700);
            break;
        case 27:
            exit(0);
   }
}

void idle(void) {
    //rotation
	rot_mercurio -= 0.008,
    rot_venus += 0.002,
    rot_terra -= 0.5,
    rot_marte -= 0.5,
    rot_jupiter -= 1,
    rot_saturno -= 1,
    rot_urano -= 0.7,
    rot_netuno -= 0.7;

    //translation
    t_mercurio -= 0.47;
    t_venus -= 0.35;
    t_terra -= 0.30;
    t_marte -= 0.24;
    t_jupiter -= 0.13;
    t_saturno -= 0.003;
    t_urano -= 0.002;
    t_netuno -= 0.0023;

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1000, 700); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow(argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}