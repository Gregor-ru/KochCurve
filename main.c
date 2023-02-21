#include "framework.h"
#include "functions.h"

RECT rc;

LRESULT CALLBACK WndProc
(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (message) {
		// ќтрисовка окна
	case WM_SIZE:
	{
		GetClientRect(hWnd, &rc);
	}
	case WM_LBUTTONDOWN:
	{

	}
	case WM_PAINT:
	{
		// ѕолучение координат клиентской части окна
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rcClient;

		GetClientRect(hWnd, &rcClient);

		//  онец отрисовки, особождение контекста
		EndPaint(hWnd, &ps);
	}
	break;

	// ¬ыход из приложени€
	case WM_DESTROY:
		PostQuitMessage(0);

		// ќстальные сообщени€ окна обрабатываютс€ стандартным образом
	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return 0;
}

int main()
{
	WNDCLASSA wndclass;

	memset(&wndclass, 0, sizeof(WNDCLASSA));
	wndclass.lpszClassName = "Fractal";
	wndclass.lpfnWndProc = WndProc;

	RegisterClassA(&wndclass);

	HWND hwnd;

	hwnd = CreateWindowA
	(
		"Fractal",
		"Fractal",
		WS_OVERLAPPEDWINDOW,
		10,
		10,
		640,
		480,
		NULL,
		NULL,
		NULL,
		NULL
	);

	HDC hdc = GetDC(hwnd);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	MSG msg;
	TPoint startPos = point_init(0,0), PosNow = point_init(0, 0);
	TLine lineNow = line_init(startPos, startPos);

	TLine* firstArray;
	TLine* secondArray;
	int renderDistanceFirst = 0, renderDistanceSecond = 0;
	firstArray = (TLine*)malloc(array_len_now(0)*sizeof(TLine));
	secondArray = (TLine*)malloc(array_len_now(0)* sizeof(TLine));

	int flag = 0;

	while (GetMessageA(&msg, NULL, 0, 0))
	{
		TPoint PosNow = mouse_click_pos(hwnd);
		renderDistanceSecond = render_distance_control(hwnd, renderDistanceSecond);
		

		if (GetKeyState(VK_LBUTTON) > 0) {

			switch (flag)
			{
			case 0:
				startPos = PosNow;
			case 1:
				flag++;
			case 2:
				
				if (renderDistanceFirst != renderDistanceSecond)
				{
					TLine* oldBufferFirst;
					TLine* oldBufferSecond;

					renderDistanceFirst = renderDistanceSecond;

					oldBufferSecond = secondArray;

					if ((secondArray = (TLine*)realloc(firstArray, array_len_now(renderDistanceFirst + 1) * sizeof(TLine)))
						== NULL)
					{
						free(oldBufferSecond);
					}

					secondArray = (TLine*)realloc(secondArray, array_len_now(renderDistanceFirst + 1) * sizeof(TLine));

					array_edit(firstArray, secondArray, renderDistanceFirst);

					oldBufferFirst = firstArray;

					if ((firstArray = (TLine*)realloc(firstArray, array_len_now(renderDistanceFirst + 1) * sizeof(TLine)))
						== NULL)
					{free(oldBufferFirst);}

					firstArray = secondArray;
					
					WinShow(hdc, rc, firstArray, renderDistanceFirst);

				}
				else
				{
					WinShow(hdc, rc, firstArray, renderDistanceFirst);
				}

				flag = 0;
			default:
				break;
			}

		}
		else 
		{

			lineNow = line_init(startPos, PosNow);
			*firstArray = lineNow;
			array_print(firstArray);

			switch (flag)
			{
			case 0:
				flag++;
			case 1:
				WinShow(hdc, rc, firstArray, 0);
			case 2:
				WinShow(hdc, rc, firstArray, 0);
			default:
				break;
			}

		}
		printf("%d\t%d\n", flag, renderDistanceFirst);
		if (msg.message == WM_QUIT) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		Sleep(0);

	}

	return 0;
}