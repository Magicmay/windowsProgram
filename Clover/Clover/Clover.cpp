#include <windows.h>
#include <math.h>
#define TWO_PI (2.0*3.14159)
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
	static TCHAR szAppName[] = TEXT("Clover");
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
		TEXT("Draw a Clover"),//window caption
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
	static HRGN hRgnClip;
	static int cxClient,cyClient;
	double fAngle, fRadius;
	int i;
	HCURSOR hCursor;
	HRGN hRgnTemp[6];

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
		ShowCursor(TRUE);
		if (hRgnClip)
		{
			DeleteObject(hRgnClip);
		}
		hRgnTemp[0] = CreateEllipticRgn(0, cyClient / 3, cxClient / 2, 2 * cyClient / 3);
		hRgnTemp[1] = CreateEllipticRgn(cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3);
		hRgnTemp[2] = CreateEllipticRgn(cxClient / 3, 0,
			2 * cxClient / 3, cyClient / 2);
		hRgnTemp[3] = CreateEllipticRgn(cxClient / 3, cyClient / 2,
			2 * cxClient / 3, cyClient);
		hRgnTemp[4] = CreateRectRgn(0, 0, 1, 1);
		hRgnTemp[5] = CreateRectRgn(0, 0, 1, 1);
		hRgnClip = CreateRectRgn(0, 0, 1, 1);

		CombineRgn(hRgnTemp[4], hRgnTemp[0], hRgnTemp[1], RGN_OR);
		CombineRgn(hRgnTemp[5], hRgnTemp[2], hRgnTemp[3], RGN_OR);
		CombineRgn(hRgnClip, hRgnTemp[4], hRgnTemp[5], RGN_XOR);

		for (i = 0; i < 6; ++i)
		{
			DeleteObject(hRgnTemp[i]);
		}
		SetCursor(hCursor);
		ShowCursor(FALSE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
		SelectClipRgn(hdc, hRgnClip);
		fRadius = _hypot(cxClient / 2.0, cyClient / 2.0);
		for (fAngle = 0.0; fAngle < TWO_PI;fAngle +=TWO_PI/360)
		{
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, (int)(fRadius*cos(fAngle) + 0.5), (int)(-fRadius*sin(fAngle) + 0.5));
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(hRgnClip);
		PostQuitMessage(0);
		return 0;
	}
	//�Ӵ�ѶϢ�����ʽ���账�������ѶϢ�ṩ�ڶ�����
	return DefWindowProc(hwnd, message, wParam, lParam);
}