#include<graphics.h>
//#include<stdlib.h>
#include<malloc.h>
#include<stdio.h>
#include<conio.h>
#include<pthread.h>
#include"game_.h"


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
IMAGE plane1[2];
IMAGE plane2[2];
IMAGE bullet1[2];
IMAGE bullet2[2];


void  initView() {

	loadResource();

	int width = background.getwidth();
	int height = background.getheight();

	initgraph(width , height);

	putimage(0, 0, &background);

	int primaryX = width / 2 - 51 ;
	int primaryY = height - 130;

	//变化量
	int dx = 0;
	int dy = 0;

	dealKeyEvent(primaryX,dx,primaryY,dy,width,height);

	return;
}


void loadResource() {

	loadimage(&background, "background.png");

	loadimage(&plane1[0], "me1.png");
	loadimage(&plane1[1], "me1y.png");

	loadimage(&plane2[0], "me2.png");
	loadimage(&plane2[1], "me2y.png");

	loadimage(&bullet1[0], "bullet1.png");
	loadimage(&bullet1[1], "bullet1y.png");

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


Bullet* head = 0;
bool isLocked = false;

void dealKeyEvent(int primaryX, int dx, int primaryY, int dy,  int width, int height) {
	Bullet* ptr= 0 ;

	BeginBatchDraw();
	while (true)
	{
		putimage(0, 0, &background);
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
				}
				break;
			case 's':
			case 'S':
				if (dy + primaryY < height - 126)//下边界
				{
					dy += 8;
				}
				break;
			case 'a':
			case 'A':
				if (dx + primaryX > 0)//左边界
				{
					dx -= 8;
				}
				break;
			case 'd':
			case 'D':
				if (dx + primaryX < width - 102)//右边界
				{
					dx += 8;
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
					if (head == 0)
					{
						//
						head = (Bullet*)malloc(sizeof(Bullet));
						head->next = 0;
						ptr = head;
					}
					ptr->x = dx + primaryX + 51 - 2;
					ptr->y = dy + primaryY - 12;
					ptr = ptr->next = (Bullet*)malloc(sizeof(Bullet));
					ptr->next = 0;
				}
				break;
			default:
				break;
			}

		}
		//刷新布局
		flushPlane(primaryX, dx, primaryY, dy,head);
	}
	EndBatchDraw();
}


void flushPlane(int primaryX, int dx, int primaryY, int dy,Bullet*head) {

	
	putimage(primaryX + dx, primaryY + dy, &plane2[1], SRCAND);
	putimage(primaryX + dx, primaryY + dy, &plane2[0], SRCPAINT); 
	//判空
	if (head!=0)
	{
		flushBullet(head);
	}
	/*putimage(dx + primaryX + 51 - 2, dy + primaryY - 12, &bullet1[1], SRCAND);
	putimage(dx + primaryX + 51 - 2, dy + primaryY - 12, &bullet1[0], SRCPAINT);*/
	FlushBatchDraw();

	putimage(primaryX + dx, primaryY + dy, &plane1[1], SRCAND);
	putimage(primaryX + dx, primaryY + dy, &plane1[0], SRCPAINT);
	/*FlushBatchDraw();*/
	FlushBatchDraw();
	return;
}

void flushBullet(Bullet * bullet) {
	Bullet* x = bullet;
	 while (x->next != 0) {
		putimage(x->x, x->y, &bullet1[1], SRCAND);
		putimage(x->x , x->y, &bullet1[0], SRCPAINT);
		x->y -= 1;
		if (x->y<=-11)
		{
			head = x->next;
			Bullet* free_ptr = x; 
			x = x->next;
			free(free_ptr);
		}else{
			x = x->next;
		}
	}
	/* Sleep(1);*/
}

void lockTheKeyBoard() {
	Sleep(250);
	isLocked = false;
}