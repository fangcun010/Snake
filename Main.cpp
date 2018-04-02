#include "gl\freeglut.h"
#include <stdio.h>

#define MAP_BLANK	0
#define MAP_OBS		1
#define MAP_FOOD	2

//子程式声明
void Init();//程序初始化
void Display();//显示游戏画面
void Reshape(int w,int h);//窗口大小调整
void Idle();//空闲处理
void Keyboard(unsigned char key,int x,int y);
bool CanMove(int x,int y);//判断某一位值是否可移入
bool CanEat(int x,int y);//判断某一位值可以食用
void SnakeMove(int nDirect);//蛇移动0：上1：下2：左3：右
void AddTail();//蛇添加尾部
void RandFood();//随机产生一个合法的食物
//全局变量
int g_Map[50][50];//游戏地图(0：空白1：阻挡物2：食物)
struct SnakeInfo
{
	int x[1024],y[1024];//蛇的身体位值
	int nLength;//蛇的长度
	int nRunDirect;//蛇的行动方向
} g_Snake;

int main(int argc,char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("贪吃蛇");
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
				MessageBox(NULL,"墙撞到你了！呵呵！~","小蛇",MB_OK);
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

	for(y=0;y<50;y++)//绘制地图
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

	for(i=0;i<g_Snake.nLength;i++)//绘制蛇的身体
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
	case 'w'://上
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
			MessageBox(NULL,"墙撞到你了！呵呵！~","小蛇",MB_OK);
		}
		break;
	case 's'://下
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
			MessageBox(NULL,"墙撞到你了！呵呵！~","小蛇",MB_OK);
		}
		break;
	case 'a'://左
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
			MessageBox(NULL,"墙撞到你了！呵呵！~","小蛇",MB_OK);
		}
		break;
	case 'd'://右
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
			MessageBox(NULL,"墙撞到你了！呵呵！~","小蛇",MB_OK);
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

	//初始化游戏地图
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

	//初始化食物
	g_Map[20][20]=MAP_FOOD;

	//初始化蛇
	g_Snake.x[0]=25;
	g_Snake.y[0]=25;
	g_Snake.x[1]=25;
	g_Snake.y[1]=24;
	g_Snake.nLength=2;
	g_Snake.nRunDirect=-1;//初始蛇不行动

	for(i=2;i<1024;i++)
	{
		g_Snake.x[i]=-100;
		g_Snake.y[i]=-100;
	}
}
