#include<GL/gl.h>
#include<GL/glut.h>
#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

struct Point {
	int x;
	int y;
	
	Point(){
		x = 0;
		y = 0;
	}
	
	Point(int x, int y){
		this->x = x;
		this->y = y;
	}
};

vector<Point> points;
int type = 2;

void plotPoint(Point a){
	glPointSize(2);
	glBegin(GL_POINTS);
		glVertex2i(a.x, a.y);
	glEnd();	
}

void plotAllPoints(Point c, Point a){
	plotPoint(Point(a.x + c.x, a.y + c.y));
	plotPoint(Point(a.y + c.x, a.x + c.y));
	plotPoint(Point(-a.x + c.x, a.y + c.y));
	plotPoint(Point(-a.y + c.x, a.x + c.y));
	plotPoint(Point(a.x + c.x, -a.y + c.y));
	plotPoint(Point(a.y + c.x, -a.x + c.y));
	plotPoint(Point(-a.x + c.x, -a.y + c.y));
	plotPoint(Point(-a.y + c.x, -a.x + c.y));
}

void circle(Point center, double radius){
	
	int pk = 3 - (2 * radius);
	double x = 0, y = radius;
	
	while(x <= y){
		if(pk < 0){
			x++;
			pk += (4 * x) + 6;
		}
		else{
			x++;
			y--;
			pk += (4 * (x - y)) + 10;
		}
		
		plotAllPoints(center, Point(x,y));
	}
	
	glFlush();
}

void DDA(Point a, Point b){
	int count = 0;
	bool draw = true;
	int dx = b.x - a.x;
	int dy = b.y - a.y;
	
	int step = (abs(dx) > abs(dy))? abs(dx) : abs(dy);
	
	float Dx = dx/(float)step , Dy = dy/(float)step ;
	
	float x = a.x, y = a.y;
	
	plotPoint(a);
	
	for(int i = 0; i <= step; i++, count++){
		x += Dx;
		y += Dy;
		
		a.x = x;
		a.y = y;
		
		plotPoint(a);
		
	}
	
	glFlush();
}


void axes(){
	DDA(Point(-500,0), Point(500,0));
	DDA(Point(0,-500), Point(0,500));
}

void mouse(int button, int state, int x, int y){
	y = 500 - y;
	x = x - 500;
		
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		points.push_back(Point(x,y));
	}
	
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		Point a, b;
		a = points[0];
		b = points[1];
		
		double dist = sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
		circle(a, dist);
		while(points.size() != 0){
			points.pop_back();
		}
	}
}



void init(){
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(-500,500,-500,500);
	axes();
	glFlush();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Circle");
	init();
	glutMouseFunc(mouse);
	glutMainLoop();
}
