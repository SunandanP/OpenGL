#include<iostream>
#include<GL/gl.h>
#include<GL/glut.h>
#include<math.h>
#include<vector>

#define radian (3.14/180)
using namespace std;

struct Point{
	float x, y;
	
	Point(){
		x = 0;
		y = 0;
	}
	
	Point(float x, float y){
		this->x = x;
		this->y = y;
	}
};

struct Color{
	float r, g, b;
	
	Color(){
		r = 0;
		b = 0;
		g = 0;
	}
	
	Color(float r, float g, float b){
		this->r = r;
		this->g = g;
		this->b = b;
	}
};
vector<Point> points;
int n = 3;

void bresenham(Point, Point, Color);

void kochCurve(Point a, Point b, Color col, int n){
	Point c, d, mid;
	
	// 1/3rd part's point
	c.x = ((2 * a.x) + b.x)/3;
	c.y = ((2 * a.y) + b.y)/3;
	
	// 2/3rd part's point
	d.x = ((2 * b.x) + a.x)/3;
	d.y = ((2 * b.y) + a.y)/3;
	
	//temp
	float x = c.x - d.x, y = c.y - d.y;
	
	//equilatral triangle's angle
	float R = 60 * radian;
	
	mid.x = d.x + ((x * cos(R)) + (y * sin(R)));
	mid.y = d.y - ((x * sin(R)) + (y * -cos(R)));
	
	if( n > 0){
	
		kochCurve(a, c, col,  n-1);
		kochCurve(c, mid, col, n-1);
		kochCurve(mid, d, col, n-1);
		kochCurve(d, b, col, n-1);
		
	}
	else{
		bresenham(a,c,  col);
		bresenham(c,mid, col);
		bresenham(mid,d, col);
		bresenham(d,b, col);
		return;
		
	}
	
	
}

void plotPoint(Point a, Color c){
	glPointSize(3);
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_POINTS);
		glVertex2i(a.x, a.y);
	glEnd();	
}


void gentleLine(Point a, Point b, Color color){
	
	
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	
	int Dx = (b.x > a.x)? 1 : -1;
	int Dy = (b.y > a.y)? 1 : -1;
	
	int pk = (2 * dy) - dx;
	
	plotPoint(a, color);
	for(int i = 0; i < dx; i++, count++){
		if(pk < 0){
			a.x += Dx;
			pk += 2 * dy;
		}
		else{
			a.x += Dx;
			a.y += Dy;
			pk += 2 * (dy - dx);
		}
		
		plotPoint(a, color);
		
	}
}

void harshLine(Point a, Point b, Color color){
	
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	
	int Dx = (b.x > a.x)? 1 : -1;
	int Dy = (b.y > a.y)? 1 : -1;
	
	int pk = (2 * dx) - dy;
	
	plotPoint(a, color);
	for(int i = 0; i < dy; i++, count++){
		if(pk < 0){
			a.y += Dy;
			pk += 2 * dx;
		}
		else{
			a.x += Dx;
			a.y += Dy;
			pk += 2 * (dx - dy);
		}
		
		plotPoint(a, color);
		
	}
}

void bresenham(Point a, Point b,Color color){
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	
	(dx > dy)? gentleLine(a,b, color) : harshLine(a,b, color);
	
	glFlush();
}


void mouse(int button, int state, int x, int y){
	y = 1000 - y;
	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
	
		points.push_back(Point(x, y));
	}
	
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		kochCurve(points[0], points[1],Color(1,1,1),n);
		while(points.size() != 0){
			points.pop_back();
		}
	}
}

void snowFlake(int i, Color color){
	kochCurve(Point(600,100), Point(400,400), color, i);
	kochCurve(Point(400,400), Point(800,400), color, i);
	kochCurve(Point(800,400), Point(600,100), color, i);
}

void keyboard(unsigned char key, int x, int y){
	if(isdigit(key)){
		snowFlake(n, Color(0,0,0));
		n = key - '0';
		snowFlake(n, Color(1,1,1));
	}
}

void init(){
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(0,1000,0,1000);	
	glFlush();
}



int main(int c, char** v){
	glutInit(&c, v);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Koch");
	init();
	snowFlake(n, Color(1,1,1));
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}
