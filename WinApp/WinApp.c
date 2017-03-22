// WinApp.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <tchar.h>

#define WINDOW_CLASS_NAME  (L"My App")

int counter;

/* write current time to file*/
void WriteTimeToFile();

LRESULT CALLBACK  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND  hwinmineWnd = NULL;

	/* check for events */
	switch (msg)
	{
		/* check for right mouse button click, updates title with number of clicks*/
		case WM_RBUTTONDOWN:
			counter++;
			if (counter >= 10000)/*added to stop from overflow*/
				counter = 1;
			break;

		/* check for left button click to update title*/
		case  WM_LBUTTONDOWN:
			TCHAR buf[5];
			_stprintf(buf, _T("%d"), counter);
			SetWindowText(hWnd, (LPSTR)buf);
			break;

		/* check for resize and show message box, blocks resizing */
		case WM_SIZING:
			MessageBox(NULL, L"רק שימי נר על החלון", L"נר", MB_OK | MB_ICONWARNING);
			break;
		/* check for key press */
		case WM_KEYDOWN:
			/* check for enter click, writes times into file */
			if (wParam == VK_RETURN)
			{
				WriteTimeToFile();
			}
			break;

			/*	Window is closing*/
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
				break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCMDLine, int nCmdshow)
{
	WNDCLASS wc = { 0 };
	HWND hWnd = NULL;
	MSG msg = { 0 };
	counter = 0;

	/* Fill window class stuct */
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.hInstance = hInstance;

	/* register class + check if function failed */
	if (FALSE == RegisterClass(&wc))
	{
		MessageBox(NULL, L"Window Class Registaration Failed!", L"ERROR", MB_OK | MB_ICONWARNING);
		return -1;
	}

	/* Create window */
	hWnd = CreateWindow(WINDOW_CLASS_NAME, L"Title", WS_OVERLAPPEDWINDOW, 0, 0, 200, 200, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"ERROR", MB_OK | MB_ICONWARNING);
		return -1;
	}

	/* Show Window */
	ShowWindow(hWnd, nCmdshow);

	/* Message Loop */
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	/* When GetMessage get quit, wParam will hold return code */
	return msg.wParam;
}

void WriteTimeToFile()
{				
	/* open/create file */
	HANDLE hFile = CreateFile(
	    L"time.txt",
		FILE_APPEND_DATA,
		0,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL);

	/*get time*/
	DWORD nBytesWrite;
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	TCHAR buf[20];
	_stprintf(buf, _T("%d:%d:%d\r\n "), lt.wHour, lt.wMinute, lt.wSecond);

	/* write time to file */
	BOOL success = WriteFile(hFile, buf, 20 * sizeof(char), &nBytesWrite, NULL);
	CloseHandle(hFile);
}
