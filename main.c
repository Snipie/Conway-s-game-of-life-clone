#include <stdio.h>
#include <GL/freeglut.h>
#include <unistd.h>
#include <string.h>

GLint width, height;
GLint lWidth, lHeight;
GLint generation[30][30];
GLint mode;

GLint btn1x1, btn1x2, btn1y1, btn1y2;
GLint btn2x1, btn2x2, btn2y1, btn2y2;

void drawGrid()
{
	GLint w=glutGet(GLUT_WINDOW_WIDTH), h=glutGet(GLUT_WINDOW_HEIGHT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		int i;
		for(i=0;i<40;i++)	//vertical
		{
			glVertex2i(20*i, 0);
			glVertex2i(20*i, h);
		}
		for(i=30;i>-10;i--)	//horizontal
		{
			glVertex2i(0, 20*i);
			glVertex2i(w, 20*i);
		}
	glEnd();
}

void drawSquare(int centerx, int centery, int size)
{
	int r = size/2;
	
	glBegin(GL_QUADS);
		glVertex2i((centerx+r), height-(centery+r));
		glVertex2i((centerx+r), height-(centery-r));
		glVertex2i((centerx-r), height-(centery-r));
		glVertex2i((centerx-r), height-(centery+r));
	glEnd();

}

void drawGeneration(int g[][30], int size)
{
	int i, j; //i -> row j -> column
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(g[i][j]==1)
				drawSquare(10+(i*20), 10+(j*20), 20);
		}
	}
}

int getNeighbours(int g[][30], int x, int y)
{
	int n=0, i, j;
	for(i=x-1;i<=x+1;i++)
	{
		for(j=y+1;j>=y-1;j--)
		{
			if(i == 30 || j == 30 || i == -1 || j == -1 || (i == x
			&& j == y))
				;
			else
				n+=g[i][j];
		}
	}
	return n;
}

void cpyMatrix(int g1[][30], int g2[][30])
{
	int i, j;
	for(i=0;i<30;i++)
		for(j=0;j<30;j++)
			g2[i][j] = g1[i][j];
}

void initMatrix(int g1[][30])
{
	int i, j;
	for(i=0;i<30;i++)
		for(j=0;j<30;j++)
			g1[i][j] = 0;
}

void nextGeneration(int g[][30])
{
	int g2[30][30];
	int i, j, n;
	for(i=0;i<30;i++)
	{
		for(j=0;j<30;j++)
		{
			n = getNeighbours(g, i, j);
			if(g[i][j]) //cell is alive
			{
				if(n<2 || n>3) //rule number 1 & number 3
					g2[i][j]=0;
				else if(n==2 || n==3) //rule number 2
					g2[i][j]=1;
				else
					g2[i][j]=g[i][j];
			}
			else
			{
				if(n==3) //rule number 4
					g2[i][j]=1;
				else
					g2[i][j]=g[i][j];
			}
		}
	}
	cpyMatrix(g2, g);
}

void drawText(char s[], int x, int y, int r, int g, int b)
{
	int i;
	glColor3f(r, g, b);
	glRasterPos2i(x, y);
	//GLUT_BITMAP_HELVETICA_12
	for(i=0;i<strlen(s);i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)(s[i]));
}

void drawButton(int x, int y, int w, int h,float r, float g, float b, char* s)
{
	int diff = w/16;
	if(!strcmp(s, "Start Game"))
	{
		btn1x1 = x+w+diff; btn1y1 = y+h;
		btn1x2 = x-w-diff; btn1y2 = y-h;
	}
	else if(!strcmp(s, "Exit"))
	{
		btn2x1 = x+w+diff; btn2y1 = y+h;
		btn2x2 = x-w-diff; btn2y2 = y-h;
	}

	glBegin(GL_QUADS);
	glColor3f(r-0.2, g-0.2, b-0.2);
	glVertex2i(x+w+diff, y+h); //Top
	glVertex2i(x+w, y+h-diff);
	glVertex2i(x-w, y+h-diff);
	glVertex2i(x-w-diff, y+h);

	glColor3f(r-.5, g-.5, b-.5);
	glVertex2i(x+w+diff, y-h); //Buttom
	glVertex2i(x+w, y-h+diff);
	glVertex2i(x-w, y-h+diff);
	glVertex2i(x-w-diff, y-h);

	glColor3f(r-0.7, g-0.7, b-0.7);
	glVertex2i(x-w, y+h-diff); //Left
	glVertex2i(x-w-diff, y+h);
	glVertex2i(x-w-diff, y-h);
	glVertex2i(x-w, y-h+diff);

	glColor3f(r-0.4, g-0.4, b-0.4);
	glVertex2i(x+w, y+h-diff); //Right
	glVertex2i(x+w+diff, y+h);
	glVertex2i(x+w+diff, y-h);
	glVertex2i(x+w, y-h+diff);
	
	glColor3f(r, g, b);
	glVertex2i(x+w, y+h-diff); //Filler
	glVertex2i(x+w, y-h+diff);
	glVertex2i(x-w, y-h+diff);
	glVertex2i(x-w, y+h-diff);
	glEnd();

	drawText(s, x-(strlen(s)*5), y-5, 1, 1, 1);
}


void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(mode == 1)
	{
		glClearColor(0, 127/255, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		drawGrid();
		drawGeneration(generation, 30);
		glFlush();
	}
	else if(mode == 0)
	{
		glClearColor(0, 127/255, 1, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		drawText("CONWAY'S GAME OF LIFE", (width/2)-150, height-100, 1, 1, 1);
		drawButton(width/2, (height/2)+40, 80, 25, 0, 0, 1, "Start Game");
		drawButton(width/2, (height/2)-40, 80, 25, 0, 0, 1, "Exit");
		glFlush();
	}
	else if(mode == -1)
	{
		drawText("CONWAY'S GAME OF LIFE", (width/2)-150, height-100, 1, 1, 1);
		drawButton(width/2, (height/2)-40, 80, 25, 0, 0, 1, "Exit");
		drawButton(width/2, (height/2)+40, 80, 25, 0, 0, .5, "Start Game");
		glFlush();
	}
	else if(mode == -2)
	{
		drawText("CONWAY'S GAME OF LIFE", (width/2)-150, height-100, 1, 1, 1);
		drawButton(width/2, (height/2)+40, 80, 25, 0, 0, 1, "Start Game");
		drawButton(width/2, (height/2)-40, 80, 25, 0, 0, .5, "Exit");
		glFlush();
	}
}

void passiveMotion(int x, int y)
{
	y=height-y;

	if(mode==1)
		return;
	if(x>=btn1x2 && x<=btn1x1)
	{
		if(y>=btn1y2 && y<=btn1y1)
		{
			mode=-1;
			glutPostRedisplay();
		}
		else if(y>=btn2y2 && y<=btn2y1)
		{
			mode=-2;
			glutPostRedisplay();
		}
		else
		{
			mode=0;
			glutPostRedisplay();
		}
	}
	else
	{
		mode=0;
		glutPostRedisplay();
	}

}

void mouse(int button, int clicked, int x, int y)
{
	printf("Mouse: %d %d gen[%d][%d]\n", x, y, x/15, y/15);
	if(clicked && mode==1)
	{
		generation[x/20][y/20] = 1;
		drawGeneration(generation, 30);
		glutPostRedisplay();
	}

	y=height-y;

	if(clicked && mode==-1)
	{	
		if(x>=btn1x2 && x<=btn1x1)
			if(y>=btn1y2 && y<=btn1y1)
			{
				mode=1;
				glutPostRedisplay();
			}
	}
	else if(clicked && mode==-2)
		glutDestroyWindow(glutGetWindow());
}

void reshape(int w, int h)
{
	width = w;
	height = h;
}

void keyboard(unsigned char key, int b, int c)
{
	if(key == 'n')
	{
		nextGeneration(generation);
		glutPostRedisplay();
	}
	else if(key == 'e')
	{
		mode = 0;
		glutPostRedisplay();
	}
	else if(key == 'c')
	{
		initMatrix(generation);
		glutPostRedisplay();
	}
}

void init(void)
{
	glClearColor(.5, .5, .5, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, width, 0.0, height);
}

int main(int argc, char* argv[])
{
	//btns[0] = {width/2, height/2 , 20, 20, 1, 0, 0};

	width = 600; height = 600;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Life!");

	init();
	glutDisplayFunc(render);
	glutPassiveMotionFunc(passiveMotion);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);

	generation[0][0] = 1;

	glutMainLoop();

	return 0;
}
