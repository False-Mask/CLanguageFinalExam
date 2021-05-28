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
}Bullets;//�ӵ�Ⱥ
typedef struct ENEMYS {
	int bloodCount;//Ѫ�� ��ʽӢ�� hh
	int x;//x������
	int y;//y������
	int speed;//�о����˶��ٶ�
	int type;//�о�������
	ENEMYS* next;
}Enermys;
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
void flushEnermy(Enermys* enermyHead);//ˢ��enermy��ʾ