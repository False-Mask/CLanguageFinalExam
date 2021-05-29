//忽视sprintf格式化的不安全报错提示使得编译通过
#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<pthread.h>
#include"game_.h"
#include<time.h>

#include<thread>
//#include<vector>
//#include<iostream>
//#include<map>
//#include<string>
using namespace std;

void playGame() {
	closegraph();

	//openCountThread();

	initView();



	//阻塞
	int x = getchar();
	return;
}

IMAGE background;
IMAGE planeFly[4];
IMAGE plane1[2];
IMAGE plane2[2];
IMAGE bullet1[2];
IMAGE bullet2[2];
//敌人1
IMAGE enemy1[2];
IMAGE enemy1_down[8];
//敌人2
IMAGE enemy2[2];
IMAGE enermy2_hit[2];
IMAGE enermy2_down[8];
//敌人3
IMAGE enemy3[4];
IMAGE enemy3_down[12];

//子弹群的头指针
Bullets* bulletsHead = 0;
//敌人的头指针
Enermys* enermysHead = 0;
//飞机
Plane plane;

bool isLocked = false;
bool isPlaneAlive = true;

int width;
int height;

int planeFlyFrame = 0;
int enemy3Frame = 0;

void  initView() {

	loadResource();

	width = background.getwidth();
	height = background.getheight();

	initgraph(width , height);

	putimage(0, 0, &background);

	int primaryX = width / 2 - 51 ;
	int primaryY = height - 130;
	

	//变化量
	int dx = 0;
	int dy = 0;

	//开线程管理敌人的出兵
	thread enermyThread(manageEnemy);

	thread dealKeyEventThread(dealKeyEvent, primaryX, dx, primaryY, dy, width, height);


	//初始化plane的值
	plane.x = primaryX;
	plane.y = primaryY;

	//dealKeyEvent(primaryX,dx,primaryY,dy,width,height);
	BeginBatchDraw();
	while (true)
	{
		flushView();
	}
	EndBatchDraw();
	return;
}


void loadResource() {
	//加载背景
	loadimage(&background, "background.png");
	
	//加载飞机
	loadimage(&planeFly[0], "me1.png");
	loadimage(&planeFly[1], "me1y.png");

	loadimage(&planeFly[2], "me2.png");
	loadimage(&planeFly[3], "me2y.png");
	//加载子弹
	loadimage(&bullet1[0], "bullet1.png");
	loadimage(&bullet1[1], "bullet1y.png");

	//加载敌人图片
	loadimage(&enemy1[0], "enemy1.png");
	loadimage(&enemy1[1], "enemy1y.png");
	for (int i = 0; i < 4; i++)
	{
		char name[20];
		sprintf(name, "enemy1_down%d.png", i + 1);
		loadimage(&enemy1_down[2*i], name);
		sprintf(name, "enemy1_down%dy.png", i + 1);
		loadimage(&enemy1_down[2*i+1], name);
	}

	loadimage(&enemy2[0], "enemy2.png");
	loadimage(&enemy2[1], "enemy2y.png");
	loadimage(&enermy2_hit[0], "enemy2_hit.png");
	loadimage(&enermy2_hit[1], "enemy2_hity.png");
	for (int  i = 0; i < 4; i++)
	{
		char name[20];
		sprintf(name, "enemy2_down%d.png", i + 1);
		loadimage(&enermy2_down[2*i], name);
		sprintf(name, "enemy2_down%dy.png", i + 1);
		loadimage(&enermy2_down[2 * i+1], name);
	}

	loadimage(&enemy3[0], "enemy3_n1.png");
	loadimage(&enemy3[1], "enemy3_n1y.png");
	loadimage(&enemy3[2], "enemy3_n2.png");
	loadimage(&enemy3[3], "enemy3_n2y.png");
	for (int i = 0; i < 6; i++)
	{
		char name[20];
		sprintf(name, "enemy3_down%d.png", i + 1);
		loadimage(&enemy3_down[2 * i], name);
		sprintf(name, "enemy3_down%dy.png", i + 1);
		loadimage(&enemy3_down[2 * i+1], name);
	}

	return;
}


void openCountThread() {
	/*thread countThread("123");
	countThread.join();*/
	/*pthread_t countThread;
	pthread_create(&countThread,NULL,countTime,NULL);*/
	return;
}


void* countTime(void* args) {

	printf("hello world");
		return NULL;
}


void sendBullet(int x,int y ) {
	IMAGE  bullet;
	loadimage(&bullet, "bullet1.png");
	putimage(x, y, &bullet);
	return;
}


void dealKeyEvent(int primaryX, int dx, int primaryY, int dy,  int width, int height) {
	Bullets* ptr= 0 ;
	while (true)
	{
		if (_kbhit())
		{
			char input = _getch();
			switch (input)
			{
			case 'w':
			case 'W':
				if (dy + primaryY > 5)//上边界
				{
					dy -= 8;
					plane.y = primaryY + dy;
				}
				break;
			case 's':
			case 'S':
				if (dy + primaryY < height - 126)//下边界
				{
					dy += 8;
					plane.y = primaryY + dy;
				}
				break;
			case 'a':
			case 'A':
				if (dx + primaryX > 0)//左边界
				{
					dx -= 8;
					plane.x = primaryX + dx;
				}
				break;
			case 'd':
			case 'D':
				if (dx + primaryX < width - 102)//右边界
				{
					dx += 8;
					plane.x = primaryX + dx;
				}
				break;
			case ' ':
				if (!isLocked)
				{
					//防止下一次事件的进入
					isLocked = true;
					//锁住键盘冷却发射
					thread unlock(lockTheKeyBoard);
					unlock.detach();
					if (bulletsHead == 0)
					{
						//
						bulletsHead = (Bullets*)malloc(sizeof(Bullets));
						bulletsHead->next = 0;
						ptr = bulletsHead;
					}
					ptr->x = dx + primaryX + 51 - 2;
					ptr->y = dy + primaryY - 12;
					ptr = ptr->next = (Bullets*)malloc(sizeof(Bullets));
					ptr->next = 0;
				}
				break;
			default:
				break;
			}

		}
		//刷新布局
		//flushView();
	}
}


void flushView() {
	putimage(0, 0, &background);

	putimage(plane.x, plane.y, &planeFly[2 * planeFlyFrame + 1], SRCAND);
	putimage(plane.x, plane.y, &planeFly[2 * planeFlyFrame], SRCPAINT);
	if (++planeFlyFrame==2)
	{
		planeFlyFrame = 0;
	}

	//判空
	if (bulletsHead != 0)
	{
		flushBullet(bulletsHead);
	}

	if (enermysHead != 0)
	{
		flushEnermy(enermysHead);
	}
	FlushBatchDraw();

	return;
}


void flushBullet(Bullets * bullet) {
	Bullets* x = bullet;
	 while (x->next != 0) {
		putimage(x->x, x->y, &bullet1[1], SRCAND);
		putimage(x->x , x->y, &bullet1[0], SRCPAINT);
		x->y -= 1;
		if (x->y<=-11)
		{
			bulletsHead = x->next;
			Bullets* free_ptr = x; 
			x = x->next;
			free(free_ptr);
		}else{
			x = x->next;
		}
	}
}


void lockTheKeyBoard() {
	Sleep(250);
	isLocked = false;
}


void manageEnemy() {
	//引入伪随机数生成器
	srand(time(NULL));

	while (isPlaneAlive)
	{

		//初始化头指针
		enermysHead = (Enermys*)malloc(sizeof(Enermys));
		enermysHead->next = 0;
		Enermys* ptr = enermysHead;//临时指针

		int enermy1_count = 3 + rand() % 4;
		int enermy2_count = 1 + rand() % 1;
		int enermy3_count = 0;
		if (rand() % 100 <= 50)
		{
			enermy3_count = 0;
		}
		else if (rand() % 100 <= 65)
		{
			enermy3_count = 1;
		}
		else if (rand() % 100 <= 75)
		{
			enermy3_count = 2;
		}
		else if (rand() % 100 <= 100)
		{
			enermy3_count = 3;
		}

		//指向最后一个节点
		while (ptr->next != 0)
		{
			ptr = ptr->next;
		}
		//初始化enemy1
		for (int i = 0; i < enermy1_count; i++)
		{
			ptr->bloodCount = 1;
			ptr->speed = 3 + (rand() % 3);
			ptr->x = rand() % (width - 57);
			ptr->y = -51 - (rand() % 6);
			ptr->type = 1;
			ptr = ptr->next = (Enermys*)malloc(sizeof(Enermys));
			ptr->next = 0;
		}
		//初始化enemy2
		for (int i = 0; i < enermy2_count; i++)
		{
			ptr->bloodCount = 2;
			ptr->speed = 2 + (rand() % 2);
			ptr->x = rand() % (width - 69);
			ptr->y = -99 - (rand() % 10);
			ptr->type = 2;
			ptr = ptr->next = (Enermys*)malloc(sizeof(Enermys));
			ptr->next = 0;
		}
		//初始化enemy3
		for (int i = 0; i < enermy3_count; i++)
		{
			ptr->bloodCount = 5;
			ptr->speed = 1 + (rand() % 1);
			ptr->x = rand() % (width - 165);
			ptr->y = -261 - (rand() % 14);
			ptr->type = 3;
			ptr = ptr->next = (Enermys*)malloc(sizeof(Enermys));
			ptr->next = 0;
		}
		//处理数据
		while (true)
		{
			Enermys* loaclPtr = 0;
			Enermys* localHeadPtr = 0;

			Enermys* freeHeadPtr = 0;
			Enermys* freePtr = 0;

			int aliveCount = 0;
			int freeCount = 0;
			//取出所有活着的敌人 并把需要free放进另外一个链表
			if (enermysHead != 0)
			{
				ptr = enermysHead;
				while (ptr->next != 0)
				{
					if (ptr->y < height && ++aliveCount == 1)
					{
						localHeadPtr = ptr;
						loaclPtr = localHeadPtr;
					}
					else if (ptr->y < height && ++aliveCount)
					{
						loaclPtr = loaclPtr->next = ptr;
					}
					else if (ptr->y >= height && ++freeCount == 1)
					{
						freeHeadPtr = ptr;
						freePtr = freeHeadPtr;
					}
					else if (ptr->y >= height && ++freeCount)
					{
						freePtr = freePtr->next = ptr;
					}
					ptr = ptr->next;
				}
			}
			//更新数据的头指针
			enermysHead = localHeadPtr;
			//更新数据
			if (enermysHead != 0)
			{
				ptr = enermysHead;
				while (ptr->next != 0)
				{
					ptr->y += ptr->speed;
					ptr = ptr->next;
				}
			}
			//把需要free掉需要free内容
			/*if (freeHeadPtr != 0)
			{
				freePtr = freeHeadPtr;
				while (freePtr->next != 0)
				{
					Enermys* localFreePtr = freePtr;
					freePtr = freePtr->next;
					free(localFreePtr);
				}
			}*/
			if (enermysHead == 0)
			{
				if (freeHeadPtr != 0)
				{
					freePtr = freeHeadPtr;
					while (freePtr->next != 0)
					{
						Enermys* localFreePtr = freePtr;
						freePtr = freePtr->next;
						free(localFreePtr);
					}
				}
				break;
			}
			Sleep(25);
		}
	}
	return;
}


void flushEnermy(Enermys* enermyHead) {
	Enermys* ptr = enermyHead;
	while (ptr->next != 0)
	{
		if (ptr->type == 1)
		{
			putimage(ptr->x, ptr->y, &enemy1[1], SRCAND);
			putimage(ptr->x, ptr->y, &enemy1[0], SRCPAINT);
		}
		else if (ptr->type == 2)
		{
			putimage(ptr->x, ptr->y, &enemy2[1], SRCAND);
			putimage(ptr->x, ptr->y, &enemy2[0], SRCPAINT);
		}
		else if (ptr->type == 3)
		{
			putimage(ptr->x, ptr->y, &enemy3[2*enemy3Frame+1], SRCAND);
			putimage(ptr->x, ptr->y, &enemy3[2 * enemy3Frame], SRCPAINT);
			if (++enemy3Frame == 2)
			{
				enemy3Frame = 0;
			}
		}
		ptr = ptr->next;
	}
	//Sleep(1);
	return;
}
