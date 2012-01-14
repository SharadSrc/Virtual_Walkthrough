/**Header Files including libraries of opengl and c**/

#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // Header file for sleeping.
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include <math.h>       // Header file for trigonometric functions.

/**Header Files Including parts of Guest House**/

#include "pil.cpp"
#include "imageloader.cpp"
#include "railing.cpp"
#include "model.cpp"
#include "stairs.cpp"
#include "room.cpp"
#include "bed.cpp"
#include "couch.cpp"
#include "side_couch.cpp"
#include "table.cpp"
#include "fridge.cpp"
#include "tv.cpp"
#include "clock.cpp"
#include "ac.cpp"
#include "back_ac.cpp"
#include "window.cpp"

/**defining codes for key movement**/

#define ESCAPE 27
#define PAGE_UP 73
#define PAGE_DOWN 81
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77

/* The number of our GLUT window */
int window; 

//GLuint texture[3];       // storage for 3 textures;
//GLuint texture[3];       // storage for 3 textures;

int light = 0;           // lighting on/off
int blend = 0;        // blending on/off

GLfloat xrot;            // x rotation
GLfloat yrot;            // y rotation
GLfloat xspeed;          // x rotation speed
GLfloat yspeed;          // y rotation speed

GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;

GLfloat lookupdown = 0.0;
const float piover180 = 0.0174532925f;

float heading, xpos, zpos;

GLfloat camx = 0, camy = 0, camz = 0; // camera location.
GLfloat therotate;

GLfloat z=0.0f;                       // depth into the screen.

GLfloat LightAmbient[]  = {0.5f, 0.5f, 0.5f, 1.0f}; 
GLfloat LightDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};

float lightAmbient[] = { 0.2f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

float matAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

float lightPosition[] = { 2.0f, 2.0f, 3.0f, 0.0f };
float lightDirection[] = { -2.0f, -2.0f, -3.0f };

GLuint filter = 0;       // texture filtering method to use (nearest, linear, linear + mipmaps)
// degrees to radians...2 PI radians = 360 degrees
float rad(float angle)
{
	return angle * piover180;
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLsizei Width, GLsizei Height)	// We call this right after our OpenGL window is created.
{
	LoadGLTextures();                           // load the textures.
	LoadTexture_Fridge();
	LoadTexture_tv();
	LoadTexture_clock();
	LoadTexture_ac();
	LoadTexture_back();
	LoadTexture_win();

	LoadTexture_Wall();
	glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);          // Set the blending function for translucency (note off at init time)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// This Will Clear The Background Color To Black
	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);                       // type of depth test to do.
	glEnable(GL_DEPTH_TEST);                    // enables depth testing.
	glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);

	// set up lights.
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
		Height=1;

	glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
GLvoid DrawGLScene(GLvoid)
{
	GLfloat xtrans, ztrans, ytrans;
	GLfloat sceneroty,scenerotx;

	gluLookAt(0,0,5,0,0,0,0,1,0);
	//	glEnable(GL_LIGHTING);
	//	glEnable(GL_LIGHT0);

	//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);

	//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 2.0f);

	//	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	//	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	//	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

	//	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 1.2f);

	//	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.0f);

	// calculate translations and rotations.
	xtrans = -xpos;
	ztrans = -zpos;
	ytrans = -walkbias-0.25f;
	sceneroty = 360.0f - yrot;
	scenerotx = 360.0f - xrot;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();

	glRotatef(lookupdown, 1.0f, 0, 0);
	glRotatef(sceneroty, 0, 1.0f, 0);
	glRotatef(scenerotx,1.0f,0,0);

	glTranslatef(xtrans, ytrans, ztrans);    

	glBindTexture(GL_TEXTURE_2D, texture[filter]);    // pick the texture.


	model();  // create the external model of the house

	make_Railing(); // create the railing on the first floor of the house
     
     	glColor3f(0.80f, 0.80f, 0.80f);
	glPushMatrix();
	glTranslatef(-3,1.02,2.3);
	glRotatef(90,1,0,0);
	glRotatef(90,0,0,1);
	gluPartialDisk(gluNewQuadric(), 0, 2, 32, 32,0,180);
	glPopMatrix();

	glColor3f(0.65f, 0.50f, 0.39f);
	glPushMatrix();
	glTranslatef(-3,-0.02,2.3);
	glRotatef(90,1,0,0);
	glRotatef(90,0,0,1);
	gluPartialDisk(gluNewQuadric(), 0, 2, 32, 32,0,180);
	glPopMatrix();
	float i;

	glColor3f(0.7,0.7,0.7);
	/**Code for making pillars in lower floor**/
//	for(i = -6.9533333;i <= -5.45 ; i+=1.45){
		renderCylinder_convenient(-6.9533,0,3,-6.9533,1,3,0.04,16);
//	}
		renderCylinder_convenient(-6.0,0,3,-6.0,1,3,0.04,16);
		renderCylinder_convenient(-4.85,0,3.0,-4.85,1,3.0,0.04,16);
		renderCylinder_convenient(-3.7,0,4.1,-3.7,1,4.1,0.04,16);
		renderCylinder_convenient(-2.42,0,4.2,-2.42,1,4.2,0.04,16);
		renderCylinder_convenient(-1.09,0,3.0,-1.09,1,3.0,0.04,16);
		
	for(i = 0.35;i <= 2.95 ; i+=1.45){
		renderCylinder_convenient(i,0,3,i,1,3,0.04,16);
	}
	renderCylinder_convenient(3.05,0,3,3.05,1,3,0.04,16);
	renderCylinder_convenient(4.45,0,3,4.45,1,3,0.04,16);

	/**Code ends for pillars on low floor**/

	/**Code for making pillars in first floor**/
	for(i = -6.9533333;i <= 2.95 ; i+=1.45){
		renderCylinder_convenient(i,1,1.5,i,2,1.5,0.04,16);
	}
	renderCylinder_convenient(3.05,1,2.95,3.05,2,2.95,0.04,16);
	renderCylinder_convenient(4.45,1,2.95,4.45,2,2.95,0.04,16);
	/**Code ends for pillars on first floor**/

	// since this is double buffered, swap the buffers to display what just got drawn.

	makeStairs();// Code for Creating The Stairs

	/**Code For Creating The Rooms**/
	glColor3f(0.75,0.75,0.75);
	for(i = -7.0f;i < 3.0f; i += 2.0f){
		render_Walls_Lower(i,0.0f,1.0f,-3.0f);
		render_Walls_Upper(i,1.0f,0.2f,-3.0f);
	}

	for(i = -5.05f;i < 3.0f;i += 2.0f){
		glPushMatrix();
		glTranslated(i, 0.0, -0.5);
		glRotated(90, 0.0, 1.0, 0.0);
		renderBed();
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(i, 1.01, -0.5);
		glRotated(90, 0.0, 1.0, 0.0);
		renderBed();
		glPopMatrix();
	}

	////COUCH

	for(i = -5.4f;i<3.0f;i += 2.0f){
		glPushMatrix();
		glTranslated(i, 0.0, 0.4);
		glRotated(90, 0.0, 1.0, 0.0);
		renderCouch();
		glPopMatrix();
	}

	//SIDE_COUCH

	for(i = -5.4f;i<3.0f;i += 2.0f){
		glPushMatrix();
		glTranslated(i, 0.0, 0.75);
//		glRotated(540, 0.0, 1.0, 0.0);
		sideCouch();
		glPopMatrix();
	}
	//TABLE
	for(i = -5.4f;i<3.0f;i += 2.0f){
		glPushMatrix();
		glTranslated(i,0.0,0.85);
		glRotatef(90,0.0,1.0,0.0);
		renderTable();
		glPopMatrix();
	}
	for(i = -6.75;i < 3.0f ; i += 2.0f){
		glPushMatrix();
		glTranslatef(i,0.0,-1.2);
		glRotatef(90,0,1,0);
		fridge(0.0,0.0,0.0);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(i,1.0,-1.2);
		glRotatef(90,0,1,0);
		fridge(0.0,0.0,0.0);
		glPopMatrix();
	}
	
	for(i = -6.98;i < 3.0f ; i += 2.0f){
		glPushMatrix();
		glTranslatef(i,0.5,-0.7);
		glRotatef(90,0,1,0);
		tv(0.0,0.0,0.0);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(i,1.5,-0.7);
		glRotatef(90,0,1,0);
		tv(0.0,0.0,0.0);
		glPopMatrix();
	}
	
	for(i = -6.98;i < 3.0f ; i += 2.0f){
		glPushMatrix();
		glTranslatef(i,0.5,0.6);
		glRotatef(90,0,1,0);
		clock(0.0,0.0,0.0);
		glPopMatrix();	
	}

	for(i = -6.65;i < 3.0f ; i += 2.0f){
		glPushMatrix();
		glTranslatef(i,0.8,0.985);
		glRotatef(180,0,1,0);
		render_ac(0,0,0);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(i,1.8,0.18);
		glRotatef(180,0,1,0);
		render_ac(0,0,0);
		glPopMatrix();
	}

	for(i = -6.80;i < 3.0f ; i += 2.0f){
		render_back(i,0.8,1.056);
		render_back(i,1.8,0.274);
	}
	
	for(i = -6.80;i < 3.0f ; i += 2.0f){
		render_window(i,0.3,1.012);
		render_window(i+0.8,0.3,0.008);
		render_window(i,1.3,0.23);

		glPushMatrix();
		glTranslatef(i,0.3,0.97);
		glRotatef(180,0,1,0);
		glTranslatef(-0.4,0,0);
		render_window(0,0,0);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(i+0.8,0.3,-0.03);
		glRotatef(180,0,1,0);
		glTranslatef(-0.4,0,0);
		render_window(0,0,0);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(i,1.3,0.18);
		glRotatef(180,0,1,0);
		glTranslatef(-0.4,0,0);
		render_window(0,0,0);
		glPopMatrix();

	}
	glutSwapBuffers();
}


/* The function called whenever a normal key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
	/* avoid thrashing this procedure */
	usleep(100);

	switch (key) {    
		case ESCAPE: // kill everything.
			/* exit the program...normal termination. */
			exit(1);                   	
			break; // redundant.

		case 'b': 
		case 'B': // switch the blending
			printf("B/b pressed; blending is: %d\n", blend);
			blend = blend ? 0 : 1;              // switch the current value of blend, between 0 and 1.
			if (blend) {
				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
			} else {
				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
			}
			printf("Blending is now: %d\n", blend);
			break;

		case 'f': 
		case 'F': // switch the filter
			printf("F/f pressed; filter is: %d\n", filter);
			filter++;                           // switch the current value of filter, between 0/1/2;
			if (filter > 2) {
				filter = 0;
			}
			printf("Filter is now: %d\n", filter);
			break;

		case 'l': 
		case 'L': // switch the lighting
			printf("L/l pressed; lighting is: %d\n", light);
			light = light ? 0 : 1;              // switch the current value of light, between 0 and 1.
			if (light) {
				glEnable(GL_LIGHTING);
			} else {
				glDisable(GL_LIGHTING);
			}
			printf("Lighting is now: %d\n", light);
			break;

		case 'w':
		case 'W':
			walkbias += 0.05f;
			break;	

		case 's':
		case 'S':
			walkbias -= 0.05f;
			break;
		case 'a':
		case 'A': // walk forward (bob head)
			zpos -= (float)sin(yrot*piover180) * 0.05f;
			xpos -= (float)cos(yrot*piover180) * 0.05f;	


			if (walkbiasangle >= 359.0f)
				walkbiasangle = 0.0f;	
			else 
				walkbiasangle+= 10;
			//			walkbias = (float)sin(walkbiasangle * piover180)/20.0f;

			break;
		case 'd':	
		case 'D': // walk forward (bob head)
			zpos += (float)sin(yrot*piover180) * 0.05f;
			xpos += (float)cos(yrot*piover180) * 0.05f;	


			if (walkbiasangle >= 359.0f)
				walkbiasangle = 0.0f;	
			else 
				walkbiasangle+= 10;
			//			walkbias = (float)sin(walkbiasangle * piover180)/20.0f;

			break;
		default:
			printf ("Key %d pressed. No action there yet.\n", key);
			break;
	}	
}

/* The function called whenever a normal key is pressed. */
void specialKeyPressed(int key, int x, int y) 
{
	/* avoid thrashing this procedure */
	usleep(100);

	switch (key) {    
		case GLUT_KEY_PAGE_UP: // tilt up
			z -= 0.2f;
			lookupdown -= 1.0f;
			break;

		case GLUT_KEY_PAGE_DOWN: // tilt down
			z += 0.2f;
			lookupdown += 1.0f;
			break;

		case GLUT_KEY_UP: // walk forward (bob head)
			xpos -= (float)sin(yrot*piover180) * 0.05f;
			zpos -= (float)cos(yrot*piover180) * 0.05f;	
			if (walkbiasangle >= 359.0f)
				walkbiasangle = 0.0f;	
			else 
				walkbiasangle+= 10;
			//			walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
			break;

		case GLUT_KEY_DOWN: // walk back (bob head)
			xpos += (float)sin(yrot*piover180) * 0.05f;
			zpos += (float)cos(yrot*piover180) * 0.05f;	
			if (walkbiasangle <= 1.0f)
				walkbiasangle = 359.0f;	
			else 
				walkbiasangle-= 10;
			//			walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
			break;

		case GLUT_KEY_LEFT: // look left
			yrot += 1.5f;
			break;

		case GLUT_KEY_RIGHT: // look right
			yrot -= 1.5f;
			break;

		default:
			printf ("Special key %d pressed. No action there yet.\n", key);
			break;
	}
}

int main(int argc, char **argv) 
{  
	/* load our world from disk */
	SetupWorld();

	/* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
	   X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */  
	glutInit(&argc, argv);  

	/* Select type of Display mode:   
	   Double buffer 
	   RGBA color
	   Depth buffer 
	   Alpha blending */  
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA );  
	
	//Antialiashing //
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	/* get a 640 x 480 window */
	glutInitWindowSize(640, 480);  

	/* the window starts at the upper left corner of the screen */
	glutInitWindowPosition(0, 0);  
	/* Open a window */  
	window = glutCreateWindow("GVC GuestHouse Project");  

	/* Register the function to do all our OpenGL drawing. */
	glutDisplayFunc(&DrawGLScene);  

	/* Go fullscreen.  This is as soon as possible. */
	glutFullScreen();

	/* Even if there are no events, redraw our gl scene. */
	glutIdleFunc(&DrawGLScene); 

	/* Register the function called when our window is resized. */
	glutReshapeFunc(&ReSizeGLScene);

	/* Register the function called when the keyboard is pressed. */
	glutKeyboardFunc(&keyPressed);

	/* Register the function called when special keys (arrows, page down, etc) are pressed. */
	glutSpecialFunc(&specialKeyPressed);

	/* Initialize our window. */
	InitGL(640, 480);

	/* Start Event Processing Engine */  
	glutMainLoop();  

	return 1;
}
