#include "Grippable.h"

Grippable::Grippable()
	: bGripWindow(FALSE) {
}

LRESULT Grippable::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_LBUTTONDOWN :
	{
		bGripWindow = TRUE;
		::SetCapture(hWnd);
		::GetCursorPos(&ptMouse);
		break;
	}
	case WM_LBUTTONUP :
	{
		bGripWindow = FALSE;
		::ReleaseCapture();
		break;
	}
	case WM_MOUSEMOVE :
	{
		if (!bGripWindow) break;

		POINT ptOld = ptMouse;
		::GetCursorPos(&ptMouse);
		POINT ptDiff = { ptMouse.x - ptOld.x, ptMouse.y - ptOld.y };

		RECT rc;
		::GetWindowRect(hWnd, &rc);
		::MoveWindow(hWnd, rc.left + ptDiff.x, rc.top + ptDiff.y,
			rc.right - rc.left, rc.bottom - rc.top, TRUE);
	}

	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}
