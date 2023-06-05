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
		this->y = y;
	}
};

struct Color{
	float r, g, b;
	
	Color(){
		r = 1;
		g = 1;
		b = 1;
	}
	
	Color(float r, float g, float b){
		this->r = r;
		this->g = g;
		this->b = b;
	}
	
};

vector<Point> points;
int type = 2;

void plotPoint(Point a){
	glColor3f(0,0,1);
	glBegin(GL_POINTS);
		glVertex2i(a.x, a.y);
	glEnd();	
}

void gentleLine(Point a, Point b){
	bool draw = true;
	int count = 0;
	
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	
	int Dx = (b.x > a.x)? 1 : -1;
	int Dy = (b.y > a.y)? 1 : -1;
	
	int pk = (2 * dy) - dx;
	
	plotPoint(a);
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
		
		plotPoint(a);
		
	}
}

void harshLine(Point a, Point b){
	bool draw = true;
	int count = 0;
	
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	
	int Dx = (b.x > a.x)? 1 : -1;
	int Dy = (b.y > a.y)? 1 : -1;
	
	int pk = (2 * dx) - dy;
	
	plotPoint(a);
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
		
		plotPoint(a);
		
	}
}

void bresenham(Point a, Point b){
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	
	(dx > dy)? gentleLine(a,b) : harshLine(a,b);
	
	glFlush();
}

Color getColor(Point a){
	Color color;
	glReadPixels(a.x, a.y, 1.0, 1.0, GL_RGB, GL_FLOAT, &color);
	return color;
}

bool isSame(Color a, Color b){
	return (a.r == b.r && a.g == b.g && a.b == b.b);
}

void colorPixel(Point a, Color color){
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
		glVertex2i(a.x, a.y);
	glEnd();	
	glFlush();
}

void floodFill(Point seed, Color fillColor){
	int x = seed.x, y = seed.y;
	
	Color pixelColor = getColor(seed);
	
	
	
	//pixel color and bg color should be same
	if(isSame(pixelColor, Color(0,0,0))) {
		colorPixel(Point(x,y), fillColor);
		
		floodFill(Point(x+1,y), fillColor);
		floodFill(Point(x-1,y), fillColor);
		floodFill(Point(x,y+1), fillColor);
		floodFill(Point(x,y-1), fillColor);
	}
	
}

void boundryFill(Point seed, Color fillColor, Color boundryColor = Color(0,0,1)){
	int x = seed.x, y = seed.y;
	
	Color pixelColor = getColor(seed);
	cout<<pixelColor.r<<" "<<pixelColor.g<<" "<<pixelColor.b<<endl;
	
	if(!isSame(pixelColor,boundryColor) && !isSame(pixelColor, fillColor) ){
		colorPixel(Point(x,y), fillColor);
		
		boundryFill(Point(x+1,y), boundryColor, fillColor);
		boundryFill(Point(x-1,y), boundryColor, fillColor);
		boundryFill(Point(x,y+1), boundryColor, fillColor);
		boundryFill(Point(x,y-1), boundryColor, fillColor);
	}
	
}


void mouse(int button, int state, int x, int y){
	y = 1000 - y;
		
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		points.push_back(Point(x,y));
		cout<<x<<" "<<y<<endl;
	}
	
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		for(int i = 0; i < points.size(); i++){
			if(i+1 < points.size()){
				bresenham(points[i], points[i+1]);	
			}
			else{
				bresenham(points[i], points[0]);
			}
		}
		while(points.size() != 0){
			points.pop_back();
		}
	}
	
}

void keyboard(unsigned char key, int x, int y){
	if(key == 'f'){
		floodFill(points[0], Color(0,0,1));
	}
	
	if(key == 'b'){
		boundryFill(points[0], Color(0,0,1));
	}
	
}


void init(){
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(0,1000,0,1000);
	glFlush();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Bresenham");
	init();
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}
