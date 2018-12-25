#include<GL\glut.h>
#include<glm\glm.hpp>
#include<glm\ext.hpp>
#include<windows.h>       //for exit() and Sleep()  
#include<cmath>

//double p1[3]{ 0,0,0 };
//double p2[3]{ 0,1,0 };
double angle = 0;

void display(void)
{
	glm::mat4 transform_camera(1.0f); // 摄像机的位置和定向，即摄像机在世界坐标系中位置
	transform_camera = glm::affineInverse(glm::lookAt(glm::vec3(0, 5, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	glm::mat4 model_view_matrix = glm::affineInverse(transform_camera);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&model_view_matrix[0][0]);
	glPushMatrix();
	//glColor3f(1.0f, 0.0f, 0.0f);
	glRotatef(angle, 0, 1.0, 0);
	angle += 2;
	glutSolidTeapot(3);
	glPopMatrix();
	
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

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	//设置光照  
	const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };//环境强度
	const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };//镜面强度

	const GLfloat white_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };//散射强度
	const GLfloat red_light_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };//散射强度

	const GLfloat red_light_position[] = { 5.0f, 5.0f, 3.0f, 0.0f };
	const GLfloat white_light_position[] = { -5.0f, 5.0f, 3.0f, 0.0f };

	const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	const GLfloat high_shininess[] = { 50.0f };

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, red_light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, red_light_position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, white_light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
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