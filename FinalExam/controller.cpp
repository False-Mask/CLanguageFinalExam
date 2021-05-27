#include<graphics.h>
#include<stdlib.h>
#include<stdio.h>
#include "menu.h"
#include"game.h"

int initMenu(int left ,int top , int right , int bottom , int space, COLORREF rgb, COLORREF textRgb) {
	 return openMenu(left,top,right,bottom,space,rgb,textRgb);
}


void newAGraphics(int width, int height, COLORREF rgb) {

	initgraph(width, height);//打开一个600*600
	setbkcolor(RGB(175, 238, 238));	//添加一个登陆的背景
	cleardevice();//刷新界面

	return;
}


void handleUserOption(int option) {
	switch (option)
	{
	case 1:
		playGame();
		break;
	case 2:

		break;
	case 3:

		break;
	default:

		break;
	}
}
