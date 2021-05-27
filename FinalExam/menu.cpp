#include<graphics.h>
#include<stdlib.h>
#include<stdio.h>
//�����û��Ľ���
int handleMouseEvent(int left, int top, int right, int bottom, int space, COLORREF rgb, COLORREF textRgb, int top2, int bottom2, int top3, int bottom3);

int openMenu(int left, int top, int right, int bottom, int space, COLORREF rgb, COLORREF textRgb) {
	//��һ��ͼ�����
	setfillcolor(rgb);
	fillrectangle(left, top, right, bottom);

	//д��
	settextcolor(textRgb);
	settextstyle(40, 0, "Arial", 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH);
	setbkmode(TRANSPARENT);
	outtextxy(left + 25, top + 5, "��ʼ��Ϸ");

	//�ڶ�����ť
	int top2 = top + space + bottom - top;
	int bottom2 = bottom + space + bottom - top;
	fillrectangle(left, top2, right, bottom2);
	outtextxy(left + 25, top2 + 5, "��ʷ��¼");

	//��������ť
	int top3 = top + 2 * (space + bottom - top);
	int bottom3 = bottom + 2 * (space + bottom - top);
	fillrectangle(left, top3, right, bottom3);
	outtextxy(left + 25, top3 + 5, "�˳���Ϸ");

	return handleMouseEvent(left, top, right, bottom, space, rgb, textRgb, top2, bottom2, top3, bottom3);
}

int handleMouseEvent(int left, int top, int right, int bottom, int space, COLORREF rgb, COLORREF textRgb,int top2,int bottom2,int top3 , int bottom3) {
	//����¼�
	MOUSEMSG m;
	int eventResult = 0;
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
			settextstyle(40, 0, "Arial", 0, 0, 400, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH);
			rectangle(left - 3, top - 3, right + 3, bottom + 3);
			if (m.uMsg == WM_LBUTTONDOWN)//�����������
			{
				settextcolor(RED);
				outtextxy(left + 25, top + 5, "��ʼ��Ϸ");
			}
			else if (m.uMsg == WM_LBUTTONUP)//�������ͷ�
			{
				settextcolor(textRgb);
				outtextxy(left + 25, top + 5, "��ʼ��Ϸ");
				eventResult = 1;
				break;
			}
		}

		else if (flagX && flagY_2)
		{
			setlinecolor(RED);
			rectangle(left - 3, top2 - 3, right + 3, bottom2 + 3);
			if (m.uMsg == WM_LBUTTONDOWN)//�����������
			{
				settextcolor(RED);
				outtextxy(left + 25, top2 + 5, "��ʷ��¼");
			}
			else if (m.uMsg == WM_LBUTTONUP)//�������ͷ�
			{
				settextcolor(textRgb);
				outtextxy(left + 25, top2 + 5, "��ʷ��¼");
				eventResult = 2;
				break;
			}
		}
		else if (flagX && flagY_3)
		{
			setlinecolor(RED);
			rectangle(left - 3, top3 - 3, right + 3, bottom3 + 3);
			if (m.uMsg == WM_LBUTTONDOWN)//�����������
			{
				settextcolor(RED);
				outtextxy(left + 25, top3 + 5, "�˳���Ϸ");
			}
			else if (m.uMsg == WM_LBUTTONUP)//�������ͷ�
			{
				settextcolor(textRgb);
				outtextxy(left + 25, top3 + 5, "�˳���Ϸ");
				eventResult = 3;
				break;
			}
		}

		else
		{
			setlinecolor(WHITE);
			rectangle(left - 3, top - 3, right + 3, bottom + 3);
			rectangle(left - 3, top2 - 3, right + 3, bottom2 + 3);
			rectangle(left - 3, top3 - 3, right + 3, bottom3 + 3);
			settextcolor(textRgb);
			outtextxy(left + 25, top + 5, "��ʼ��Ϸ");
			outtextxy(left + 25, top2 + 5, "��ʷ��¼");
			outtextxy(left + 25, top3 + 5, "�˳���Ϸ");
		}
	}
	return eventResult;
}
