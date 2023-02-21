#pragma once

typedef struct SPoint
{
	float x, y;
} TPoint;

typedef struct SLine
{
	TPoint a, b;
} TLine;

void WinShow(HDC hdc, RECT rc, TLine* lineArray, int arrayLen);
int array_len_now(int n);
void array_edit(TLine* firstArray, TLine* secondArray, int renderDistance);
void create_line(HDC hdc, TLine line);
TPoint mouse_click_pos(HWND hwnd);
TLine line_init(TPoint a, TPoint b);
TPoint point_init(float x, float y);
void array_show(HDC hdc, TLine* lineArray, int arrayLen);
void line_print(TLine line);
void array_print(TLine* array);
void array_show(HDC hdc, TLine* lineArray, int arrayLen);
int render_distance_control(HWND hwnd, int renderDistance);