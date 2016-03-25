#include <windows.h>
#pragma comment(lib,"winmm.lib")
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//HINSTANCE 执行实体（程序自身）代号
	//HWND视窗代号
	//HDC装置代号
	//MSG 讯息结构
	//WNDCLASS 视窗类别结构
	//PAINTSTRUCT 矩形结构
	static TCHAR szAppName[] = TEXT("AltWind");
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
		TEXT("Alternate and Winding Fill Modes"),//window caption
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
	ShowWindow(hwnd, iCmdShow);//显示
	UpdateWindow(hwnd);//重画
	while (GetMessage(&msg, NULL, 0, 0))//讯息回圈
	{
		TranslateMessage(&msg);//键盘转换
		DispatchMessage(&msg);//呼叫WndProc,处理完后传给getMessage,循环。
	}
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)//视窗讯息处理程式
{
	//CS_视窗类别样式
	//CW_建立视窗
	//DT_绘制文字
	//IDI_图示ID
	//IDC_游标ID
	//MB_讯息方块
	//SND_声音
	//WM_视窗讯息
	//WS_视窗样式
	//WHITE_BRUSH、LTGRAY_BRUSH、GRAY_BRUSH、DKGRAY_BRUSH、BLACK_BRUSH 和NULL_BRUSH
	HDC hdc;
	PAINTSTRUCT ps;
	//	RECT rect;
	static POINT aptFigure[10] = { 10, 70, 50,70, 50, 10, 90, 10, 90, 50, 30, 50, 30, 90, 70, 90, 70, 30,10, 30 };
	static int cxClient,cyClient;
	int i;
	POINT apt[10];
	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, GetStockObject(GRAY_BRUSH));
		for (i = 0; i < 10; ++i)
		{
			apt[i].x = cxClient*aptFigure[i].x / 200;
			apt[i].y = cyClient*aptFigure[i].y / 100;
		}
		SetPolyFillMode(hdc, ALTERNATE);
		Polygon(hdc, apt, 10);

		for (i = 0; i < 10;++i)
		{
			apt[i].x += cxClient / 2;
		}
		SetPolyFillMode(hdc, WINDING);
		Polygon(hdc, apt, 10);

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	//视窗讯息处理程式不予处理的所有讯息提供内定处理
	return DefWindowProc(hwnd, message, wParam, lParam);
}