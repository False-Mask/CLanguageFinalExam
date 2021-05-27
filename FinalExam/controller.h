#pragma once
void newAGraphics(int width ,  int height , COLORREF rgb); //新建一个可选菜单
int initMenu(int left, int top, int right, int bottom, int space, COLORREF rgb, COLORREF textRgb); //初始化菜单界面
void handleUserOption(int option);