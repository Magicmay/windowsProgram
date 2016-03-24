#include <windows.h>
#include "Sysmets.h"
#pragma comment(lib,"winmm.lib")
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//HINSTANCE ִ��ʵ�壨������������
	//HWND�Ӵ�����
	//HDCװ�ô���
	//MSG ѶϢ�ṹ
	//WNDCLASS �Ӵ����ṹ
	//PAINTSTRUCT ���νṹ
	static TCHAR szAppName[] = TEXT("SysMets2");
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
		TEXT("Get System Metrics No.2"),//window caption
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,//window style
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
	HDC hdc;
	PAINTSTRUCT ps;
	//	RECT rect;

	static int cxChar, cxCaps, cyChar,cyClient,iVscrollPos,iVscrollMax;
	int i,y;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;
	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
	//	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar/2;150%��
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar/2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
		SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
		return 0;
	case WM_SIZE:
	/*	iVscrollMax = max(0, NUMLINES - cyClient / cyChar);
		SetScrollRange(hwnd, SB_VERT, 0, iVscrollMax, true);*/
		cyClient = HIWORD(lParam);
		return 0;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iVscrollPos -= 1;
			break;;
		case SB_LINEDOWN:
			iVscrollPos += 1;
			break;
		case SB_PAGEUP:
			iVscrollPos -= cyClient / cyChar;
			break;
		case SB_PAGEDOWN:
			iVscrollPos += cyClient / cyChar;
			break;
		case SB_THUMBPOSITION:
			iVscrollPos = HIWORD(wParam);
			break;
		default:
			break;
		}
		iVscrollPos = max(0, min(iVscrollPos, NUMLINES - 1));
		if (iVscrollPos!=GetScrollPos(hwnd,SB_VERT))
		{
			SetScrollPos(hwnd, SB_VERT,iVscrollPos, TRUE);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0; i < NUMLINES; i++)
		{
			y = cyChar*(i - iVscrollPos);
			TextOut(hdc, 0, y, sysmetrics[i].szLabel,
				lstrlen(sysmetrics[i].szLabel));

			TextOut(hdc, 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			int ilenth = wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].Index));
			TextOut(hdc, 22 * cxCaps + 40 * cxChar, y, szBuffer, ilenth);
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