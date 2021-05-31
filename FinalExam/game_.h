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
	BULLETS* last;
}Bullets;//子弹群
typedef struct ENEMYS {
	int bloodCount;//血量 中式英文 hh
	int x;//x轴坐标
	int y;//y轴坐标
	int speed;//敌军的运动速度
	int type;//敌军的类型
	bool isAlive = true;
	bool isHit = false;
	int destroyFrame;
	int hitFrame = 0;
	ENEMYS* last;
	ENEMYS* next;
}Enermys;
typedef struct ENEMYCOUNT {
	unsigned long long all;
	unsigned long long enemy1;
	unsigned long long enemy2;
	unsigned long long enemy3;
	int currentEnemy1;
	int currentEnemy2;
	int currentEnemy3;
	int currentAll;
	int killCount;
	int passCount;
}EnemyCount;


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
void flushEnermy();//刷新enermy显示
void initEnermyCount(EnemyCount* enemyCount);//初始化发兵的个数
void initEnermyData(EnemyCount count, Enermys* ptr);//初始化enermy的数据
void updataEnermy(Enermys * ptr);//更新敌军的运动轨迹
void copyEnemy(Enermys* src,Enermys* ptr);//复制一份新的Enemys
EnemyCount getEnemyCountAndLock();
void spyEnemyData(Enermys* ptr);//观察enemy的变动
bool judgeBulletsHitEnemy(Bullets * localBullets, Enermys* localPtr);//检查是否击中
void updataBullets(Bullets* bulletsPtr);//更新发射的子弹
Enermys* enemyLinkedListRemove(Enermys* ptr);
void updataFrame();//更新帧率