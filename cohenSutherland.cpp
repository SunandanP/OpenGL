#include<iostream>
#include<GL/gl.h>
#include<GL/glut.h>
#include<vector>

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
		this-> y = y;
	}
};

struct Code{
	int l;
	int r;
	int b;
	int t;
	
	Code(){
		l = 0;
		r = 0;
		b = 0;
		t = 0;
	}
	
	Code(int l, int r, int b, int t){
		this->l = l;
		this->r = r;
		this->b = b;
		this->t = t;
	}
};

struct Color{
	int r;
	int g;
	int b;
	
	Color(){
		r = g = b = 0;
	}
	
	Color(int r, int g, int b){
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

Point Min(250,250), Max(750,750);
vector<Point> points;

void init(){
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(0,1000,0,1000);
	glFlush();
}

void plotPoint(Point a, Color color){
	glPointSize(3);
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
		glVertex2i(a.x, a.y);
	glEnd();
	glFlush();	
}

void gentleLine(Point a, Point b, Color color){
	int dx = abs(b.x - a.x), dy = abs(b.y - a.y);
	int Dx = (b.x > a.x)? 1 : -1;
	int Dy = (b.y > a.y)? 1 : -1;
	
	int pk = (2 * dy) - dx;
	plotPoint(a, color);
	for(int i = 0; i < dx; i++){
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
	int dx = abs(b.x - a.x), dy = abs(b.y - a.y);
	int Dx = (b.x > a.x)? 1 : -1;
	int Dy = (b.y > a.y)? 1 : -1;
	
	int pk = (2 * dx) - dy;
	
	plotPoint(a, color);
	for(int i = 0; i < dy; i++){
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

void bresenham(Point a, Point b, Color color){
	int dx = abs(b.x - a.x), dy = abs(b.y - a.y);
	
	(dx > dy)? gentleLine(a,b,color) : harshLine(a,b, color);
}

void draw(){
	bresenham(Point(Min.x, Min.y), Point(Max.x, Min.y), Color(1,1,1));
	bresenham(Point(Max.x, Min.y), Point(Max.x, Max.y), Color(1,1,1));
	bresenham(Point(Max.x, Max.y), Point(Min.x, Max.y), Color(1,1,1));
	bresenham(Point(Min.x, Max.y), Point(Min.x, Min.y), Color(1,1,1));
}

Code getCode(Point a){
	Code code;
	
	if(a.x < Min.x){
		code.l = 1;
	}
	
	if(a.x > Max.x){
		code.r = 1;
	}
	
	if(a.y < Min.y){
		code.b = 1;
	}
	
	if(a.y > Max.y){
		code.t = 1;
	}
	
	return code;
}

bool checkCode(Code code){
	return (code.l == 0 && code.r == 0 && code.b == 0 && code.t == 0);
}

int andOperation(int a, int b){
	return (a == 1 && b == 1)? 1 : 0; 
}

Code andCodes(Code c1, Code c2){
	Code code;
	
	code.l = andOperation(c1.l, c2.l);
	code.r = andOperation(c1.r, c2.r);
	code.b = andOperation(c1.b, c2.b);
	code.t = andOperation(c1.t, c2.t);
	
	return code;
}

Point calculateIntersection(Point a, float slope){
	Point p;
	
	Code code = getCode(a);
	
	
	if(checkCode(code)){
		p.x = a.x;
		p.y = a.y;
	}
	
	
	
	if(code.l == 1){
		p.x = Min.x;
		p.y = a.y + ((slope) * (Min.x - a.x));
	}
	
	if(code.r == 1){
		p.x = Max.x;
		p.y = a.y + ((slope) * (Max.x - a.x));
	}
	
	if(code.b == 1){
		p.y = Min.y;
		p.x = a.x + ((1/slope) * (Min.y - a.y));
	}
	
	if(code.t == 1){
		p.y = Max.y;
		p.x = a.x + ((1/slope) * (Max.y - a.y));
	}
	
	
	

	return p;
}

void clip(Point a, Point b){
	Code c1 = getCode(a), c2 = getCode(b);
	
	bresenham(a, b, Color(0,0,0));
	
	//completely accepted
	if(checkCode(c1) && checkCode(c2)){
		bresenham(a,b,Color(0,1,0));
	}
	else{
		
		Code code = andCodes(c1, c2);
		
		//partial acceptance	
		if(checkCode(code)){
			float slope = (b.y - a.y) / float((b.x - a.x));
			bresenham(calculateIntersection(a, slope), calculateIntersection(b, slope), Color(0,1,0));
			
		}
		
		//otherwise complete rejection.
		
	}
	
	
}


void mouse(int button, int state, int x, int y){
	y = 1000 - y;
	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		points.push_back(Point(x,y));
	}
	
	
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		bresenham(points[0],points[1],Color(1,0,1));
	}
	
	if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
		clip(points[0], points[1]);
		while(points.size() != 0)
			points.pop_back();

	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Clipping");
	glutMouseFunc(mouse);
	init();
	draw();
	glutMainLoop();
}

