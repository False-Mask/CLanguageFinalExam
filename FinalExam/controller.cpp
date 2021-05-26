#include<graphics.h>

void initMenu(int left ,int top , int right , int bottom , int space, COLORREF rgb, COLORREF textRgb) {
	//��һ��ͼ�����
	setfillcolor(rgb);
	fillrectangle(left, top, right, bottom);

	//д��
	setbkcolor(textRgb);
	settextstyle(40, 0,"Arial", 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH);
	setbkmode(TRANSPARENT);
	outtextxy(left+25, top+5, "��ʼ��Ϸ");

	//�ڶ�����ť
	int top2 = top + space + bottom - top; 
	int bottom2 = bottom + space + bottom - top;
	fillrectangle(left, top2, right, bottom2);
	outtextxy(left+25, top + space + bottom - top + 5, "��ʷ��¼");

	//��������ť
	int top3 = top + 2 * (space + bottom - top);
	int bottom3 = bottom + 2 * (space + bottom - top);
	fillrectangle(left, top3, right,bottom3);
	outtextxy(left  + 25, top + 2 * (space + bottom - top)  + 5, "��ʷ��¼");

	//����¼�
	MOUSEMSG m;

	while (true)
	{
		m = GetMouseMsg();
		bool flagX = m.x >= left && m.x <= right;
		bool flagY_1 = m.y >= top && m.y <= bottom;
		bool flagY_2 = m.y >= top2 && m.y <= bottom2; 
		bool flagY_3 = m.y >= top3 && m.y <= bottom3;
		if (flagX && flagY_1)
		{
			setlinecolor(RED);
			rectangle(left-3, top-3, right+3, bottom+3);
		}
		else if (flagX && flagY_2)
		{
			setlinecolor(RED);
			rectangle(left - 3, top2 - 3, right + 3, bottom2 + 3);
		}
		else if (flagX && flagY_3)
		{
			setlinecolor(RED);
			rectangle(left - 3, top3 - 3, right + 3, bottom3 + 3);
		}
		else
		{
			setlinecolor(WHITE);
			rectangle(left - 3, top - 3, right + 3, bottom + 3);
			rectangle(left - 3, top2 - 3, right + 3, bottom2 + 3);
			rectangle(left - 3, top3 - 3, right + 3, bottom3 + 3);
		}
	}

	return;
}


void newAGraphics(int width, int height, COLORREF rgb) {

	initgraph(width, height);//��һ��600*600
	setbkcolor(RGB(175, 238, 238));	//���һ����½�ı���
	cleardevice();//ˢ�½���

	return;
}
