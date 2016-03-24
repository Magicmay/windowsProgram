#include "Devcaps1.h"
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
	static TCHAR szAppName[] = TEXT("DevCaps1");
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
		TEXT("Device Capabilities"),//window caption
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
	static int cxChar, cxCaps, cyChar;
	TCHAR szBuffer[10];
	HDC hdc;
	int i;
	PAINTSTRUCT ps;
//	RECT rect;
	TEXTMETRIC tm;
	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0; i < NUMLINES;++i)
		{
			TextOut(hdc, 0, cyChar*i, devcaps[i].szLabel, lstrlen(devcaps[i].szLabel));
			TextOut(hdc, 14 * cxCaps, cyChar*i, devcaps[i].szDesc, lstrlen(devcaps[i].szDesc));
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			TextOut(hdc, 14 * cxCaps + 35 * cxChar, cyChar*i, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetDeviceCaps(hdc, devcaps[i].iIndex)));
			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	//�Ӵ�ѶϢ�����ʽ���账�������ѶϢ�ṩ�ڶ�����
	return DefWindowProc(hwnd, message, wParam, lParam);
}