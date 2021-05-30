//忽视sprintf格式化的不安全报错提示使得编译通过
#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<pthread.h>
#include"game_.h"
#include<time.h>
#include<future>
#include<thread>
#include "flush.h"
using namespace std;
//初始化一下数据锁
mutex  enemyLock;
void playGame() {
	closegraph();

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
//含有锁的get方法
Enermys* getEnermysHeadAndLock() {
	//加一下锁
	enemyLock.lock();
	return enermysHead;
}

//与计数相关的结构体
EnemyCount enemyCount = {0,0,0,0,0,0,0,0,0,0};
EnemyCount getEnemyCountAndLock() {
	return enemyCount;
}


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
	enermyThread.detach();
	dealKeyEventThread.detach();

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
						bulletsHead->next = NULL;
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
	}
}


void flushView() {
	putimage(0, 0, &background);
	//判空
	if (bulletsHead != 0)
	{
		flushBullet(bulletsHead);
	}

	//刷新视图之前获取锁
	Enermys* enemyHeadPtr = getEnermysHeadAndLock();
	if (enemyHeadPtr != 0)
	{
		flushEnermy(enemyHeadPtr);
	}
	//释放锁
	enemyLock.unlock();


	//如果图片是可以看见的
	if (plane.y <= height)
	{
		putimage(plane.x, plane.y, &planeFly[2 * planeFlyFrame + 1], SRCAND);
		putimage(plane.x, plane.y, &planeFly[2 * planeFlyFrame], SRCPAINT);
		if (++planeFlyFrame == 2)
		{
			planeFlyFrame = 0;
		}
	}

	FlushBatchDraw();


	return;
}


void flushBullet(Bullets * bullet) {
	Bullets* x = bullet;
	 while (x->next != 0) {
		if (x->y > -11)
		 {
			 putimage(x->x, x->y, &bullet1[1], SRCAND);
			 putimage(x->x, x->y, &bullet1[0], SRCPAINT);
		 }
		x->y -= 1;
		if (x->y  <= -11)
		{
			bulletsHead = x->next;
			Bullets* free_ptr = x; 
			x = x->next;
			free(free_ptr);
		}
		else{
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
	//获取锁
	getEnermysHeadAndLock();
	//修改enermy 初始化头指针
	enermysHead = (Enermys*)malloc(sizeof(Enermys));
	enermysHead->next = NULL;
	enermysHead->last = NULL;
	//释放锁
	enemyLock.unlock();

	while (isPlaneAlive)
	{
		//获取锁	构建 临时指针
		Enermys* ptr = getEnermysHeadAndLock();
		//初始化enemry的数量
		initEnermyCount(&enemyCount);
		//指向最后一个节点
		while (ptr->next != 0)
		{
			ptr = ptr->next;
		}
		//初始化数据
		initEnermyData(enemyCount, ptr);
		//操做完成释放锁
		enemyLock.unlock();

		while (true)
		{
	//		//这两个涉及到结构体指针的初始化‘切忌’扩大作用域
	//		/*int aliveCount = 0;
	//		int freeCount = 0;*/

	//		//处理数据
	//		/*Enermys* loaclPtr = 0;
	//		Enermys* localHeadPtr = 0;

	//		Enermys* freeHeadPtr = 0;
	//		Enermys* freePtr = 0;*/
			Enermys* localPtr = getEnermysHeadAndLock();
	//		//取出所有活着的敌人 并把需要free放进另外一个链表
	//		//if (enermysHead != 0)
	//		//{
	//		//	ptr = enermysHead;
	//		//	//遍历所有
	//		//	while (ptr->next != 0)
	//		//	{
	//		//		//如果是第一个检索到的存活的战机 设置为局部的头指针
	//		//		if (ptr->y < height && aliveCount == 0)
	//		//		{
	//		//			localHeadPtr = (Enermys*)malloc(sizeof(Enermys));
	//		//			copyEnemy(ptr,localHeadPtr);
	//		//			loaclPtr = localHeadPtr;
	//		//			++aliveCount;
	//		//		}
	//		//		//如果是存活而且不是第一个，充当节点
	//		//		else if  (ptr->y < height)
	//		//		{
	//		//			loaclPtr = loaclPtr->next = (Enermys*)malloc(sizeof(Enermys));
	//		//			copyEnemy(ptr, loaclPtr);
	//		//			++aliveCount;
	//		//		}
	//		//		//如果死亡了，并且是死亡战机的第一个 ->充当头指针
	//		//		else if (ptr->bloodCount == 0 || (ptr->y >= height && freeCount == 0))
	//		//		{
	//		//			//记录击杀量和逃脱量
	//		//			if (ptr->bloodCount== 0)
	//		//			{
	//		//				killCount++;
	//		//			}
	//		//			else
	//		//			{
	//		//				passCount++;
	//		//			}
			spyEnemyData(localPtr);
	//		//			++freeCount;
	//		//		}
	//		//		//如果死亡
	//		//		else if (ptr->y >= height )
	//		//		{
	//		//			//记录击杀量和逃脱量
	//		//			if (ptr->bloodCount == 0)
	//		//			{
	//		//				killCount++;
	//		//			}
	//		//			else
	//		//			{
	//		//				passCount++;
	//		//			}
	//		//			//freePtr = freePtr->next = ptr;
	//		//			++freeCount;
	//		//		}
	//		//		ptr = ptr->next;
	//		//	}
	//		//	//最后一个节点没复制导致没执行一次少掉一个节点
	//		//	loaclPtr = loaclPtr->next = (Enermys*)malloc(sizeof(Enermys));
	//		//	copyEnemy(ptr, loaclPtr);
	//		//	//刷新
	//		//	if (freeHeadPtr != 0)
	//		//	{
	//		//		freePtr = freeHeadPtr;
	//		//		while (freePtr->next != 0)
	//		//		{
	//		//			Enermys* localFreePtr = freePtr;
	//		//			freePtr = freePtr->next;
	//		//			free(localFreePtr);
	//		//		}
	//		//	}
	//		//}
	//		///////////////////////////////
	//		/*freeHeadPtr = enermysHead;
	//		freePtr = freeHeadPtr;*/
	//		//更新数据的头指针
	//		//enermysHead = localHeadPtr;
	//		//刷新敌军的轨迹
			updataEnermy(enermysHead);


			//更新完成释放锁
			enemyLock.unlock();

		//	//当敌人小于总数的5分之一时候就开始发下一波
			EnemyCount count = enemyCount;
			if (count.currentAll <= count.all/40)
			{
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
		//确保图像是可以看见的才画
		if (ptr->y <height )
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
				putimage(ptr->x, ptr->y, &enemy3[2 * enemy3Frame + 1], SRCAND);
				putimage(ptr->x, ptr->y, &enemy3[2 * enemy3Frame], SRCPAINT);
				if (++enemy3Frame == 2)
				{
					enemy3Frame = 0;
				}
			}
		}


		//不可见 回收
		/*else
		{
			if (ptr->last == 0)
			{
				enermysHead = ptr->next;
				enermysHead->last = 0;
				countEnemyCount(ptr);
				free(ptr);
				ptr = enermysHead;
			}
			else
			{
				Enermys* freeData = ptr;
				Enermys* last = ptr->last;
				Enermys* next = ptr->next;
				last->next = next;
				next->last = last;
				ptr = ptr->last;
				countEnemyCount(ptr);
				free(freeData);
			}
		}*/


		if (ptr->next ==0 && ptr->last ==0)
		{
			break;
		}
		ptr = ptr->next;
	}
	return;
}

void initEnermyCount(EnemyCount* count) {
	//发兵
	int count1 = 3 + rand() % 4;
	int count2 = 1 + rand() % 1;

	count->enemy1+= count1;
	count->enemy2 += count2;

	count->currentEnemy1 += count1;
	count->currentEnemy2 += count2;
	//初始化敌军
	if (rand() % 100 <= 50)
	{
		count->enemy3 += 0;
		count->currentEnemy3 += 0;
	}
	else if (rand() % 100 <= 65)
	{
		count->enemy3 += 1;
		count->currentEnemy3 += 1;
	}
	else if (rand() % 100 <= 75)
	{
		count->enemy3 += 2;
		count->currentEnemy3 += 2;
	}
	else if (rand() % 100 <= 100)
	{
		count->enemy3 += 3;
		count->currentEnemy3 += 3;
	}
	count->all = count->enemy1 + count->enemy2 + count->enemy3;
	count->currentAll = count->currentEnemy1 + count->currentEnemy2 + count->currentEnemy3;
}

void initEnermyData(EnemyCount count, Enermys* ptr){
	int enermy1_count = count.currentEnemy1;
	int enermy2_count = count.currentEnemy2;
	int enermy3_count = count.currentEnemy3;
	Enermys*  last = 0;
	//初始化enemy1
	for (int i = 0; i < enermy1_count; i++){
		last = ptr;
		ptr->bloodCount = 1;
		ptr->speed = 3 + (rand() % 3);
		ptr->x = rand() % (width - 57);
		ptr->y = -51 - (rand() % 6);
		ptr->type = 1;
		ptr = ptr->next= (Enermys*)malloc(sizeof(Enermys));
		ptr->last = last;
		ptr->next = 0;
	}
	//初始化enemy2
	for (int i = 0; i < enermy2_count; i++){
		last = ptr;
		ptr-> bloodCount = 2;
		ptr-> speed = 2 + (rand() % 2);
		ptr-> x = rand() % (width - 69);
		ptr-> y = -99 - (rand() % 10);
		ptr-> type = 2;
		ptr = ptr->next = (Enermys*)malloc(sizeof(Enermys));
		ptr->last = last;
		ptr->next = 0;
	}
	//初始化enemy3
	for (int i = 0; i < enermy3_count; i++)
	{
		last = ptr;
		ptr->bloodCount = 5;
		ptr->speed = 1 + (rand() % 1);
		ptr->x = rand() % (width - 165);
		ptr->y = -261 - (rand() % 14);
		ptr->type = 3;
		ptr = ptr->next = (Enermys*)malloc(sizeof(Enermys));
		ptr->last = last;
		ptr->next = NULL;
	}
	return;
}

void updataEnermy(Enermys* ptr) {
	Enermys* localPtr = ptr;
	//更新数据
	if (localPtr != NULL)
	{
		while (localPtr->next != 0)
		{
			localPtr->y += localPtr->speed;
			localPtr = localPtr->next;
		}
	}
	return;
}

void copyEnemy(Enermys* src, Enermys* ptr) {
	ptr->bloodCount = src->bloodCount;
	ptr->speed = src->speed;
	ptr->type = src->type;
	ptr->x = src->x;
	ptr->y = src->y;
	ptr->next = 0 ;
	return;
}


void countEnemyCount(Enermys* ptr) {
	switch (ptr->type)
	{
	case 1:
		enemyCount.currentEnemy1--;
		break;
	case 2:
		enemyCount.currentEnemy2--;
		break;
	case 3:
		enemyCount.currentEnemy3--;
	default:
		break;
	}
	/*enemyLock.unlock();*/
	enemyCount.currentAll--;
	return;
}


void spyEnemyData(Enermys * ptr) {
	Enermys* localPtr = ptr;
	//安全措施
	if (localPtr==NULL ||(localPtr->last ==NULL&& localPtr->next==NULL))
	{
		return;
	}
	while (localPtr-> next !=0)
	{
		//不可见状态
		if (localPtr-> y >= height)
		{
			//Free的策略
			if (localPtr->last == 0)
			{
				enermysHead = localPtr->next;
				enermysHead->last = NULL;
				countEnemyCount(localPtr);
				free(localPtr);
				localPtr = enermysHead;
			}
			else
			{
				Enermys* freeData = localPtr;
				Enermys* last = localPtr->last;
				Enermys* next = localPtr->next;
				last->next = next;
				next->last = last;
				localPtr = localPtr->last;
				countEnemyCount(localPtr);
				free(freeData);
			}
		}
		if (localPtr->last ==NULL && localPtr->next == NULL)
		{
			break;
		}
		localPtr = localPtr->next;
	}
}
