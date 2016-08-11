#include "About.h"

AboutWindow::AboutWindow() {

}

INT AboutWindow::Initialize() {
	INT cx = 400, cy = 400;
	INT x = (::GetSystemMetrics(SM_CXSCREEN) - cx) / 2;
	INT y = (::GetSystemMetrics(SM_CYSCREEN) - cy) / 2;
	RECT rc = { x, y, x + cx, y + cy };
	if (!Window::Register(_T("AboutWindowClass"))) {
		return 1;
	}

	setEventHandler();

	if (!Window::Create(NULL, 0, WS_POPUP | WS_SYSMENU | WS_CAPTION,
		rc, _T("AboutWindowClass"), _T("About..."))) {
		return 2;
	}

	::ShowWindow(hMainWnd, SW_NORMAL);
	::UpdateWindow(hMainWnd);

	return 0;
}
void AboutWindow::setEventHandler() {
	AddEventHandler(WM_CLOSE, (Window::Reaction)&AboutWindow::OnClose);
	AddEventHandler(WM_PAINT, (Window::Reaction)&AboutWindow::OnPaint);
}
LRESULT AboutWindow::OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Show(SW_HIDE);
	return 0;
}
LRESULT AboutWindow::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hWnd, &ps);

	RECT rc;
	::GetClientRect(hWnd, &rc);
	::SetBkMode(hdc, TRANSPARENT);
	::DrawText(hdc, _T("About This...Hello"), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	::EndPaint(hWnd, &ps);
	//return ::DefWindowProc(hWnd, WM_PAINT, wParam, lParam);
	return 0;
}