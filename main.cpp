


#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"

using namespace std;

const float BUILDING_WIDTH = 7.0f; //The length of x and z dimension of the building
const float BUILDING_HEIGHT = BUILDING_WIDTH*10;
float _angle = 0;            //The rotation of the building
GLuint _textureId,_textureId1;           //The OpenGL id of the texture
GLuint _displayListId;
float r,g,b;
const float SCALEX=0.15f;
const float SCALEZ=0.15f;
float ALPHA = 1.0f; //The opacity of each face of the cube
//The forward position relative to the floor
float _pos = 0;
float xpos=0, ypos=0, zpos=0;
const float FLOOR_TEXTURE_SIZE = 15.0f; //The size of each floor "tile"
int _mipmaptextureId;
float xangle,zangle;

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
		case 'w':
			ypos++;
			break;
		case 's':
			ypos--;
			break;
		case 'a':
			xpos--;
			break;
		case 'd':
			xpos++;
			break;
		case 'i':
			zpos++;
			break;
		case 'k':
			zpos--;
			break;
		case '4':
			_angle--;
			break;
		case '6':
			_angle++;
			break;
	}
}


void handleSpecialKeypress(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			xangle--;
			break;
		case GLUT_KEY_RIGHT:
			xangle++;
			break;
		case GLUT_KEY_UP:
			zangle++;
			break;
		case GLUT_KEY_DOWN:
			zangle--;
			break;
	}
}


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	return textureId;
}


GLuint loadMipmappedTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  GL_RGB,
					  image->width, image->height,
					  GL_RGB,
					  GL_UNSIGNED_BYTE,
					  image->pixels);
	return textureId;
}


void drawPyramid(){
	float Pyramid_Height = 10.0f;
	float scale = 1.0f;
	//glShadeModel(GL_SMOOTH); //Enable smooth shading
	glPushMatrix();
	glTranslatef(0,BUILDING_HEIGHT*(1+3/100.0f)/2,0);
	glBegin(GL_TRIANGLES);
	glColor3f(1,1,1);
	glNormal3f(0,0,1);
	glVertex3f(0,Pyramid_Height,0);


	glVertex3f(-BUILDING_WIDTH*scale / 2, 0, BUILDING_WIDTH*scale / 2);
	//glNormal3f(-1,1,0);

	glVertex3f(-BUILDING_WIDTH*scale / 2, 0, -BUILDING_WIDTH*scale / 2);
	glVertex3f(0,Pyramid_Height,0);
	glVertex3f(-BUILDING_WIDTH*scale / 2, 0, -BUILDING_WIDTH*scale / 2);
	//glNormal3f(0,1,-1);

	glVertex3f(BUILDING_WIDTH*scale / 2, 0, -BUILDING_WIDTH*scale / 2);
	glVertex3f(0,Pyramid_Height,0);
	glVertex3f(BUILDING_WIDTH*scale / 2, 0, -BUILDING_WIDTH*scale / 2);
	//glNormal3f(1,1,0);
	glVertex3f(BUILDING_WIDTH*scale / 2, 0, BUILDING_WIDTH*scale / 2);
	glVertex3f(0,Pyramid_Height,0);
	glVertex3f(BUILDING_WIDTH*scale / 2, 0, BUILDING_WIDTH*scale / 2);

	//glNormal3f(0,1,1);
	glVertex3f(-BUILDING_WIDTH*scale / 2, 0, BUILDING_WIDTH*scale / 2);

	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(BUILDING_WIDTH*scale / 2, 0, BUILDING_WIDTH*scale / 2);
	glVertex3f(-BUILDING_WIDTH*scale / 2, 0, BUILDING_WIDTH*scale / 2);
	glVertex3f(-BUILDING_WIDTH*scale / 2, 0, -BUILDING_WIDTH*scale / 2);
	glVertex3f(+BUILDING_WIDTH*scale / 2, 0, -BUILDING_WIDTH*scale / 2);


	glEnd();
	glPopMatrix();

	glShadeModel(GL_FLAT);
}


void drawFloor(){
	glPushMatrix();
	float scale = 20.0f;
	glTranslatef(0,-BUILDING_HEIGHT*(1+1/20.0f)/2,0);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _mipmaptextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,
					GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);



	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, _pos / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, 0.0f, -1000.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, (2000 + _pos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(0.0f, (2000 + _pos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(0.0f, _pos / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, 0.0f, -1000.0f);


	glEnd();
	glDisable(GL_TEXTURE_2D);

	glTranslatef(0,2.0f,BUILDING_WIDTH*2.25f);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);


	float pathLt = 3.5f*BUILDING_WIDTH;
	float pathWt = 0.5f*BUILDING_WIDTH;
	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-pathWt / 2, 0, +pathLt / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-pathWt / 2, 0, -pathLt / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(+pathWt / 2, 0, -pathLt / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pathWt / 2, 0, pathLt / 2);


	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

}

void drawWater(){

	glPushMatrix();
	glTranslatef(0,-BUILDING_HEIGHT*(1+1/20.0f)/2,0);
	glTranslatef(3/4.0f*BUILDING_WIDTH,0,3/4.0f*BUILDING_WIDTH);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);


	float pathLt = BUILDING_WIDTH/4.0f;
	float pathWt = BUILDING_WIDTH/4;
	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-pathWt / 2, 0, +pathLt / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-pathWt / 2, 0, -pathLt / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(+pathWt / 2, 0, -pathLt / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(pathWt / 2, 0, pathLt / 2);


	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void drawCube(GLuint _textureId, float r, float g, float b,float alpha){

	glBegin(GL_QUADS);

	//glColor3f(0.0f,0.0f,0.0f);
	//Top face
	glColor4f(r,g,b, 1.0f);
	glNormal3f(0.0, 1.0f, 0.0f);
	glVertex3f(-BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);
	glVertex3f(-BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);
	glVertex3f(BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);
	glVertex3f(BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);

	//Bottom face
	glColor4f(r,g,b, 1.0f);
	glNormal3f(0.0, -1.0f, 0.0f);
	glVertex3f(-BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);
	glVertex3f(BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);
	glVertex3f(BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);
	glVertex3f(-BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);

	//Left face
	glNormal3f(-1.0, 0.0f, 0.0f);
	glColor4f(r,g,b, alpha);
	glVertex3f(-BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);
	glVertex3f(-BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);
	glVertex3f(-BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);
	glVertex3f(-BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);

	//Right face
	glNormal3f(1.0, 0.0f, 0.0f);
	glColor4f(r,g,b, alpha);
	glVertex3f(BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);
	glVertex3f(BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);
	glVertex3f(BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);
	glVertex3f(BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);


	glEnd();

	glBegin(GL_QUADS);

	//Front face
	glNormal3f(0.0, 0.0f, 1.0f);
	glColor4f(r,g,b, alpha);
	//glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);
	//glTexCoord2f(1.0f, 0.0f);
	glVertex3f(BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);
	//glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);
	//glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, BUILDING_WIDTH / 2);

	//Back face
	glNormal3f(0.0, 0.0f, -1.0f);
	glColor4f(r,g,b, alpha);
	//glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);
	//glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);
	//glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BUILDING_WIDTH / 2, BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);
	//glTexCoord2f(0.0f, 1.0f);
	glVertex3f(BUILDING_WIDTH / 2, -BUILDING_HEIGHT / 2, -BUILDING_WIDTH / 2);

	glEnd();
	//glDisable(GL_TEXTURE_2D);




}

void drawWalls(){

	r = 0.125f;
	g=0;
	b=0;
	for (int i=0;i<4;i++){
		glPushMatrix();
		float yscale = 0.4f;
		float xscale = 8.0f;
		float zscale = SCALEZ;
		float Zoff = BUILDING_WIDTH*4;
		float Xoff = 0;
		if(i%2)	Zoff *= -1;
		if(i>=2){
			Xoff = Zoff;
			Zoff =0;
			xscale = SCALEZ;
			zscale = 8.0f;
		}
		glTranslatef(Xoff,-BUILDING_HEIGHT*(1+1/20.0f)/2, Zoff);
		glScalef(xscale, yscale, zscale);
		drawCube(_textureId, r,g,b,ALPHA);
		glPopMatrix();
	}
}




void drawtop(){

	for(int i=0; i<2; i++){
		float yoff = BUILDING_HEIGHT*(1+1/60.0f)/2;
		if(i == 1)			yoff *= -1;
		glPushMatrix();
		glTranslatef(0,yoff,0);
		glScalef(1+2*SCALEX, 1/60.0f,1+2*SCALEX);
		drawCube(_textureId,r,g,b,ALPHA);
		glPopMatrix();
	}

	for(int i=0; i<4; i++){
		float scaley = 1/60.0f;
		float scalex = 1+2*SCALEX;
		float scalez = SCALEZ/9;

		float Yoff= BUILDING_HEIGHT*(1+3*scaley)/2;
		float Zoff= BUILDING_WIDTH*(1+2*SCALEZ-scalez)/2;
		float Xoff = 0;

		if(i%2)	Zoff *= -1;
		if(i>=2)	{
			scalex = scalez;
			scalez = 1+2*SCALEX;
			Xoff = Zoff;
			Zoff = 0;

		}



		glPushMatrix();

		glTranslatef( Xoff,Yoff, Zoff );
		glScalef( scalex, scaley, scalez);
		drawCube(_textureId,r,g,b,ALPHA);
		glPopMatrix();


	}

}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //Change the background to grey
	//glShadeModel(GL_SMOOTH); //Enable smooth shading
	glEnable(GL_BLEND); //Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function

	glEnable(GL_CULL_FACE);



	Image* image = loadBMP("floor.bmp");
	_mipmaptextureId = loadMipmappedTexture(image);
	delete image;

	Image* image1 = loadBMP("texture.bmp");
	_textureId = loadTexture(image1);
	delete image1;

	/*
	Image* image2 = loadBMP("vtr.bmp");
	_textureId1 = loadTexture(image2);
	delete image2;
	*/

	/*
	//Set up a display list for drawing a cube
	_displayListId = glGenLists(1); //Make room for the display list
	glNewList(_displayListId, GL_COMPILE); //Begin the display list
	drawCube(_textureId,r,g,b); //Add commands for drawing a cube to the display list
	glEndList(); //End the display list
	*/

}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}


void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-xpos, -ypos, -20.0f-zpos);



	GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightPos[] = {-2 * BUILDING_WIDTH, BUILDING_HEIGHT/1.5f, 4 * BUILDING_WIDTH, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	/*
	GLfloat lightColor1[] = {0, 0, 1.0f, 1.0f};
	GLfloat lightPos1[] = {-BUILDING_WIDTH, BUILDING_HEIGHT,  BUILDING_WIDTH, 1.0f};
	//GLfloat lightPos1[] = {0,0,0, 0.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	*/

	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
	glRotatef(xangle, 1.0f, 0.0f, 0.0f);
	glRotatef(zangle, 0.0f, 0.0f, 1.0f);

	//draw the main body
	//Call the display list for drawing a cube

	r=0;
	g=0;
	b=0;
	ALPHA = 1.0f;
	//glCallList(_displayListId);
	glCullFace(GL_FRONT);
	drawCube(_textureId,r,g,b,ALPHA);
	glCullFace(GL_BACK);
	drawCube(_textureId,r,g,b,ALPHA);

	//drawPyramid();
	drawFloor();
	drawWalls();
	//drawGarden();
	//drawWater();

	ALPHA = 1.0f;
	r=1;
	g=1;
	b=1;
	float temp;

	for(int i=0;i<8;i++){

		float Xoff=BUILDING_WIDTH*(1 + SCALEX)/2;
		//float Xoff=BUILDING_WIDTH*(1 - SCALEX)/2;
		float Zoff=BUILDING_WIDTH*(1 + SCALEZ)/2;


		if(i%2)	Zoff *= -1;
		if(i==2 || i==3 || i==6 || i==7)	Xoff *= -1;

		if(i>3)	{
			temp = Zoff;
			Zoff = Xoff;
			Xoff = temp;
		}
		//float rotAngle = (i>3)	? 90.0f : 0.0f;
		glPushMatrix(); //Save the transformations performed thus far
		glTranslatef(Xoff, 0.0f,Zoff );

		if(i<3)	glScalef(SCALEX, 1, SCALEZ);
		else glScalef(SCALEZ, 1, SCALEX);		//Scale by given amt in the x, y, and z directions
		//glRotatef(rotAngle, 0.0f, 1.0f, 0.0f);

		drawCube(_textureId,r,g,b,ALPHA);		//glCallList(_displayListId);
		glPopMatrix();

	}

	float gap;
	for(int i=0 ; i<20 ; i++){
		float scale = 0.05f;
		if(!(i%5))	gap = 0.2f;
		float Xoff= BUILDING_WIDTH*(1-gap)/2;
		float Zoff= BUILDING_WIDTH*(1+scale+scale/3)/2;
		if(i>=5 && i<10)	Zoff *= -1;
		if(i>=10)	{
			temp = Zoff;
			Zoff = Xoff;
			Xoff = temp;
		}
		if (i>=15)	Xoff *= -1;
		glPushMatrix();

		glTranslatef(Xoff, 0.0f,Zoff );
		glScalef(scale, 1, scale);
		drawCube(_textureId,r,g,b,ALPHA);
		glPopMatrix();
		gap += 0.4f;

	}


	for(int i=0 ; i<80 ; i++){
		float scalez = 0.05f/3;
		float scaley = 1/100.0f;
		float scalex = 1;
		if(!(i%20))	gap = 0.05f;
		float Yoff= BUILDING_HEIGHT*(1-gap)/2;
		float Zoff= BUILDING_WIDTH*(1+scalez)/2;
		float Xoff = 0;
		if(i>=20 && i<40)	Zoff *= -1;
		if(i>=40)	{
			scalex = scalez;
			scalez = 1;
			Xoff = Zoff;
			Zoff = 0;

		}
		if (i>=60)	Xoff *= -1;


		glPushMatrix();

		glTranslatef( Xoff,Yoff, Zoff );
		glScalef( scalex, scaley, scalez);
		drawCube(_textureId,r,g,b,ALPHA);
		glPopMatrix();
		gap += 0.1f;

	}

	drawtop();
	glPushMatrix();
	glTranslatef(0,BUILDING_HEIGHT/9.0f,0);
	glScalef(0.6f, 0.8f, 0.6f);
	drawtop();
	glPopMatrix();

	r=0.0f;	b=0.0f;	g=0.0f;
	glPushMatrix();

	glTranslatef(-2.0f, BUILDING_HEIGHT*(1+3/50.0f)/2   , -1.2f);
	glScalef(SCALEX, 1/100.0f, SCALEX);
	drawCube(_textureId,r,g,b,ALPHA);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, BUILDING_HEIGHT*(1+3/50.0f)/2   , -1.2f);
	glScalef(SCALEX, 1/100.0f, SCALEX);
	drawCube(_textureId,r,g,b,ALPHA);
	glTranslatef(0,0,0.8f*2/SCALEX);
	drawCube(_textureId,r,g,b,ALPHA);
	glPopMatrix();


	glutSwapBuffers();
}

//Called every 25 milliseconds
void update(int value) {
	_angle += 1.0f;
	if (_angle > 360) {
		_angle -= 360;
	}
	if (zangle > 360) {
		_angle -= 360;
	}
	if (xangle > 360) {
		_angle -= 360;
	}

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);

	glutCreateWindow("3D Building Graphics Project");
	initRendering();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecialKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);

	glutMainLoop();
	return 0;
}








