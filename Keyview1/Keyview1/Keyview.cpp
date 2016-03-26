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
	static TCHAR szAppName[] = TEXT("KeyView1");
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
		TEXT("KeyBoard Message View #1"),//window caption
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,//window style
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


	static int  cxChar, cxCaps, cyChar, cxClient, cyClient, cxClientMax,cyClientMax;
	static int cLinesMax, cLines;
	static PMSG pmsg;
	static RECT rectScroll;
	static TCHAR szTop[] = TEXT("Message       Key Char")TEXT("            Repeat Scan Ext ALT Prev Tran");
	static TCHAR szUnd[] = TEXT("_______       ___ ____")TEXT("            ______ ____ ___ ___ ____  ___");
	static TCHAR* szFormat[2] = { TEXT("%-13s %3d %-15s%c %6u %4d %3s %3s %4s %4s"), TEXT("%-13s 0x%04X%1s%c%6u %4d %3s %3s %4s %4s") };
	static TCHAR* szYes = TEXT("Yes");
	static TCHAR* szNo = TEXT("No");
	static TCHAR* szDown = TEXT("Down");
	static TCHAR* szUp = TEXT("Up");

	static TCHAR * szMessage[] = {
		TEXT("WM_KEYDOWN"), TEXT("WM_KEYUP"),
		TEXT("WM_CHAR"), TEXT("WM_DEADCHAR"),
		TEXT("WM_SYSKEYDOWN"), TEXT("WM_SYSKEYUP"),
		TEXT("WM_SYSCHAR"), TEXT("WM_SYSDEADCHAR") };
	HDC hdc;
	int i, iType;
	PAINTSTRUCT ps;
	TCHAR szBuffer[128], szKeyName[32];
	TEXTMETRIC tm;
	switch (message)
	{
	case WM_CREATE:
	case WM_DISPLAYCHANGE:
		// Get maximum size of client area
		cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
		cyClientMax = GetSystemMetrics(SM_CYMAXIMIZED);

		// Get character size for fixed-pitch font
		hdc = GetDC(hwnd);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;

		ReleaseDC(hwnd,hdc);
		// Allocate memory for display lines
		if (pmsg)
		{
			free(pmsg);
		}
		cLinesMax = cyClientMax / cyChar;
		pmsg = (PMSG)malloc(cLinesMax * sizeof(MSG));
		cLines = 0;
			// fall through
	case WM_SIZE:
		if (message==WM_SIZE)
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		}
			// Calculate scrolling rectangle
		rectScroll.left = 0;
		rectScroll.right = cxClient;
		rectScroll.top = cyChar;
		rectScroll.bottom = cyChar * (cyClient / cyChar);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		for (i = cLinesMax - 1; i > 0; --i)
		{
			pmsg[i] = pmsg[i - 1];
		}
			// Store new message
		pmsg[0].hwnd = hwnd;
		pmsg[0].message = message;
		pmsg[0].wParam = wParam;
		pmsg[0].lParam = lParam;
		cLines = min(cLines + 1, cLinesMax);
		// Scroll up the display
		ScrollWindow(hwnd, 0, -cyChar, &rectScroll, &rectScroll);
		break; // i.e., call DefWindowProc so Sys messages work
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, szTop, lstrlen(szTop));
		TextOut(hdc, 0, 0, szUnd, lstrlen(szUnd));

		for (i = 0; i < min(cLines, cyClient / cyChar - 1); i++)
		{
			iType = pmsg[i].message == WM_CHAR ||
				pmsg[i].message == WM_SYSCHAR ||
				pmsg[i].message == WM_DEADCHAR ||
				pmsg[i].message == WM_SYSDEADCHAR;
			GetKeyNameText(pmsg[i].lParam, szKeyName,
				sizeof(szKeyName) / sizeof(TCHAR));
			TextOut(hdc, 0, (cyClient / cyChar - 1 - i) * cyChar, szBuffer,
				wsprintf(szBuffer, szFormat[iType],
				szMessage[pmsg[i].message - WM_KEYFIRST],
				pmsg[i].wParam,
				(PTSTR)(iType ? TEXT(" ") : szKeyName),
				(TCHAR)(iType ? pmsg[i].wParam : ' '),
				LOWORD(pmsg[i].lParam),
				HIWORD(pmsg[i].lParam) & 0xFF,
				0x01000000 & pmsg[i].lParam ? szYes : szNo,
				0x20000000 & pmsg[i].lParam ? szYes : szNo,
				0x40000000 & pmsg[i].lParam ? szDown : szUp,
				0x80000000 & pmsg[i].lParam ? szUp : szDown));
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	//视窗讯息处理程式不予处理的所有讯息提供内定处理
	return DefWindowProc(hwnd, message, wParam, lParam);
}
