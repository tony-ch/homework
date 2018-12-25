#include <GL/glut.h>
#include <cmath>
void display(void)
{
	//表示出各个点的坐标，设半径 R=0.2
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
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//绘制边
	//glBegin(GL_POLYGON); 
	//glColor3f(0.0f, 0.0f, 0.0f);
	//glVertex2f(0.0f, 0.0f);
	//glVertex2f(0.0f, L5 + L1);//1
	//glVertex2f(L2, L1);//2
	//glVertex2f(L2 + L6, L1);//3
	//glVertex2f(L4, -L3);//4
	//glVertex2f(L7, -L8);//5
	//glVertex2f(0.0f, -Rf);//6
	//glVertex2f(-L7, -L8);//7
	//glVertex2f(-L4, -L3);//8
	//glVertex2f(-L2 - L6, L1);//9
	//glVertex2f(-L2, L1);//10
	//glVertex2f(0.0f, L5 + L1);
	//glEnd();

	glEnable(GL_POINT_SMOOTH);//设置点的大小并设点为圆点
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

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 500);
	glutCreateWindow("Star");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
