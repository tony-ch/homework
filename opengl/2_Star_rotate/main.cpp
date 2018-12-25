#include <GL/glut.h>
#include <cmath>
#include <iostream>

float angle = 0.0;
bool auto_rotate = true;


void display(void)
{

	//表示出各个点的坐标，设半径 
	double R = 0.3;
	const double PI = acos(0.) * 2;
	double L1 = R*cos(36 * PI / 180);
	double L2 = R*sin(36 * PI / 180);
	double L3 = R*cos(72 * PI / 180);
	double L4 = R*sin(72 * PI / 180);
	double L5 = L2*tan(72 * PI / 180);
	double L6 = L2 / cos(72 * PI / 180);
	double L7 = L6*sin(54 * PI / 180);
	double L8 = L6*cos(54 * PI / 180) + R;

	//保存当前模型视图矩阵。
	glPushMatrix();
	
	
	//平移到左上角
	glTranslatef(-L2 - L6, L1, 0);
	//旋转
	glRotatef(angle, 0.0, 0.0, 1);
	//平移回到原点
	glTranslatef(L2 + L6, -L1, 0);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	//设置点的大小，以及启动alpha blend以及多采样，这样可以画圆形的点,点大小应该放在begin和end之外
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(8.0f);
	glBegin(GL_POINTS); //绘制顶点
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, L5 + L1);//1
	glVertex2f(L2 + L6, L1);//3
	glVertex2f(L7, -L8);//5
	glVertex2f(-L7, -L8);//7
	glVertex2f(-L2 - L6, L1);//9
	glVertex2f(0.0f, L5 + L1);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex2f(L2, L1);//2
	glVertex2f(L4, -L3);//4
	glVertex2f(0.0f, -R);//6
	glVertex2f(-L4, -L3);//8
	glVertex2f(-L2, L1);//10
	glEnd();

	//反走样
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(3.0f);//画线
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, L5 + L1);//1
	glVertex2f(L7, -L8);//5
	glVertex2f(-L2 - L6, L1);//9
	glVertex2f(L2 + L6, L1);//3
	glVertex2f(-L7, -L8);//7
	glVertex2f(0.0f, L5 + L1);//1
	glEnd();

	glFlush();

	// 弹出堆栈
	glPopMatrix();
	// 交换缓冲区
	glutSwapBuffers();
	
}

void reshape(int w, int h) {

	//防止除数即高度为0
	if (h == 0)
		h = 1;
	float ratio = 1.0* w / h;
	//单位化投影矩阵。
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//设置视口大小为增个窗口大小
	glViewport(0, 0, w, h);
	//设置正确的投影矩阵
	gluPerspective(45, ratio, 1, 1000);
	//设置模型视图矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);
}


void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case ' ':
		auto_rotate = !auto_rotate;
		break;
	default:
		break;
	}
}

//处理手动旋转,(UP)逆时针,(DOWN)顺时针
void specialkey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (auto_rotate) break;
		std::cout << "UP" << std::endl;
		angle = angle + 2;
		angle = angle > 360 ? angle - 360 : angle;
		glutPostRedisplay();//刷新显示
		break;
	case GLUT_KEY_DOWN:
		if (auto_rotate) break;
		std::cout << "DOWN" << std::endl;
		angle = angle - 2;
		angle = angle < 0 ? angle + 360 : angle;
		glutPostRedisplay();//刷新显示
		break;
	default:
		break;
	}
}

//设置定时器,每隔10ms检查是否需要自动旋转
void timer(int iTimerIndex) {
	if (auto_rotate) {
		Sleep(10);
		// 让angle自动增加。
		angle=angle+2;
		angle = angle > 360 ? angle - 360 : angle;
		glutPostRedisplay();//刷新显示
	}
	glutTimerFunc(10, timer, 0);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 500);
	glutCreateWindow("Star Rotate");
	glutDisplayFunc(display);
	//glutIdleFunc(display);
	glutReshapeFunc(reshape);

	//设置定时器,每隔10ms检查是否需要自动旋转
	glutTimerFunc(10, timer, 0);

	//处理(ESC)退出,和(Space)开关自动旋转
	glutKeyboardFunc(keyboard);
	
	//处理手动旋转,(UP)逆时针,(DOWN)顺时针
	glutSpecialFunc(specialkey);

	//开启深度测试
	//glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}