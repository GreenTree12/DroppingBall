#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#include<GL/glut.h>


#define pi (double)2*acos((double)0)

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double z = 60, iniHeight = 60, g = 9.8, iniVelocity, t=0.0;
int up=1;

struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 70,0,0);
			glVertex3f(-70,0,0);
			glVertex3f(0,-70,0);
			glVertex3f(0, 70,0);
			glVertex3f(0,0, 70);
			glVertex3f(0,0,-70);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-6;i<=6;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -70, 0);
				glVertex3f(i*10,  70, 0);

				//lines parallel to X-axis
				glVertex3f(-70, i*10, 0);
				glVertex3f( 70, i*10, 0);
			}
		}glEnd();
	}
}


void drawsphere(float radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));	
		r=sqrt(radius*radius-h*h);
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
		
	}
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			if((j%2==0 && i%2==0) || (j%2!=0 && i%2!=0))
				glColor3f(1,1,1);
			else
				glColor3f(0,0,1);
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
				
			}glEnd();
		}
		
	}
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':	
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;	
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/ 
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,-1,150,	0,0,0,	0,0,1);
	
	
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
	glColor3f(1,0,0);
    //Bouncing Ball--------------------------------------------
	glPushMatrix();{
	glTranslated(0,0,z+8);
	drawsphere(8,30,20);
	glRotatef(180,1,0,0);
	drawsphere(8,30,20);
	}glPopMatrix();
	
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	if(up==1)
	{
		z = iniHeight - (.5*g*t*t);
		iniVelocity = (g*t);
		t += .01;
		if(z<=0.0)
		{
			up = 0;
			t = 0;
		}
	}
	else
	{
		double z1;
		z = iniVelocity*t - (.5*g*t*t);
		t += .01;
		if(z>=iniHeight)
		{
			up = 1;
			t = 0.0;
			iniVelocity = 0;
		}
	}
	//codes for any changes in Models, Camera
	//sleep(60);
	glutPostRedisplay();
	//Timer(0);
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=100.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
	
	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 700);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
