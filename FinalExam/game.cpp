//����sprintf��ʽ���Ĳ���ȫ������ʾʹ�ñ���ͨ��
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
//��ʼ��һ��������
mutex  enemyLock;
void playGame() {
	closegraph();

	initView();

	//����
	int x = getchar();
	return;
}

IMAGE background;
IMAGE planeFly[4];
IMAGE plane1[2];
IMAGE plane2[2];
IMAGE bullet1[2];
IMAGE bullet2[2];
//����1
IMAGE enemy1[2];
IMAGE enemy1_down[8];
//����2
IMAGE enemy2[2];
IMAGE enermy2_hit[2];
IMAGE enermy2_down[8];
//����3
IMAGE enemy3[4];
IMAGE enemy3_down[12];

//�ӵ�Ⱥ��ͷָ��
Bullets* bulletsHead = 0;
//���˵�ͷָ��
Enermys* enermysHead = 0;
//��������get����
Enermys* getEnermysHeadAndLock() {
	//��һ����
	enemyLock.lock();
	return enermysHead;
}

//�������صĽṹ��
EnemyCount enemyCount = {0,0,0,0,0,0,0,0,0,0};
EnemyCount getEnemyCountAndLock() {
	return enemyCount;
}


//�ɻ�
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
	

	//�仯��
	int dx = 0;
	int dy = 0;

	//���̹߳�����˵ĳ���
	thread enermyThread(manageEnemy);

	thread dealKeyEventThread(dealKeyEvent, primaryX, dx, primaryY, dy, width, height);
	enermyThread.detach();
	dealKeyEventThread.detach();

	//��ʼ��plane��ֵ
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
	//���ر���
	loadimage(&background, "background.png");
	
	//���طɻ�
	loadimage(&planeFly[0], "me1.png");
	loadimage(&planeFly[1], "me1y.png");

	loadimage(&planeFly[2], "me2.png");
	loadimage(&planeFly[3], "me2y.png");
	//�����ӵ�
	loadimage(&bullet1[0], "bullet1.png");
	loadimage(&bullet1[1], "bullet1y.png");

	//���ص���ͼƬ
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
				if (dy + primaryY > 5)//�ϱ߽�
				{
					dy -= 8;
					plane.y = primaryY + dy;
				}
				break;
			case 's':
			case 'S':
				if (dy + primaryY < height - 126)//�±߽�
				{
					dy += 8;
					plane.y = primaryY + dy;
				}
				break;
			case 'a':
			case 'A':
				if (dx + primaryX > 0)//��߽�
				{
					dx -= 8;
					plane.x = primaryX + dx;
				}
				break;
			case 'd':
			case 'D':
				if (dx + primaryX < width - 102)//�ұ߽�
				{
					dx += 8;
					plane.x = primaryX + dx;
				}
				break;
			case ' ':
				if (!isLocked)
				{
					//��ֹ��һ���¼��Ľ���
					isLocked = true;
					//��ס������ȴ����
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
	//�п�
	if (bulletsHead != 0)
	{
		flushBullet(bulletsHead);
	}

	//ˢ����ͼ֮ǰ��ȡ��
	Enermys* enemyHeadPtr = getEnermysHeadAndLock();
	if (enemyHeadPtr != 0)
	{
		flushEnermy(enemyHeadPtr);
	}
	//�ͷ���
	enemyLock.unlock();


	//���ͼƬ�ǿ��Կ�����
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

	//����α�����������
	srand(time(NULL));
	//��ȡ��
	getEnermysHeadAndLock();
	//�޸�enermy ��ʼ��ͷָ��
	enermysHead = (Enermys*)malloc(sizeof(Enermys));
	enermysHead->next = NULL;
	enermysHead->last = NULL;
	//�ͷ���
	enemyLock.unlock();

	while (isPlaneAlive)
	{
		//��ȡ��	���� ��ʱָ��
		Enermys* ptr = getEnermysHeadAndLock();
		//��ʼ��enemry������
		initEnermyCount(&enemyCount);
		//ָ�����һ���ڵ�
		while (ptr->next != 0)
		{
			ptr = ptr->next;
		}
		//��ʼ������
		initEnermyData(enemyCount, ptr);
		//��������ͷ���
		enemyLock.unlock();

		while (true)
		{
	//		//�������漰���ṹ��ָ��ĳ�ʼ�����мɡ�����������
	//		/*int aliveCount = 0;
	//		int freeCount = 0;*/

	//		//��������
	//		/*Enermys* loaclPtr = 0;
	//		Enermys* localHeadPtr = 0;

	//		Enermys* freeHeadPtr = 0;
	//		Enermys* freePtr = 0;*/
			Enermys* localPtr = getEnermysHeadAndLock();
	//		//ȡ�����л��ŵĵ��� ������Ҫfree�Ž�����һ������
	//		//if (enermysHead != 0)
	//		//{
	//		//	ptr = enermysHead;
	//		//	//��������
	//		//	while (ptr->next != 0)
	//		//	{
	//		//		//����ǵ�һ���������Ĵ���ս�� ����Ϊ�ֲ���ͷָ��
	//		//		if (ptr->y < height && aliveCount == 0)
	//		//		{
	//		//			localHeadPtr = (Enermys*)malloc(sizeof(Enermys));
	//		//			copyEnemy(ptr,localHeadPtr);
	//		//			loaclPtr = localHeadPtr;
	//		//			++aliveCount;
	//		//		}
	//		//		//����Ǵ����Ҳ��ǵ�һ�����䵱�ڵ�
	//		//		else if  (ptr->y < height)
	//		//		{
	//		//			loaclPtr = loaclPtr->next = (Enermys*)malloc(sizeof(Enermys));
	//		//			copyEnemy(ptr, loaclPtr);
	//		//			++aliveCount;
	//		//		}
	//		//		//��������ˣ�����������ս���ĵ�һ�� ->�䵱ͷָ��
	//		//		else if (ptr->bloodCount == 0 || (ptr->y >= height && freeCount == 0))
	//		//		{
	//		//			//��¼��ɱ����������
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
	//		//		//�������
	//		//		else if (ptr->y >= height )
	//		//		{
	//		//			//��¼��ɱ����������
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
	//		//	//���һ���ڵ�û���Ƶ���ûִ��һ���ٵ�һ���ڵ�
	//		//	loaclPtr = loaclPtr->next = (Enermys*)malloc(sizeof(Enermys));
	//		//	copyEnemy(ptr, loaclPtr);
	//		//	//ˢ��
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
	//		//�������ݵ�ͷָ��
	//		//enermysHead = localHeadPtr;
	//		//ˢ�µо��Ĺ켣
			updataEnermy(enermysHead);


			//��������ͷ���
			enemyLock.unlock();

		//	//������С��������5��֮һʱ��Ϳ�ʼ����һ��
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
		//ȷ��ͼ���ǿ��Կ����ĲŻ�
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


		//���ɼ� ����
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
	//����
	int count1 = 3 + rand() % 4;
	int count2 = 1 + rand() % 1;

	count->enemy1+= count1;
	count->enemy2 += count2;

	count->currentEnemy1 += count1;
	count->currentEnemy2 += count2;
	//��ʼ���о�
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
	//��ʼ��enemy1
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
	//��ʼ��enemy2
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
	//��ʼ��enemy3
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
	//��������
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
	//��ȫ��ʩ
	if (localPtr==NULL ||(localPtr->last ==NULL&& localPtr->next==NULL))
	{
		return;
	}
	while (localPtr-> next !=0)
	{
		//���ɼ�״̬
		if (localPtr-> y >= height)
		{
			//Free�Ĳ���
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
