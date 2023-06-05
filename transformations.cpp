#include<GL/gl.h>
#include<GL/glut.h>
#include<vector>
#include<cmath>

#define radian M_PI/180

using namespace std;

struct Point{
	int x;
	int y;

	public:
		
	Point(){
		x = 0;
		y = 0;
	}
	
	Point(int x, int y){
		this->x = x;
		this->y = y;
	}
};


void init(){
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(-500,500,-500,500);
	glFlush();
}


vector<vector<float>> points;

void plotPoint(Point a){
	glBegin(GL_POINTS);
		glVertex2i(a.x, a.y);
	glEnd();
	glFlush();	
}


void gentleLine(Point a, Point b){
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	
	int pk = (2 * dy) - dx;
	
	int Dx = (b.x > a.x)? 1 : -1;
	int Dy = (b.y > a.y)? 1 : -1;
	
	plotPoint(a);
	
	for(int i = 0; i < dx; i++){
		if(pk < 0){
			pk += 2 * dy;
			a.x += Dx;
		}
		else{
			pk += (2 * (dy - dx));
			a.x += Dx;
			a.y += Dy;
		}
		
		plotPoint(a);
	}
	
}

void harshLine(Point a, Point b){
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	
	int pk = (2 * dx) - dy;
	
	int Dx = (b.x > a.x)? 1 : -1;
	int Dy = (b.y > a.y)? 1 : -1;
	
	plotPoint(a);
	
	for(int i = 0; i < dy; i++){
		if(pk < 0){
			pk += 2 * dx;
			a.y += Dy;
		}
		else{
			pk += (2 * (dx - dy));
			a.x += Dx;
			a.y += Dy;
		}
		
		plotPoint(a);
	}
	
}

void bresenham(Point a, Point b){
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	
	(dx > dy)? gentleLine(a,b) : harshLine(a,b);
}


void drawPolygon(){
	for(int i = 0; i < points.size(); i++){
			if(i+1 < points.size()){
				bresenham(Point(points[i][0], points[i][1]), Point(points[i+1][0], points[i+1][1]));
			}
			else{
				bresenham(Point(points[i][0], points[i][1]), Point(points[0][0], points[0][1]));
			}
		}
}

void mouse(int button, int state, int x, int y){
	y = 500 - y;
	x = x - 500;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		points.push_back({x , y , 1});
	}
	
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		drawPolygon();
	}
	
}

void matrixMul(vector<vector<float>>& a, vector<vector<float>>& b) {
	
	int numRowsA = a.size();
	vector<vector<float>> result(20, vector<float>(3));
	
	for (int i = 0; i < numRowsA; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j];
		}
	}
	
	for (int i = 0; i < numRowsA; i++) {
		for (int j = 0; j < 3; j++) {
			a[i][j] = result[i][j];
		}
	}
	
}



void axes(){
	bresenham(Point(-500,0), Point(500,0));
	bresenham(Point(0,-500), Point(0,500));
}

void translate(float x, float y){
	vector<vector<float>> translationMatrix = {
		{1, 0, 0}, 
		{0, 1, 0},
		{x, y, 1}};
		
	
	
	matrixMul(points, translationMatrix);
	
	drawPolygon();
	while(points.size() != 0){
			points.pop_back();
		}
}

void scale(float sx, float sy){
	vector<vector<float>> scaleMatrix = {
		{sx, 0, 0}, 
		{0, sy, 0},
		{0, 0, 1}};
	
	matrixMul(points, scaleMatrix);
	
	drawPolygon();
	while(points.size() != 0){
			points.pop_back();
		}
}

void rotation(float rotationAngle) {
	vector<vector<float>> rotationMatrix = {
	{cos(rotationAngle), sin(rotationAngle), 0}, 
	{-sin(rotationAngle), cos(rotationAngle), 0},
	{0, 0, 1}};
															
	matrixMul(points, rotationMatrix);
	drawPolygon();
	while(points.size() != 0){
			points.pop_back();
		}
}

void reflectionX() {
	vector<vector<float>> reflectionMatrix = {
		{1, 0, 0}, 
		{0, -1, 0},
		{0, 0, 1}};
															
	matrixMul(points, reflectionMatrix);
	drawPolygon();
	while(points.size() != 0){
			points.pop_back();
		}
}

void reflectionY() {
	vector<vector<float>> reflectionMatrix = {
		{-1, 0, 0}, 
		{0, 1, 0},
		{0, 0, 1}};
															
	matrixMul(points, reflectionMatrix);
	drawPolygon();
	while(points.size() != 0){
			points.pop_back();
		}
}

void shear(int x) {
	vector<vector<float>>  shearMat = {
		{1, x, 0}, 
		{0, 1, 0},
		{0, 0, 1}};
															
	matrixMul(points, shearMat);
	drawPolygon();
	while(points.size() != 0){
			points.pop_back();
		}
}


void keyboard(unsigned char key, int x, int y){
	if(key == 't'){
		translate(300,300);	
	}
	
	if(key == 's'){
		scale(2,2);
	}
	
	if(key == 'r'){
		rotation(180 * radian);
	}
	
	if(key == 'x'){
		reflectionX();
	}
	
	if(key == 'y'){
		reflectionY();
	}
	
	if(key == 'm'){
		shear(1.5);
	}
}


int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Transformation vector!");
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	init();
	axes();
	glutMainLoop();
}



