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
LRESULT Grippable::OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	bGripWindow = TRUE;
	::SetCapture(hWnd);
	::GetCursorPos(&ptMouse);

	return ::DefWindowProc(hWnd, WM_LBUTTONDOWN, wParam, lParam);
}
LRESULT Grippable::OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	bGripWindow = FALSE;
	::ReleaseCapture();
	return ::DefWindowProc(hWnd, WM_LBUTTONUP, wParam, lParam);
}
LRESULT Grippable::OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if (!bGripWindow) return ::DefWindowProc(hWnd, WM_MOUSEMOVE, wParam, lParam);

	POINT ptOld = ptMouse;
	::GetCursorPos(&ptMouse);
	POINT ptDiff = { ptMouse.x - ptOld.x, ptMouse.y - ptOld.y };

	RECT rc;
	::GetWindowRect(hWnd, &rc);
	::MoveWindow(hWnd, rc.left + ptDiff.x, rc.top + ptDiff.y,
		rc.right - rc.left, rc.bottom - rc.top, TRUE);

	return ::DefWindowProc(hWnd, WM_MOUSEMOVE, wParam, lParam);
}
