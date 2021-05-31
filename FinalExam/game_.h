#pragma 
typedef struct PLANE {
	int bloodCount = 1;
	bool isAlive = true;
	int x = 0;
	int y = 0;
}Plane;
typedef struct BULLETS
{
	int x;//x������
	int y;//y������
	int kind;//�ӵ�����
	BULLETS* next;//Ϊ�˷����ͼ��ʱ��һ���� ����Ϊһ������
	BULLETS* last;
}Bullets;//�ӵ�Ⱥ
typedef struct ENEMYS {
	int bloodCount;//Ѫ�� ��ʽӢ�� hh
	int x;//x������
	int y;//y������
	int speed;//�о����˶��ٶ�
	int type;//�о�������
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


void initView();//��ʼ����ͼ
void loadResource();//������Դ�ļ�
void openCountThread();//������¼ʱ����߳�
void dealKeyEvent(int primaryX,int dx,int primaryY,int dy,int width,int height);//��������¼�
void* countTime(void* args);//
void sendBullet(int x, int y);
void flushView();
void flushBullet(Bullets * bullet);
void lockTheKeyBoard();//��ȴ�ո������֧�̣߳�
void manageEnemy();//�ַ��б�����֧�̣߳�
void flushEnermy();//ˢ��enermy��ʾ
void initEnermyCount(EnemyCount* enemyCount);//��ʼ�������ĸ���
void initEnermyData(EnemyCount count, Enermys* ptr);//��ʼ��enermy������
void updataEnermy(Enermys * ptr);//���µо����˶��켣
void copyEnemy(Enermys* src,Enermys* ptr);//����һ���µ�Enemys
EnemyCount getEnemyCountAndLock();
void spyEnemyData(Enermys* ptr);//�۲�enemy�ı䶯
bool judgeBulletsHitEnemy(Bullets * localBullets, Enermys* localPtr);//����Ƿ����
void updataBullets(Bullets* bulletsPtr);//���·�����ӵ�
Enermys* enemyLinkedListRemove(Enermys* ptr);
void updataFrame();//����֡��