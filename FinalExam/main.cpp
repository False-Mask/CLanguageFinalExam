#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include"controller.h"
int main() {
	newAGraphics(600,600,RGB(175, 238, 238) );  //��ʼ��һ��ͼ�ν���
	int resultCode = initMenu(200,200,400,250,20,LIGHTBLUE,WHITE);	//��ʼ��һ��ѡ��˵�
	handleUserOption(resultCode);

	//char x = getchar();	 //���Խ׶������߳�
	closegraph(); //�ر�
	return 0;
}
