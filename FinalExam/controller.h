#pragma once
void newAGraphics(int width ,  int height , COLORREF rgb); //新建一个可选菜单
void initMenu(int left, int top, int right, int bottom, int space, COLORREF rgb, COLORREF textRgb); //初始化菜单界面
void handleMouseEvent(int left ,int right ,int top1,int bottom1,int top2,int bottom2,int top3,int bottom3); //处理Menu的鼠标移动，点击事件