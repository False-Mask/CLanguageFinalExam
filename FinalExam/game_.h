#pragma once
//�ӵ�
typedef struct BULLET
{
	int x;
	int y;
	int kind;
	BULLET* next;
}Bullet;
void initView();//��ʼ����ͼ
void loadResource();//������Դ�ļ�
void openCountThread();//������¼ʱ����߳�
void dealKeyEvent(int primaryX,int dx,int primaryY,int dy,int width,int height);//��������¼�
void* countTime(void* args);//
void sendBullet(int x, int y);
void flushPlane(int primaryX,int dx ,int primaryY,int dy,Bullet* head);
void flushBullet(Bullet * bullet);
void lockTheKeyBoard();