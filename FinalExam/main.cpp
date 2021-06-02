#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include"controller.h"
int main() {
	newAGraphics(600,600,RGB(175, 238, 238) );  //初始化一个图形界面
	int resultCode = initMenu(200,200,400,250,20,LIGHTBLUE,WHITE);	//初始化一个选择菜单
	handleUserOption(resultCode);

	//char x = getchar();	 //测试阶段阻塞线程
	closegraph(); //关闭
	return 0;
}
