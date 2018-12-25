#include<GL\glut.h>
#include<glm\glm.hpp>
#include<glm\ext.hpp>
#include<windows.h>       //for exit() and Sleep()  
#include<cmath>

double p1[3]{ 1,1,-1 };
double p2[3]{ -4,4,4 };
double angle = 0;

void draw_coordinate()
{
	glMatrixMode(GL_MODELVIEW);
	// O
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.05, 50, 50);
	glPopMatrix();
	// X
	glPushMatrix();
	//glColor3f(0.0f, 1.0f, 0.0f);
	glTranslatef(25, 0, 0);
	glScalef(1.0f, 0.0005f, 0.0005f);
	glutSolidCube(50);
	glPopMatrix();
	// Y
	glPushMatrix();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0, 25, 0);
	glScalef(0.0005f, 1.0f, 0.0005f);
	glutSolidCube(50);
	glPopMatrix();
	// Z
	glPushMatrix();
	//glColor3f(0.0f, 0.0f, 1.0f);
	glTranslatef(0, 0, 25);
	glScalef(0.0005f, 0.0005f, 1.0f);
	glutSolidCube(50);
	glPopMatrix();
}

void draw_rotation_axis()
{
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);//画线
	glBegin(GL_LINES);
	glVertex3f(p1[0], p1[1], p1[2]);
	glVertex3f(p2[0], p2[1], p2[2]);
	glEnd();

	glPopMatrix();
}

void draw_cube()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(-p1[0], -p1[1], -p1[2]);
	glRotatef(angle, p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
	glTranslatef(p1[0], p1[1], p1[2]);
	glTranslatef(0, 3, 0);
	angle += 2;

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	//glColor3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	//glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	//glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	//glColor3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);

	glColor3f(0.0, 1.0, 0.0);
	//glColor3f(1.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	//glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	//glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	//glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);

	glColor3f(0.0, 0.0, 1.0);
	//glColor3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	//glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	//glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	//glColor3f(1.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);

	glColor3f(1.0, 1.0, 0.0);
	//glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	//glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	//glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	//glColor3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);

	glColor3f(1.0, 0.0, 1.0);
	//glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	//glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	//glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	//glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);

	glColor3f(0.0, 1.0, 1.0);
	//glColor3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	//glColor3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	//glColor3f(1.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	//glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glEnd();
	glPopMatrix();
}


/*void rotate() {
	double vlen = sqrt(pow(p2[0] - p1[0], 2) + pow(p2[1] - p1[1], 2) + pow(p2[2] - p1[2], 2));
	double v[3]{ (p2[0] - p1[0])/vlen,(p2[1] - p1[1])/vlen,(p2[2] - p1[2])/vlen};
	double a = v[0], b = v[1], c = v[2], d = sqrt(b*b + c*c);
	GLfloat M[16]{ d,-a*b / d,-a*c / d,0, 0,c / d,-b / d,0,a,b,c,0,0,0,0,1 };
	GLfloat MT[16]{ d,0,a,0,-a*b / d,c / d,b,0,-a*c / d,-b / d,c,0,0,0,0,1 };
	glTranslatef(p1[0], p1[1], p1[2]);
	glMultMatrixf(M);
	glRotatef(angle, 0, 0, 1.0);
	glMultMatrixf(MT);
	glTranslatef(-p1[0], -p1[1], -p1[2]);
}*/

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 transform_camera(1.0f); // 摄像机的位置和定向，即摄像机在世界坐标系中位置
	transform_camera = glm::affineInverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	glm::mat4 model_view_matrix = glm::affineInverse(transform_camera);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&model_view_matrix[0][0]);
	draw_coordinate();

	draw_rotation_axis();

	draw_cube();

	glFlush();
	glutSwapBuffers();
	Sleep(20);
}

void reshape(int w, int h)
{
	if (h == 0) h = 1;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);

}
void init(int width, int height)
{
	if (height == 0) height = 1;
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	//gluLookAt(-1, 1, 1, 0, 0, 0, -1, -1, 2);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(700, 500);
	glutCreateWindow("Cube");

	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutReshapeFunc(reshape);
	init(640, 480);
	glutMainLoop();
	return 0;
}