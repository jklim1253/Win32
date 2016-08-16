#include "winmain.h"
#include "ImageButton.h"

ImageButton Add, MinMax, Close;
BOOL bFullScreen = FALSE;
BOOL bGrabWindow = FALSE;
POINT ptMouse;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_DESTROY) {
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_CREATE) {
		SIZE cBlock = { 100,100 };
		RECT rcButton = { 0,0,50,50 };
		Add.Create(hWnd, rcButton, ID_PLUS, _T("add.bmp"), cBlock);
		::OffsetRect(&rcButton, 50, 0);
		MinMax.Create(hWnd, rcButton, ID_MAXMIN, _T("max.bmp"), cBlock);

		RECT rc;
		::GetClientRect(hWnd, &rc);

		rcButton = { rc.right - 50, 0, rc.right, 50 };
		Close.Create(hWnd, rcButton, ID_CLOSE, _T("close.bmp"), cBlock);

		return 0;
	}
	else if (uMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::SetBkMode(hdc, TRANSPARENT);
		::DrawText(hdc, _T("If want to move window, grab any point and then drag."), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		::EndPaint(hWnd, &ps);
	}
	else if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_ESCAPE) {
			::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
			return 0;
		}
	}
	else if (uMsg == WM_COMMAND) {
		UINT uId = LOWORD(wParam);
		if (uId == ID_PLUS) {
			::MessageBox(NULL, _T("Add"), _T("Info"), MB_OK);
		}
		else if (uId == ID_MAXMIN) {
			if (bFullScreen) {
				::SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				RECT rc;
				::GetClientRect(hWnd, &rc);
				::MoveWindow(Close.GetHandle(), rc.right - 50, 0, 50, 50, TRUE);
			}
			else {
				::SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				RECT rc;
				::GetClientRect(hWnd, &rc);
				::MoveWindow(Close.GetHandle(), rc.right - 50, 0, 50, 50, TRUE);
			}
			bFullScreen = !bFullScreen;
		}
		else if (uId == ID_CLOSE) {
			::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		}
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		::SetCapture(hWnd);

		::GetCursorPos(&ptMouse);

		bGrabWindow = TRUE;
	}
	else if (uMsg == WM_LBUTTONUP) {

		::ReleaseCapture();

		bGrabWindow = FALSE;
	}
	else if (uMsg == WM_MOUSEMOVE) {
		if (bFullScreen) return 0;

		if (!bGrabWindow) return 0;

		POINT ptOld = ptMouse;

		::GetCursorPos(&ptMouse);

		POINT ptDiff = { ptMouse.x - ptOld.x, ptMouse.y - ptOld.y };

		RECT rcWin;
		::GetWindowRect(hWnd, &rcWin);
		::MoveWindow(hWnd, rcWin.left + ptDiff.x, rcWin.top + ptDiff.y, rcWin.right - rcWin.left, rcWin.bottom - rcWin.top, TRUE);
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

