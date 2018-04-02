#include "gl\freeglut.h"
#include <stdio.h>

#define MAP_BLANK	0
#define MAP_OBS		1
#define MAP_FOOD	2

//�ӳ�ʽ����
void Init();//�����ʼ��
void Display();//��ʾ��Ϸ����
void Reshape(int w,int h);//���ڴ�С����
void Idle();//���д���
void Keyboard(unsigned char key,int x,int y);
bool CanMove(int x,int y);//�ж�ĳһλֵ�Ƿ������
bool CanEat(int x,int y);//�ж�ĳһλֵ����ʳ��
void SnakeMove(int nDirect);//���ƶ�0����1����2����3����
void AddTail();//�����β��
void RandFood();//�������һ���Ϸ���ʳ��
//ȫ�ֱ���
int g_Map[50][50];//��Ϸ��ͼ(0���հ�1���赲��2��ʳ��)
struct SnakeInfo
{
	int x[1024],y[1024];//�ߵ�����λֵ
	int nLength;//�ߵĳ���
	int nRunDirect;//�ߵ��ж�����
} g_Snake;

int main(int argc,char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("̰����");
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();

	return 0;
}

void Display()
{
	static int msLast=0;
	static int msLastMove=0;
	static bool bShowFood=true;
	bool bAddTail=false;
	int y,x;
	int i;

	glClear(GL_COLOR_BUFFER_BIT);

	if(GetTickCount()-msLast>500)
	{
		bShowFood=!bShowFood;
		msLast=GetTickCount();
	}

	if(GetTickCount()-msLastMove>300)
	{
		if(g_Snake.nRunDirect!=-1)
		{
			x=g_Snake.x[0];
			y=g_Snake.y[0];
			switch(g_Snake.nRunDirect)
			{
			case 0:
				y++;
				break;
			case 1:
				y--;
				break;
			case 2:
				x--;
				break;
			case 3:
				x++;
				break;
			}
			if(g_Map[x][y]==MAP_FOOD)
			{
				int nX,nY;
				g_Map[x][y]=MAP_BLANK;
				AddTail();
				bAddTail=true;
			}
			else if(!CanMove(x,y))
			{
				MessageBox(NULL,"ǽײ�����ˣ��Ǻǣ�~","С��",MB_OK);
			}
			SnakeMove(g_Snake.nRunDirect);
			if(bAddTail)
			{
				g_Snake.nLength++;
				RandFood();
			}
		}
		msLastMove=GetTickCount();
	}

	for(y=0;y<50;y++)//���Ƶ�ͼ
	{
		for(x=0;x<50;x++)
		{
			if(g_Map[x][y]==MAP_OBS)
			{
				glColor3f(0.0,0.0,1.0);
				glRectf((float)x*10,(float)y*10,(float)(x+1)*10,(float)(y+1)*10);
			}
			else if(g_Map[x][y]==MAP_FOOD && bShowFood)
			{
				glColor3f(0.0,1.0,0.0);
				glRectf((float)x*10,(float)y*10,(float)(x+1)*10,(float)(y+1)*10);
			}
		}
	}

	for(i=0;i<g_Snake.nLength;i++)//�����ߵ�����
	{
		glColor3f(1.0,1.0,0.0);
		glRectf((float)g_Snake.x[i]*10,(float)g_Snake.y[i]*10,(float)(g_Snake.x[i]+1)*10,(float)(g_Snake.y[i]+1)*10);
	}

	glutSwapBuffers();
}


void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'w'://��
		if(CanMove(g_Snake.x[0],g_Snake.y[0]+1))
		{
			SnakeMove(0);
		}
		else if(CanEat(g_Snake.x[0],g_Snake.y[0]+1))
		{
			g_Map[g_Snake.x[0]][g_Snake.y[0]+1]=MAP_BLANK;
			AddTail();
			SnakeMove(0);
			g_Snake.nLength++;
			RandFood();
		}
		else if(g_Map[g_Snake.x[0]][g_Snake.y[0]+1]==MAP_OBS)
		{
			MessageBox(NULL,"ǽײ�����ˣ��Ǻǣ�~","С��",MB_OK);
		}
		break;
	case 's'://��
		if(CanMove(g_Snake.x[0],g_Snake.y[0]-1))
		{
			SnakeMove(1);
		}
		else if(CanEat(g_Snake.x[0],g_Snake.y[0]-1))
		{
			g_Map[g_Snake.x[0]][g_Snake.y[0]-1]=MAP_BLANK;
			AddTail();
			SnakeMove(1);
			g_Snake.nLength++;
			RandFood();
		}
		else if(g_Map[g_Snake.x[0]][g_Snake.y[0]-1]==MAP_OBS)
		{
			MessageBox(NULL,"ǽײ�����ˣ��Ǻǣ�~","С��",MB_OK);
		}
		break;
	case 'a'://��
		if(CanMove(g_Snake.x[0]-1,g_Snake.y[0]))
		{
			SnakeMove(2);
		}
		else if(CanEat(g_Snake.x[0]-1,g_Snake.y[0]))
		{
			g_Map[g_Snake.x[0]-1][g_Snake.y[0]]=MAP_BLANK;
			AddTail();
			SnakeMove(2);
			g_Snake.nLength++;
			RandFood();
		}
		else if(g_Map[g_Snake.x[0]-1][g_Snake.y[0]]==MAP_OBS)
		{
			MessageBox(NULL,"ǽײ�����ˣ��Ǻǣ�~","С��",MB_OK);
		}
		break;
	case 'd'://��
		if(CanMove(g_Snake.x[0]+1,g_Snake.y[0]))
		{
			SnakeMove(3);
		}
		else if(CanEat(g_Snake.x[0]+1,g_Snake.y[0]))
		{
			g_Map[g_Snake.x[0]+1][g_Snake.y[0]]=MAP_BLANK;
			AddTail();
			SnakeMove(3);
			g_Snake.nLength++;
			RandFood();
		}
		else if(g_Map[g_Snake.x[0]+1][g_Snake.y[0]]==MAP_OBS)
		{
			MessageBox(NULL,"ǽײ�����ˣ��Ǻǣ�~","С��",MB_OK);
		}
		break;
	}
	glutPostRedisplay();
}

void SnakeMove(int nDirect)
{
	int i;
	int NowX,int NowY;
	int nLastX,nLastY;

	nLastX=g_Snake.x[0];
	nLastY=g_Snake.y[0];
	switch(nDirect)
	{
	case 0:
		g_Snake.y[0]++;
		break;
	case 1:
		g_Snake.y[0]--;
		break;
	case 2:
		g_Snake.x[0]--;
		break;
	case 3:
		g_Snake.x[0]++;
		break;
	}

	for(i=1;i<g_Snake.nLength;i++)
	{
		NowX=g_Snake.x[i];
		NowY=g_Snake.y[i];
		g_Snake.x[i]=nLastX;
		g_Snake.y[i]=nLastY;
		nLastX=NowX;
		nLastY=NowY;
	}

	g_Snake.nRunDirect=nDirect;
}

void RandFood()
{
	int x,y;
	int i;

	while(1)
	{
		x=rand()%50;
		y=rand()%50;

		if(g_Map[x][y]==MAP_BLANK)
		{
			for(i=0;i<g_Snake.nLength;i++)
			{
				if(g_Snake.x[i]==x && g_Snake.y[i]==y)
					continue;
			}
			break;
		}
	}

	g_Map[x][y]=MAP_FOOD;
}

bool CanMove(int x,int y)
{
	int i;
	if(g_Map[x][y]==MAP_BLANK)
	{
		for(i=1;i<g_Snake.nLength;i++)
			if(g_Snake.x[i]==x && g_Snake.y[i]==y)
				return false;

		return true;
	}

	return false;
}

bool CanEat(int x,int y)
{
	if(g_Map[x][y]==MAP_FOOD)
		return true;
	return false;
}

void AddTail()
{
	int nLength;

	nLength=g_Snake.nLength;

	g_Snake.x[nLength]=g_Snake.x[nLength-1];
	g_Snake.y[nLength]=g_Snake.y[nLength-1];
}

void Idle()
{
	glutPostRedisplay();
}

void Reshape(int w,int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,500,0,500,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
}

void Init()
{
	int i;
	int x,y;
	glClearColor(0.0,0.0,0.0,0.0);

	//��ʼ����Ϸ��ͼ
	for(x=0;x<50;x++)
	{
		g_Map[x][0]=MAP_OBS;
		g_Map[x][49]=MAP_OBS;
	}


	for(y=1;y<49;y++)
	{
		g_Map[0][y]=MAP_OBS;
		g_Map[49][y]=MAP_OBS;
	}

	//��ʼ��ʳ��
	g_Map[20][20]=MAP_FOOD;

	//��ʼ����
	g_Snake.x[0]=25;
	g_Snake.y[0]=25;
	g_Snake.x[1]=25;
	g_Snake.y[1]=24;
	g_Snake.nLength=2;
	g_Snake.nRunDirect=-1;//��ʼ�߲��ж�

	for(i=2;i<1024;i++)
	{
		g_Snake.x[i]=-100;
		g_Snake.y[i]=-100;
	}
}
