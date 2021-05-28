#pragma once
//子弹
typedef struct BULLET
{
	int x;
	int y;
	int kind;
	BULLET* next;
}Bullet;
void initView();//初始化视图
void loadResource();//加载资源文件
void openCountThread();//开启记录时间的线程
void dealKeyEvent(int primaryX,int dx,int primaryY,int dy,int width,int height);//处理键盘事件
void* countTime(void* args);//
void sendBullet(int x, int y);
void flushPlane(int primaryX,int dx ,int primaryY,int dy,Bullet* head);
void flushBullet(Bullet * bullet);
void lockTheKeyBoard();