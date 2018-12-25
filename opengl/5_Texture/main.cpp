#include<GL\glut.h>
#include<glm\glm.hpp>
#include<glm\ext.hpp>
#include<windows.h>       //for exit() and Sleep()  
#include<cmath>

//double p1[3]{ 0,0,0 };
//double p2[3]{ 0,1,0 };
double angle = 0;
//double angle_cube = -45;
GLubyte image[64 * 64 * 3];
GLubyte surround[64 * 64 * 3];

int win_w =500;
int win_h = 500;

bool isFirst = true;
bool cube_rotate = false;

void draw_teapot(double size) {
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	
	glColor3f(1.0f, 0.0f, 0.0f);
	
	if (isFirst) {
		glm::mat4 transform_camera = glm::affineInverse(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 20), glm::vec3(0, 1, 0)));
		glm::mat4 model_view_matrix = glm::affineInverse(transform_camera);
		glLoadMatrixf(&model_view_matrix[0][0]);
		glPushMatrix();
		glRotatef(-angle, 0, 1.0, 0);
		glTranslatef(0, 0, 10);
	}
	glTranslatef(0, 0, 10);
	glRotatef(angle, 0, 1.0, 0);
	angle += 1;
	if (angle >= 360) angle = angle - 360;
	glutSolidTeapot(size);
	
	if (isFirst) {
		glPopMatrix();
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, win_w, win_h, 1);
	}
	glPopMatrix();
}

void draw_cube(double size)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0, 0, -10);
	glRotatef(angle, 0, 1.0, 0);
	/*if(cube_rotate)
		angle_cube+=2;*/
	/*if(isFirst)
		angle += 2;
	if (angle >= 360) angle = angle - 360;*/

	double beg, end;
	beg = isFirst ? 0.2 : 0;
	end = isFirst ? 0.8 : 1.0;

	glBegin(GL_QUADS);
	if (!isFirst) {
		glColor3f(1.0, 1.0, 1.0);//上
		glTexCoord2f(end, end); glVertex3f(size / 2, size / 2, -size / 2);
		glTexCoord2f(end, beg); glVertex3f(-size / 2, size / 2, -size / 2);
		glTexCoord2f(beg, beg); glVertex3f(-size / 2, size / 2, size / 2);
		glTexCoord2f(beg, end); glVertex3f(size / 2, size / 2, size / 2);
		//glColor3f(0.0, 1.0, 0.0);//下
		glTexCoord2f(beg, end); glVertex3f(size / 2, -size / 2, size / 2);
		glTexCoord2f(end, end); glVertex3f(-size / 2, -size / 2, size / 2);
		glTexCoord2f(end, beg); glVertex3f(-size / 2, -size / 2, -size / 2);
		glTexCoord2f(beg, beg); glVertex3f(size / 2, -size / 2, -size / 2);
	}
	//glColor3f(0.0, 0.0, 1.0);//前
	if (isFirst) {
		glTexCoord2f(beg, end);glVertex3f(size / 2, size / 2, size / 2);
		glTexCoord2f(end, end);glVertex3f(-size / 2, size / 2, size / 2);
		glTexCoord2f(end, beg);glVertex3f(-size / 2, -size / 2, size / 2);
		glTexCoord2f(beg, beg);glVertex3f(size / 2, -size / 2, size / 2);
	}

	//glColor3f(1.0, 1.0, 0.0);//后
	if (!isFirst) {
		glTexCoord2f(end, beg); glVertex3f(size / 2, -size / 2, -size / 2);
		glTexCoord2f(beg, beg); glVertex3f(-size / 2, -size / 2, -size / 2);
		glTexCoord2f(beg, end); glVertex3f(-size / 2, size / 2, -size / 2);
		glTexCoord2f(end, end); glVertex3f(size / 2, size / 2, -size / 2);
	}
	//glColor3f(1.0, 0.0, 1.0);//左
	if (!isFirst) {
		glTexCoord2f(end, end); glVertex3f(-size / 2, size / 2, size / 2);
		glTexCoord2f(beg, end); glVertex3f(-size / 2, size / 2, -size / 2);
		glTexCoord2f(beg, beg); glVertex3f(-size / 2, -size / 2, -size / 2);
		glTexCoord2f(end, beg); glVertex3f(-size / 2, -size / 2, size / 2);
	}
	//glColor3f(0.0, 1.0, 1.0);//右
	if (!isFirst) {
		glTexCoord2f(beg, end); glVertex3f(size / 2, size / 2, -size / 2);
		glTexCoord2f(end, end); glVertex3f(size / 2, size / 2, size / 2);
		glTexCoord2f(end, beg); glVertex3f(size / 2, -size / 2, size / 2);
		glTexCoord2f(beg, beg); glVertex3f(size / 2, -size / 2, -size / 2);
	}
	Sleep(10);
	glEnd();
	glPopMatrix();
}

void display(void)
{
	// 摄像机的位置和定向，即摄像机在世界坐标系中位置
	glm::mat4 transform_camera = glm::affineInverse(glm::lookAt(glm::vec3(20, 10, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	glm::mat4 model_view_matrix = glm::affineInverse(transform_camera);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&model_view_matrix[0][0]);
	isFirst = true;
	draw_teapot(2);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_cube(6);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	isFirst = false;
	draw_cube(6);
	draw_teapot(2);

	glFlush();
	glutSwapBuffers();

	Sleep(20);
}

void reshape(int w, int h)
{
	if (h == 0) h = 1;
	
	win_w = w;
	win_h = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);

}
void init(int width, int height)
{
	if (height == 0) height = 1;

	//glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	//设置光照  
	const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };//环境强度
	const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };//镜面强度

	const GLfloat white_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };//散射强度
	//const GLfloat red_light_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };//散射强度

	//const GLfloat red_light_position[] = { 5.0f, 5.0f, 3.0f, 0.0f };
	const GLfloat white_light_position[] = { -5.0f, 5.0f, 3.0f, 0.0f };

	const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	const GLfloat high_shininess[] = { 50.0f };

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/

	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, white_light_position);

	/*glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, red_light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, red_light_position);*/

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


	glDepthFunc(GL_LEQUAL);
	
	int i, j, c;
	for (i = 0; i<64; i++)
	{
		for (j = 0; j<64; j++)
		{
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			image[i*64*3+j*3+0] = (GLubyte)c;
			image[i*64*3+j*3+1] = (GLubyte)c;
			image[i*64*3+j*3+2] = (GLubyte)c;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case ' ':
		cube_rotate = !cube_rotate;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(win_w, win_h);
	glutCreateWindow("Cube");

	glutDisplayFunc(display);
	glutIdleFunc(display);

	//处理(ESC)退出,和(Space)开关自动旋转
	glutKeyboardFunc(keyboard);

	glutReshapeFunc(reshape);
	init(640, 480);
	glutMainLoop();
	return 0;
}