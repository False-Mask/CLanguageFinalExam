#pragma 
typedef struct PLANE {
	int bloodCount = 1;
	bool isAlive = true;
	int x = 0;
	int y = 0;
}Plane;
typedef struct BULLETS
{
	int x;//x轴坐标
	int y;//y轴坐标
	int kind;//子弹类型
	BULLETS* next;//为了方便绘图的时候一把嗦 声明为一个链表
}Bullets;//子弹群
typedef struct ENEMYS {
	int bloodCount;//血量 中式英文 hh
	int x;//x轴坐标
	int y;//y轴坐标
	int speed;//敌军的运动速度
	int type;//敌军的类型
	ENEMYS* next;
}Enermys;
void initView();//初始化视图
void loadResource();//加载资源文件
void openCountThread();//开启记录时间的线程
void dealKeyEvent(int primaryX,int dx,int primaryY,int dy,int width,int height);//处理键盘事件
void* countTime(void* args);//
void sendBullet(int x, int y);
void flushView();
void flushBullet(Bullets * bullet);
void lockTheKeyBoard();//冷却空格键（分支线程）
void manageEnemy();//分发敌兵（分支线程）
void flushEnermy(Enermys* enermyHead);//刷新enermy显示