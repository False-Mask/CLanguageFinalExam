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
using namespace std;
//��ʼ��һ��������
mutex  enemyLock;
void playGame() {
	closegraph();

	initView();

	//����
	//int x = getchar();
	return;
}

IMAGE background;
IMAGE planeFly[4];
IMAGE plane1[2];
IMAGE plane2[2];
IMAGE bullet1[2];
IMAGE bullet2[2];
IMAGE me_destroy[8];
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
IMAGE enemy3_hit[2];
//�ӵ�Ⱥ��ͷָ��
Bullets* bulletsHead = 0;
//���˵�ͷָ��
Enermys* enermysHead = 0;

//�������صĽṹ��
EnemyCount enemyCount = {0,0,0,0,0,0,0,0,0,0};


//�ɻ�
Plane plane;

bool isLocked = false;
bool isPlaneAlive = true;

int width;
int height;

int planeFlyFrame = 0;
int enemy3Frame = 0;

void  initView() {

	A:loadResource();

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
	while (true && isPlaneAlive)
	{
		flushView();
	}
	EndBatchDraw();
	//������
	putimage(0, 0, &background);

	settextstyle(30, 0, "Arial", 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	setlinecolor(BLACK);

	int left1 = 480 / 2 - 60;
	int top1 = 700 / 2 - 110;
	int right1 = 480 / 2 + 60;
	int bottom1 = 700 / 2 - 60;
	outtextxy(480/2 - 50, 700/2 - 100, "���¿�ʼ");
	roundrect(480 / 2 - 60, 700 / 2 - 110, 480 / 2 +60, 700 / 2 - 60,10,10);

	int left2 = 480 / 2 - 60;
	int top2 = 700 / 2 - 10;
	int right2 = 480 / 2 + 60;
	int bottom2 = 700 / 2 + 40;
	outtextxy(480 / 2 - 50, 700 / 2 , "�˳���Ϸ");
	roundrect(480 / 2 - 60, 700 / 2 - 10, 480 / 2 + 60, 700 / 2 + 40, 10, 10);
	MOUSEMSG m;
	while (true) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONUP)
		{
			if (m.x >= left1 && m.x<= right1 && m.y>=top1 && m.y<= bottom1)
			{
				isPlaneAlive = true;
				plane.isAlive = true;
				plane.destroyFrame = 4;
				goto A;
			}
			if (m.x >= left2 && m.x<= right2 && m.y >= top2 && m.y <= bottom2) {
				break;
			}
		}
	}
	return;
}


void loadResource() {
	//���ر���
	loadimage(&background, "background.png");
	
	//���طɻ�
	loadimage(&planeFly[0], "me1.png");
	loadimage(&planeFly[1], "me1y.png");
	for (int i = 0; i < 4; i++)
	{
		char name[20];
		sprintf(name, "me_destroy_%d.png", i + 1);
		loadimage(&me_destroy[2 * i], name);
		sprintf(name, "me_destroy_%dy.png", i + 1);
		loadimage(&me_destroy[2 * i+1], name);
	}

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
	loadimage(&enemy3_hit[0], "enemy3_hit.png");
	loadimage(&enemy3_hit[1], "enemy3_hity.png");
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
	Bullets* ptr= NULL;
	Bullets* lastPtr = NULL;
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
					enemyLock.lock();
					ptr = bulletsHead;
					if (bulletsHead == NULL)
					{
						//
						bulletsHead = (Bullets*)malloc(sizeof(Bullets));
						bulletsHead->next = NULL;
 						bulletsHead->last = NULL;
						ptr = bulletsHead;
					}
					else
					{
						while (ptr->next!=NULL)
						{
							ptr = ptr->next;
						}
					}
					ptr->x = dx + primaryX + 51 - 2;
					ptr->y = dy + primaryY - 12;
					lastPtr = ptr;
					ptr = ptr->next = (Bullets*)malloc(sizeof(Bullets));
					ptr->last = lastPtr;
					ptr->next = NULL;
					enemyLock.unlock();
				}
				break;
			default:
				break;
			}
		}
		if (!isPlaneAlive)
		{
			break;
		}
	}
}


void flushView() {
	putimage(0, 0, &background);
	//�п�
	if (bulletsHead != 0)
	{
		enemyLock.lock();
		flushBullet(bulletsHead);
		enemyLock.unlock();
	}

	//ˢ����ͼ֮ǰ��ȡ��
	Enermys* enemyHeadPtr = enermysHead;
	if (enemyHeadPtr != 0)
	{
		enemyLock.lock();
		flushEnermy();
		enemyLock.unlock();
	}
	//�ͷ���


	//���ͼƬ�ǿ��Կ�����
	if (plane.isAlive)
	{
		enemyLock.lock();
		putimage(plane.x, plane.y, &planeFly[2 * planeFlyFrame + 1], SRCAND);
		putimage(plane.x, plane.y, &planeFly[2 * planeFlyFrame], SRCPAINT);
		enemyLock.unlock();
		if (++planeFlyFrame == 2)
		{
			planeFlyFrame = 0;
		}
	}
	else if (!plane.isAlive && plane.destroyFrame>0)
	{
		int frame = 4 - plane.destroyFrame;
		putimage(plane.x, plane.y, &planeFly[2 *frame  + 1], SRCAND);
		putimage(plane.x, plane.y, &planeFly[2 * frame], SRCPAINT);
	}
	else if (plane.destroyFrame == 0) {
		isPlaneAlive = false;
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
		x = x->next;
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
	enemyLock.lock();
	//�޸�enermy ��ʼ��ͷָ��
	enermysHead = (Enermys*)malloc(sizeof(Enermys));
	enermysHead->next = NULL;
	enermysHead->last = NULL;
	//�ͷ���
	enemyLock.unlock();

	while (isPlaneAlive)
	{
		enemyLock.lock();
		//��ȡ��	���� ��ʱָ��
		Enermys* ptr = enermysHead;
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

		while (isPlaneAlive)
		{
			enemyLock.lock();

			Enermys* localPtr = enermysHead;
			spyEnemyData(localPtr);
			updataEnermy(enermysHead);
			updataBullets(bulletsHead);
			//����֡��
			updataFrame();
			//������С��������5��֮һʱ��Ϳ�ʼ����һ��
			EnemyCount count = enemyCount;
			//
			if (judgeIsAlive())
			{
				plane.isAlive = false;
			}
			if (count.currentAll <= count.all/15)
			{
				enemyLock.unlock();
				break;
			}
			enemyLock.unlock();
			Sleep(25);
		}

	}
	return;
}


void flushEnermy() {
	Enermys* ptr = enermysHead;
	while (ptr->next != 0)
	{
		//ȷ��ͼ���ǿ��Կ����ĲŻ�
		if (ptr->y <height )
		{
			if (ptr->type == 1)
			{
				if (!ptr -> isAlive && ptr->destroyFrame != 0 )
				{
					int frame = 4 -  ptr->destroyFrame;
					putimage(ptr->x, ptr->y, &enemy1_down[2*frame+1], SRCAND);
					putimage(ptr->x, ptr->y, &enemy1_down[2*frame], SRCPAINT);
					/*ptr->destroyFrame--;*/
				}
				else if(ptr->isAlive)
				{
					putimage(ptr->x, ptr->y, &enemy1[1], SRCAND);
					putimage(ptr->x, ptr->y, &enemy1[0], SRCPAINT);
				}
			}
			else if (ptr->type == 2)
			{
				if (!ptr->isAlive && ptr->destroyFrame != 0) {
					int frame = 4 - ptr->destroyFrame;
					putimage(ptr->x, ptr->y, &enermy2_down[2 * frame + 1], SRCAND);
					putimage(ptr->x, ptr->y, &enermy2_down[2 * frame], SRCPAINT);
					//ptr->destroyFrame--;
				}
				else if(ptr->isAlive)
				{
					putimage(ptr->x, ptr->y, &enemy2[1], SRCAND);
					putimage(ptr->x, ptr->y, &enemy2[0], SRCPAINT);
				}
			}
			else if (ptr->type == 3)
			{
				if (!ptr->isAlive && ptr->destroyFrame != 0) {
					int frame = 6 - ptr->destroyFrame;
					putimage(ptr->x, ptr->y, &enemy3_down[2 * frame + 1], SRCAND);
					putimage(ptr->x, ptr->y, &enemy3_down[2 * frame], SRCPAINT);
					//ptr->destroyFrame--;
				}
				else if (ptr->isAlive) {
					putimage(ptr->x, ptr->y, &enemy3[2 * enemy3Frame + 1], SRCAND);
					putimage(ptr->x, ptr->y, &enemy3[2 * enemy3Frame], SRCPAINT);
				}
			}
		}

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
		ptr->destroyFrame = 4;
		ptr->hitFrame = 1;
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
		ptr->destroyFrame = 4;
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
		ptr->destroyFrame = 6;
		ptr->hitFrame = 1;
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


void spyEnemyData(Enermys * ptr) {
	int enemy_1 = 0;
	int enemy_2 = 0;
	int enemy_3 = 0;
	Enermys* localPtr = ptr;
	//��ȫ��ʩ
	//�۲촦��enemys
	while (localPtr-> next !=0)
	{
		if (localPtr == NULL || (localPtr->last == NULL && localPtr->next == NULL))
		{
			enemyCount.currentEnemy1 = enemy_1;
			enemyCount.currentEnemy2 = enemy_2;
			enemyCount.currentEnemy3 = enemy_3;
			enemyCount.currentAll = enemy_3 + enemy_2 + enemy_1;
			return;
		}
		//���ɼ�״̬
		if (localPtr-> y >= height)
		{
			//Free�Ĳ���
			if (localPtr->last == NULL && localPtr->next!=NULL)
			{
				enermysHead = localPtr->next;
				enermysHead->last = NULL;
				free(localPtr);
				localPtr = enermysHead;
			}
			else if(localPtr->last !=NULL && localPtr->next!=NULL)
			{
				Enermys* freeData = localPtr;
				Enermys* last = localPtr->last;
				Enermys* next = localPtr->next;
				last->next = next;
				next->last = last;
				localPtr = localPtr->last;
				free(freeData);
			}
		}
		if (localPtr->last ==NULL && localPtr->next == NULL)
		{
			break;
		}
		switch (localPtr-> type)
		{
		case 1:
			enemy_1++;
			break;
		case 2:
			enemy_2++;
			break;
		case 3:
			enemy_3++;
			break;
		default:
			break;
		}
		localPtr = localPtr->next;
	}
	enemyCount.currentEnemy1 = enemy_1;
	enemyCount.currentEnemy2 = enemy_2;
	enemyCount.currentEnemy3 = enemy_3;
	enemyCount.currentAll = enemy_3+ enemy_2+ enemy_1;
	//�۲첢����bullet �����ӵ���ʱ��û�ӵ���ֱ���˳��ˣ� �ڲ����ж�enemy�Ĺ�������
	if (bulletsHead != NULL)
	{
		Bullets* localBullets = bulletsHead;
		while (localBullets->next != NULL) {
			if (localBullets->y <= -11)
			{
				//ͷ
				if (localBullets->last == NULL)
				{
					Bullets* freePtr = localBullets;
					localBullets = localBullets->next;
					free(freePtr);
					localBullets->last = NULL;
					bulletsHead = localBullets;
					break;
				}
				else
				{
					Bullets* free_ptr = localBullets;
					Bullets* next = localBullets->next;
					Bullets* last = localBullets->last;
					last->next = next;
					next->last = last;
					free(free_ptr);
				}
				localBullets = localBullets->next;
			}

			//��֤��ȫ
			if (localBullets->next == NULL && localBullets->last == NULL)
			{
				free(localBullets);
				bulletsHead = NULL;
				break;
			}

			//����ӵ��Ƿ���е���
			Enermys* localPtr = enermysHead;
			if (localPtr != NULL)
			{
				while (localPtr->next  != NULL) {
					//������
					bool flag = judgeBulletsHitEnemy(localBullets, localPtr);
					if (flag) {
 						localPtr->isHit = true;
   						Bullets* freePtr = localBullets;
						Bullets* last = localBullets->last;
						Bullets* next = localBullets->next;
						//�����ӵ�
   						if (last==NULL)
						{
							free(freePtr);
							localBullets = next;
							localBullets->last = NULL;
							bulletsHead = localBullets;
							//break;
						}
						else
						{
							last->next = next;
							next->last = last;
 							free(freePtr);
							localBullets = last;
						}
						//���� enemy
						if (localPtr !=NULL)
						{//��Ѫ������1
							if (localPtr->bloodCount>1)
							{
 								localPtr->bloodCount--;
							}//Ѫ������1 �Լ�֮��isAlive����Ϊfalse
							else if(localPtr->bloodCount == 1)
							{
								localPtr->bloodCount = 0;
								localPtr->isAlive = false;
							}//������� ����֡�ʻ�û�в����� ���Ȳ��ͷ��ڴ�
 							else if(localPtr->bloodCount == 0 && localPtr->destroyFrame > 0)
							{
								localPtr->destroyFrame--;
							}//�������֡���Ѿ���������� �ͽ��ڵ�����free��
							else if (localPtr->bloodCount == 0 && localPtr->destroyFrame== 0) {
								//����ڵ�
								localPtr = enemyLinkedListRemove(localPtr);
								if (localPtr->next ==NULL && localPtr->last == NULL)
								{
									break;
								}
							}
						}
					}
					localPtr = localPtr->next;
				}
			}
			if (localBullets->next==NULL)
			{
				bulletsHead = localBullets;
				return;
			}
			//ָ����һ���ڵ�
			localBullets = localBullets->next;
		}
	}

	//�����ӵ�Ϊ��ʱ������
	if (bulletsHead != NULL && bulletsHead->next == NULL && enermysHead!=NULL && enermysHead->next!=NULL)
	{
		//����ÿһ��enemy���Ƿ���Ҫ���յ�
		Enermys* enemyPtr = enermysHead;
		while (enemyPtr->next!=NULL)
		{
			if (enemyPtr->bloodCount == 0 && enemyPtr->destroyFrame == 0) {
				//����ڵ�
				enemyPtr = enemyLinkedListRemove(enemyPtr);
				if (enemyPtr->next == NULL && enemyPtr->last == NULL)
				{
					break;
				}
			}
			enemyPtr = enemyPtr->next;
		}
	}
	return;
	
}


bool judgeBulletsHitEnemy(Bullets * localBullets, Enermys * localPtr) {
		if (localPtr != NULL && localPtr->isAlive)
		{
			Enermys* ptr = localPtr;
			switch (ptr->type)
				{
					//57*43
				case 1:
					if (localBullets->x >= localPtr->x
						&& localBullets->x <= localPtr->x + 55
						&& localBullets->y >= localPtr->y
  						&& localBullets->y <= localPtr->y + 40)
					{
  						return true;
					}
					else
					{
						return false;
					}
					break;
					//69 * 99
				case 2:
					if (localBullets->x >= localPtr->x
						&& localBullets->x <= localPtr->x + 67
						&& localBullets->y >= localPtr->y
						&& localBullets->y <= localPtr->y + 97)
					{
						return true;
					}
					else
					{
						return false;
					}
					break;
					//169 * 258
				case 3:
					if (localBullets->x >= localPtr->x
						&& localBullets->x <= localPtr->x + 167
						&& localBullets->y >= localPtr->y
						&& localBullets->y <= localPtr->y + 256)
					{
						return true;
					}
					else
					{
						return false;
					}
					break;
				default:
					break;
				}
		}
		return false;
}


void updataBullets(Bullets* bulletsPtr) {
	if (bulletsPtr!=NULL)
	{
		Bullets* localPtr = bulletsPtr;
		while (localPtr->next!=NULL)
		{
			localPtr->y -= 10;
			localPtr = localPtr->next;
		}
	}
	return;
}


Enermys* enemyLinkedListRemove(Enermys* localPtr ) {
	if (localPtr!=NULL)
	{
		//Free�Ĳ���
		if (localPtr->last == NULL && localPtr->next!=NULL)
		{
 			enermysHead = localPtr->next;
			enermysHead->last = NULL;
			free(localPtr);
			localPtr = enermysHead;
			return localPtr;
		}
		else if (localPtr->last != NULL)
		{
			Enermys* freeData = localPtr;
			Enermys* last = localPtr->last;
			Enermys* next = localPtr->next;
			last->next = next;
			next->last = last;
			localPtr = localPtr->last;
			free(freeData);
			return localPtr;
		}
		else if (localPtr-> last == NULL && localPtr -> next==NULL)
		{
			return localPtr;
		}
	}
	return NULL;
}

void updataFrame() {
	//����enemy3
	if (++enemy3Frame == 2)
	{
		enemy3Frame = 0;
	}
	//ˢ��Enemy frame
	if (enermysHead != NULL)
	{
		Enermys* ptr = enermysHead;
		while (ptr->next != NULL)
		{
			if (!ptr->isAlive && ptr->destroyFrame >= 1)
			{
				ptr->destroyFrame--;
			}
			if (ptr->isHit && ptr->hitFrame >= 1)
			{
				ptr->hitFrame--;
			}
			else if (ptr->isHit && ptr->hitFrame == 0)
			{
				ptr->isHit = false;
				switch (ptr->type)
				{
				case 1:
					break;
				case 2:
					ptr->hitFrame = 2;
					break;
				case 3:
					ptr->hitFrame = 2;
					break;
				default:
					break;
				}
			}
			if (ptr->next == NULL && ptr->last == NULL)
			{
				break;
			}
			ptr = ptr->next;
		}
	}
	return;
}

bool judgeIsAlive() {
	bool flag = false;
	if (enermysHead != NULL && plane.isAlive==true) {
		Enermys* ptr = enermysHead;
		while (ptr->next != NULL) {
			int enemyCenterX = 0;
			int enemyCenterY = 0;
			//102 * 126
			int planeCenterX = plane.x +102 / 2;
			int planeCenterY = plane.y +126 / 2;
			int r = 0;
			//switch (ptr->type)
			//	{
			//		//57*43
			//	case 1:
			//		if (
			//			ptr-> y + 43 >= plane.y &&
			//			ptr-> y + 43 <= plane.y +126 &&
			//			(abs(ptr -> x - plane.x-102) <= 102 + 57 ||abs(plane.x- ptr -> x - 57) <= 102 + 57)
			//		){
			//			flag = true;
			//		}
			//		break;
			//		//69 * 99
			//	case 2:
			//		if (
			//			ptr->y + 99 >= plane.y &&
			//			ptr->y + 99 <= plane.y + 126 &&
			//			(abs(ptr->x - plane.x - 102) <= 102 + 69 || abs(plane.x - ptr->x - 69) <= 102 + 69)
			//		){
			//			flag = true;
			//		}
			//		break;
			//		//169 * 258
			//	case 3:
			//		if (
			//			ptr->y + 258 >= plane.y &&
			//			ptr->y + 258 <= plane.y + 126 &&
			//			(abs(ptr->x - plane.x - 102) <= 102 + 169 || abs(plane.x - ptr->x - 169) <= 102 + 169)
			//		){
			//			flag = true;
			//		}
			//		break;
			//	default:
			//		break;
			//	}
			switch (ptr->type) {
				//57*43
			case 1:
				enemyCenterX = ptr->x + 57 / 2;
				enemyCenterY = ptr->y + 43 / 2;
				r= sqrt(pow(enemyCenterX - planeCenterX, 2) + pow(enemyCenterY - planeCenterY, 2));
				if (r <= (102+45)/2)
				{
					flag = true;
				}
				break;
				//69 * 99
			case 2:
				enemyCenterX = ptr->x + 69 / 2;
				enemyCenterY = ptr->y + 99 / 2;
				r = sqrt(pow(enemyCenterX - planeCenterX, 2) + pow(enemyCenterY - planeCenterY, 2));
				if (r <= (102 + 69) / 2)
				{
					flag = true;
				}
				break;
				//169 * 258
			case 3:
				enemyCenterX = ptr->x + 169 / 2;
				enemyCenterY = ptr->y + 258 / 2;
				r = sqrt(pow(enemyCenterX - planeCenterX, 2) + pow(enemyCenterY - planeCenterY, 2));
				if (r <= (102 + 200) / 2)
				{
					flag = true;
				}
				break;
			default:
				break;
			}
			ptr = ptr->next;
		}
	}
	else if (plane.isAlive==false && plane.destroyFrame>0) {
		plane.destroyFrame--;
	}
	return flag;
}
