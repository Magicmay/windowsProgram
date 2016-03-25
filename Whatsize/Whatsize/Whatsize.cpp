#include <windows.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//HINSTANCE ִ��ʵ�壨������������
	//HWND�Ӵ�����
	//HDCװ�ô���
	//MSG ѶϢ�ṹ
	//WNDCLASS �Ӵ����ṹ
	//PAINTSTRUCT ���νṹ
	static TCHAR szAppName[] = TEXT("WhatSize");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("this prigram requires Windows 7!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName,//window class name
		TEXT("What Size is the Window"),//window caption
		WS_OVERLAPPEDWINDOW,//window style
		CW_USEDEFAULT,//initial x position
		CW_USEDEFAULT,//initial y position
		CW_USEDEFAULT,//initial x size
		CW_USEDEFAULT,//initial y size
		NULL,//parent window handle
		NULL,//window menu handle
		hInstance,//program instance handle
		NULL//creation parameters
		);
	ShowWindow(hwnd, iCmdShow);//��ʾ
	UpdateWindow(hwnd);//�ػ�
	while (GetMessage(&msg, NULL, 0, 0))//ѶϢ��Ȧ
	{
		TranslateMessage(&msg);//����ת��
		DispatchMessage(&msg);//����WndProc,������󴫸�getMessage,ѭ����
	}
	return msg.wParam;
}
void Show(HWND hwnd, HDC hdc, int xText, int yText, int iMapMode, TCHAR* szMapMode)
{
	TCHAR szBuffer[60];
	RECT rect;

	SaveDC(hdc);//����װ������
	SetMapMode(hdc, iMapMode);//�趨�µ�ӳ�䷽ʽ
	GetClientRect(hwnd, &rect);//ȡ����ʾ����
	DPtoLP(hdc, (PPOINT)&rect, 2);//��ʾ��������ת��Ϊ�߼�����
	RestoreDC(hdc, -1);//�ָ�ԭӳ�䷽ʽ
	TextOut(hdc, xText, yText, szBuffer, wsprintf(szBuffer, TEXT("%-20s%7d%7d%7d%7d"), szMapMode, rect.left, rect.right, rect.top
		, rect.bottom));
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)//�Ӵ�ѶϢ�����ʽ
{
	//CS_�Ӵ������ʽ
	//CW_�����Ӵ�
	//DT_��������
	//IDI_ͼʾID
	//IDC_�α�ID
	//MB_ѶϢ����
	//SND_����
	//WM_�Ӵ�ѶϢ
	//WS_�Ӵ���ʽ
	static TCHAR szHeading[] = TEXT("Mapping Mode Left Right Top Bottom");
	static TCHAR szUndLine[] = TEXT("------- ---- ---- ----- --- ------");
	static int cxChar, cyChar;
	HDC hdc;
	int i;
	PAINTSTRUCT ps;
	//	RECT rect;
	TEXTMETRIC tm;
	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 1, 1, NULL);
		SetViewportExtEx(hdc, cxChar, cyChar, NULL);

		TextOut(hdc, 1, 1, szHeading, lstrlen(szHeading));
		TextOut(hdc, 1, 2, szUndLine, lstrlen(szUndLine));

		Show(hwnd, hdc, 1, 3, MM_TEXT, TEXT("TEXT(pixels)"));
		Show(hwnd, hdc, 1, 4, MM_LOMETRIC, TEXT("LOMETRIC(.lmm)"));
		Show(hwnd, hdc, 1, 5, MM_HIMETRIC, TEXT("HIMETRIC(.01mm)"));

		Show(hwnd, hdc, 1, 6, MM_LOENGLISH, TEXT("LOENGLISH(.01 in)"));
		Show(hwnd, hdc, 1, 7, MM_HIENGLISH, TEXT("HIENGLISH(.001 in)"));
		Show(hwnd, hdc, 1, 8, MM_TWIPS, TEXT("TWIPS(1/1440 in)"));

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	//�Ӵ�ѶϢ�����ʽ���账�������ѶϢ�ṩ�ڶ�����
	return DefWindowProc(hwnd, message, wParam, lParam);
}