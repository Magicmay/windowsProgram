#include <windows.h>
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//HINSTANCE ִ��ʵ�壨������������
	//HWND�Ӵ�����
	//HDCװ�ô���
	//MSG ѶϢ�ṹ
	//WNDCLASS �Ӵ����ṹ
	//PAINTSTRUCT ���νṹ
	static TCHAR szAppName[] = TEXT("Bezier");
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
		TEXT("Beier Splines"),//window caption
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
void Drawbezier(HDC hdc, POINT apt[])
{
	PolyBezier(hdc, apt, 4);
	MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
	LineTo(hdc, apt[1].x, apt[1].y);
	MoveToEx(hdc, apt[2].x, apt[2].y, NULL);
	LineTo(hdc, apt[3].x, apt[3].y);
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
	static POINT apt[4];
	int cxClient, cyClient;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		apt[0].x = cxClient / 4;
		apt[0].y = cyClient / 2;
		apt[1].x = cxClient / 2;
		apt[1].y = cyClient / 4;
		apt[2].x = cxClient / 2;
		apt[2].y = 3 * cyClient / 4;
		apt[3].x = 3 * cxClient / 4;
		apt[3].y = cyClient / 4;
		return 0;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if (wParam&MK_LBUTTON||wParam&MK_RBUTTON)
		{
			hdc = GetDC(hwnd);
			SelectObject(hdc, GetStockObject(WHITE_PEN));
			Drawbezier(hdc, apt);
			if (wParam&MK_LBUTTON)
			{
				apt[1].x = LOWORD(lParam);
				apt[1].y = HIWORD(lParam);
			}
			if (wParam&MK_RBUTTON)
			{
				apt[2].x = LOWORD(lParam);
				apt[2].y = HIWORD(lParam);
			}
			SelectObject(hdc, GetStockObject(BLACK_PEN));
			Drawbezier(hdc, apt);
			ReleaseDC(hwnd, hdc);
		}
		return 0;
	case WM_PAINT:
		InvalidateRect(hwnd, NULL,true);
		hdc = BeginPaint(hwnd, &ps);
		Drawbezier(hdc, apt);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	//�Ӵ�ѶϢ�����ʽ���账�������ѶϢ�ṩ�ڶ�����
	return DefWindowProc(hwnd, message, wParam, lParam);
}