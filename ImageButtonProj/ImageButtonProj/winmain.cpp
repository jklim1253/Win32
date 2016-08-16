#include "winmain.h"

INT WINAPI
_tWinMain(HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPTSTR lpCmdLine,
	INT nShowCmd) {

	LPCTSTR szClassName = _T("ImageButtonProj");
	LPCTSTR szWindowName = _T("ImageButton Tester");

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hInstance = hInst;
	wcex.lpfnWndProc = &WndProc;
	wcex.lpszClassName = szClassName;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	if (!::RegisterClassEx(&wcex)) {
		DWORD dwError = ::GetLastError();
		return 1;
	}

	DWORD dwStyle = WS_POPUP | WS_VISIBLE;
	INT cx = 800;
	INT cy = 600;
	INT x = (::GetSystemMetrics(SM_CXSCREEN) - cx) / 2;
	INT y = (::GetSystemMetrics(SM_CYSCREEN) - cy) / 2;

	HWND hWnd = ::CreateWindowEx(0, szClassName, szWindowName, dwStyle, x, y, cx, cy,
		NULL, NULL, hInst, NULL);
	if (hWnd == NULL) {
		DWORD dwError = ::GetLastError();
		return 2;
	}

	::ShowWindow(hWnd, SW_NORMAL);
	::UpdateWindow(hWnd);

	MSG msg;

	while (true) {
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			break;
		}
	}

	return msg.wParam;
}