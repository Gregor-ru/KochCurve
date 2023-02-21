#pragma once

#include "framework.h"
#include "functions.h"

int array_len_now(int n)
{
	return pow(4, n);
}

TPoint point_init(float x, float y)
{
	TPoint pointNow;

	pointNow.x = x; pointNow.y = y;

	return pointNow;
}

TLine line_init(TPoint a, TPoint b)
{
	TLine lineNow;

	lineNow.a = a; lineNow.b = b;

	return lineNow;
}

TLine line_first(TLine line)
{
	TLine lineNow;

	lineNow = line_init
	(
		point_init(line.a.x, line.a.y),
		point_init((1.0 / 3) * (2.0 * line.a.x +  line.b.x), (1.0 / 3) * (2.0*line.a.y + line.b.y))
	);

	return lineNow;
}

TLine line_second(TLine line)
{
	TLine lineNow;

	lineNow = line_init
	(
		point_init((1.0 / 3) * (2.0 * line.a.x + line.b.x), (1.0 / 3) * (2.0 * line.a.y +  line.b.y)),
		point_init((1.0 / 6) * (3.0*line.a.x + 4.0 * line.b.x - sqrt(3.0)*(line.b.y - line.a.y)), (1.0 / 6) * (3.0 * line.a.y + 4.0 * line.b.y - sqrt(3.0) * (line.b.x - line.a.x)))
	);

	return lineNow;
}

TLine line_third(TLine line)
{
	TLine lineNow;

	lineNow = line_init
	(
		point_init((1.0 / 6) * (3.0 * line.a.x + 4.0 * line.b.x - sqrt(3.0) * (line.b.y - line.a.y)), (1.0 / 6) * (3.0 * line.a.y + 4.0 * line.b.y - sqrt(3.0) * (line.b.x - line.a.x))),
		point_init((1.0 / 3) * (line.a.x + 2.0 * line.b.x), (1.0 / 3) * (line.a.y + 2.0 * line.b.y))
	);

	return lineNow;
}

TLine line_fourth(TLine line)
{
	TLine lineNow;

	lineNow = line_init
	(
		point_init((1.0 / 3) * (line.a.x + 2.0 * line.b.x), (1.0 / 3) * (line.a.y + 2.0 * line.b.y)),
		point_init(line.b.x, line.b.y)
	);

	return lineNow;
}

void point_print(TPoint point)
{
	printf("(x: %f; y: %f)", point.x, point.y);
}

void line_print(TLine line)
{

	printf("%s", "(a: ");
	point_print(line.a);
	printf("%s", "( b: ");
	point_print(line.b);
	printf("%s\t", ")");
}

void array_print(TLine* array)
{
	for (int i = 0; i < sizeof(*array) / sizeof(array[0]); i++)
	{
		line_print(array[i]);
		printf("\n");
	}
}

int render_distance_control(HWND hwnd, int renderDistance)
{
	int renderDistanceNow = renderDistance;

	if (GetKeyState(VK_UP) > 0) renderDistanceNow++;
	if (GetKeyState(VK_DOWN) > 0 && renderDistanceNow>0) renderDistanceNow--;

	return renderDistanceNow;
}

void array_edit(TLine* firstArray, TLine* secondArray, int renderDistance)
{
	int arrayLen = array_len_now(renderDistance+1);

	for (int i = 0; i < arrayLen; i = i + 4)
	{
		*(secondArray + i) = line_first(*(firstArray + (i / 4)));
		*(secondArray + i + 1) = line_second(*(firstArray + (i / 4)));
		*(secondArray + i + 2) = line_third(*(firstArray + (i / 4)));
		*(secondArray + i + 3) = line_fourth(*(firstArray + (i / 4)));
	}

}

void create_line(HDC hdc, TLine line)
{
	HPEN hBlackPen = CreatePen(PS_SOLID, 1.5, RGB(0, 0, 0));

	// нарисовать линию
	SelectObject(hdc, hBlackPen);

	MoveToEx(hdc, line.a.x, line.a.y, NULL);

	LineTo(hdc, line.b.x, line.b.y);

	// Удаление кистей
	DeleteObject(hBlackPen);

}

void array_show(HDC hdc, TLine* lineArray, int arrayLen)
{

	HPEN hBlackPen = CreatePen(PS_SOLID, 1.5, RGB(0, 0, 0));

	// нарисовать линию
	SelectObject(hdc, hBlackPen);

	for (int i = 0; i < arrayLen; i++)
	{
		MoveToEx(hdc, lineArray[i].a.x, lineArray[i].a.y, NULL);
		LineTo(hdc, lineArray[i].b.x, lineArray[i].b.y);
	}

	// Удаление кистей
	DeleteObject(hBlackPen);

}

void WinShow( HDC hdc, RECT rc, TLine* lineArray, int renderDistance)
{
	HDC sqDc = CreateCompatibleDC(hdc);
	HBITMAP sqBm = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

	SelectObject(sqDc, sqBm);

	SelectObject(sqDc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(sqDc, RGB(255, 255, 255));
	Rectangle(sqDc, 0, 0, 640, 840);

	//create_line(sqDc, line);

	array_show(sqDc, lineArray, array_len_now(renderDistance));

	BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, sqDc, 0, 0, SRCCOPY);
	DeleteDC(sqDc);
	DeleteObject(sqBm);

}

TPoint mouse_click_pos(HWND hwnd) {

	LPPOINT pPnt;
	pPnt = malloc(sizeof(*pPnt));

	if (pPnt)GetCursorPos(pPnt);
	if (pPnt)ScreenToClient(hwnd, pPnt);

	TPoint PosNow = point_init(0,0);
	if(pPnt) PosNow = point_init(pPnt[0].x, pPnt[0].y);

	return PosNow;
}

/*
while (GetMessageA(&msg, NULL, 0, 0))
	{
		TPoint PosNow = mouse_click_pos(hwnd);

		if (GetKeyState(VK_LBUTTON) > 0) {

			startPos = PosNow;
			//printf("x: %f	y: %f\n", PosNow.x, PosNow.y);
			flag++;

		}
		else
		{

			lineNow = line_init(startPos, PosNow);
			int renderDistanceNow = 0;

			firstArray = (TLine*)calloc(array_len_now(renderDistanceNow), sizeof(TLine));
			secondArray = (TLine*)calloc(array_len_now(renderDistanceNow), sizeof(TLine));

			//line_print(lineNow);

			firstArray[0] = lineNow;
			array_print(firstArray);

			for (int i = 0; i < 4; i++)
			{
				array_edit(firstArray, secondArray, i);
			}
			flag = 0;
			//printf("x: %f	y: %f\n", PosNow.x, PosNow.y);
		}

		if (msg.message == WM_QUIT) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		WinShow(hdc, rc, firstArray, 1);
		Sleep(0);

	}
*/
