#include<GL/gl.h>
#include<GL/glut.h>
#include<iostream>
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

vector<Point> points;
int type = 2;

void plotPoint(Point a, int size){
	glPointSize(size);
	glBegin(GL_POINTS);
		glVertex2i(a.x, a.y);
	glEnd();	
}

void DDA(Point a, Point b, int type){
	int count = 0;
	bool draw = true;
	int dx = b.x - a.x;
	int dy = b.y - a.y;
	
	int step = (abs(dx) > abs(dy))? abs(dx) : abs(dy);
	
	float Dx = dx/(float)step , Dy = dy/(float)step ;
	
	float x = a.x, y = a.y;
	
	plotPoint(a,1);
	
	for(int i = 0; i <= step; i++, count++){
		x += Dx;
		y += Dy;
		
		a.x = x;
		a.y = y;
		cout<<count<<endl;	
		//dotted line
		if(type == 0){
			if(count % 7 == 0){
				plotPoint(a,1);
			}
		}
		//dashed
		else if(type == 1){
			if(count % 20 == 0){
				if(draw == true){
					draw = false;
				}
				else{
					draw = true;
				}
			}
			
			if(draw){
				plotPoint(a,1);
				
			}	
		}
		//simple
		else if (type == 2){
			plotPoint(a,1);
			
		}
		//solid
		else if(type == 3){
			plotPoint(a,3);
			
		}
	}
	
	glFlush();
}


void axes(){
	DDA(Point(-500,0), Point(500,0), 2);
	DDA(Point(0,-500), Point(0,500), 2);
}

void mouse(int button, int state, int x, int y){
	y = 500 - y;
	x = x - 500;
		
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		points.push_back(Point(x,y));
	}
	
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		DDA(points[0], points[1], type);
		while(points.size() != 0){
			points.pop_back();
		}
	}
}

void keyboard(unsigned char key, int x, int y){
	if(key == '2'){
		type = 2;
	}
	
	if(key == '1'){
		type = 1;
	}
	
	if(key == '0'){
		type = 0;
	}
	
	if(key == '3'){
		type = 3;
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
	glutCreateWindow("DDA");
	init();
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}
