#include <stdio.h>
#include <GL/freeglut.h>
#include <unistd.h>
#include <string.h>

GLint width, height;
GLint lWidth, lHeight;
GLint generation[40][40];
GLint mode;

void drawGrid()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		int i;
		for(i=0;i<40;i++)	//vertical
		{
			glVertex2i(20*i, 0);
			glVertex2i(20*i, 800);
		}
		for(i=0;i<40;i++)	//horizontal
		{
			glVertex2i(0, 20*i);
			glVertex2i(800, 20*i);
		}
	glEnd();
}

void drawSquare(int centerx, int centery, int size)
{
	int r = size/2;
	
	glBegin(GL_QUADS);
		glVertex2i(centerx+r, centery+r);
		glVertex2i(centerx+r, centery-r);
		glVertex2i(centerx-r, centery-r);
		glVertex2i(centerx-r, centery+r);
	glEnd();

}

void drawGeneration(int g[][40], int size)
{
	int i, j; //i -> row j -> column
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(g[i][j]==1)
				drawSquare(10+((i)*20), (10+(j)*20), 20);
		}
	}
}

int getNeighbours(int g[][40], int x, int y)
{
	int n=0, i, j;
	for(i=x-1;i<=x+1;i++)
	{
		for(j=y+1;j>=y-1;j--)
		{
			if(i == 40 || j == 40 || i == -1 || j == -1 || (i == x
			&& j == y))
				;
			else
				n+=g[i][j];
		}
	}
	return n;
}

void cpyMatrix(int g1[][40], int g2[][40])
{
	int i, j;
	for(i=0;i<40;i++)
		for(j=0;j<40;j++)
			g2[i][j] = g1[i][j];
}

void initMatrix(int g1[][40])
{
	int i, j;
	for(i=0;i<40;i++)
		for(j=0;j<40;j++)
			g1[i][j] = 0;
}

void nextGeneration(int g[][40])
{
	int g2[40][40];
	int i, j, n;
	for(i=0;i<40;i++)
	{
		for(j=0;j<40;j++)
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

	glBegin(GL_QUADS);
	glColor3f(r-0.2, g-0.2, b-0.2);
	glVertex2i(x+w+diff, y+h); //(UP)Upper right
	glVertex2i(x+w, y+h-diff); //Down right
	glVertex2i(x-w, y+h-diff);
	glVertex2i(x-w-diff, y+h);

	glColor3f(r-.5, g-.5, b-.5);
	glVertex2i(x+w+diff, y-h); //(DOWN)Upper right
	glVertex2i(x+w, y-h+diff); //Down right
	glVertex2i(x-w, y-h+diff);
	glVertex2i(x-w-diff, y-h);

	glColor3f(r-0.7, g-0.7, b-0.7);
	glVertex2i(x-w, y+h-diff);  //(LEFT)Upper right
	glVertex2i(x-w-diff, y+h);
	glVertex2i(x-w-diff, y-h);
	glVertex2i(x-w, y-h+diff);

	glColor3f(r-0.4, g-0.4, b-0.4);
	glVertex2i(x+w, y+h-diff);  //(RIGHT)Upper right
	glVertex2i(x+w+diff, y+h);
	glVertex2i(x+w+diff, y-h);
	glVertex2i(x+w, y-h+diff);
	
	glColor3f(r, g, b);
	glVertex2i(x+w, y+h-diff);  //FILLER
	glVertex2i(x+w, y-h+diff);
	glVertex2i(x-w, y-h+diff);
	glVertex2i(x-w, y+h-diff);
	glEnd();

	drawText(s, x-(strlen(s)*5), y-5, 1, 1, 1);
}


void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(mode)
	{
		glClearColor(0, (127/255), 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		drawGrid();
		drawGeneration(generation, 40);
		glFlush();
	}
	else
	{
		glClearColor(0, 127/255, 1, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		drawText("CONWAY'S GAME OF LIFE", (width/2)-150, height-100, 1, 1, 1);
		drawButton(width/2, (height/2)+40, 80, 25, 0, 0, 1, "Start Game");
		drawButton(width/2, (height/2)-40, 80, 25, 0, 0, 1, "Exit");
		glFlush();
	}
	
}

void passiveMotion(int x, int y)
{
	if(mode)
		return;
	//printf("X = %d ||| y = %d\n", x, y);
	if(x>=320 && x<=480)
		if(y>=310 && y<=350)
		{
			drawButton(width/2, (height/2)+40, 80, 25, 0, 0, .5, "Start Game");
			glFlush();
		}
		else if(y>=380 && y<=420)
		{
			drawButton(width/2, (height/2)-40, 80, 25, 0, 0, .5, "Exit");
			glFlush();
		}
		else
			glutPostRedisplay();
}

void mouse(int button, int clicked, int x, int y)
{
	//printf("button = %d, clicked = %d, x = %d, y = %d\n", 
	//button, clicked, x, y);
	if(clicked && mode==1)
	{
		printf("x = %d, y = %d", x, y);
		printf("Square: %d %d\n", x/20, (718-y)/20);
		generation[x/20][39-(y/20)] = 1;
		drawGeneration(generation, 40);
		glutPostRedisplay();
	}
	if(clicked && mode==0)
	{
		if(x>=320 && x<=480)
			if(y>=310 && y<=350)
			{
				mode=1;
				glutPostRedisplay();
			}
			else if(y>=380 && y<=420)
			{
				printf("EXIT!\n");
				glutDestroyWindow(glutGetWindow());
			}
	}
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

	width = 800; height = 800;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Life!");

	init();
	glutDisplayFunc(render);
	//glutIdleFunc(idle);
	glutPassiveMotionFunc(passiveMotion);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);

	generation[5][5] = 1;
	generation[4][5] = 1;
	generation[3][5] = 1;
	generation[38][11] = 1;
	generation[38][12] = 1;
	generation[38][13] = 1;
	generation[38][14] = 1;
	generation[38][15] = 1;

	generation[5][35] = 1;
	generation[5][34] = 1;
	generation[5][33] = 1;
	generation[6][33] = 1;
	generation[7][33] = 1;
	generation[7][34] = 1;
	generation[7][35] = 1;
	generation[6][35] = 1;
	
	generation[39][39] = 1;
	glutMainLoop();

	return 0;
}