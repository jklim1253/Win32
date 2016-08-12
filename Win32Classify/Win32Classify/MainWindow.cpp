#include "MainWindow.h"
#include "About.h"

MainWindow::MainWindow() {

}

INT MainWindow::Initialize() {
	INT cx = 800, cy = 600;
	INT x = (::GetSystemMetrics(SM_CXSCREEN) - cx) / 2;
	INT y = (::GetSystemMetrics(SM_CYSCREEN) - cy) / 2;
	RECT rc = { x, y, x + cx, y + cy };
	if (!Window::Register(_T("WindowTest"))) {
		return 1;
	}

	setEventHandler();

	if (!Window::Create(NULL, 0, WS_POPUP | WS_VISIBLE, rc, _T("WindowTest"), _T("Window Test"))) {
		return 2;
	}

	::ShowWindow(hMainWnd, SW_NORMAL);
	::UpdateWindow(hMainWnd);

	return 0;
}
INT MainWindow::Run() {
	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return (INT)msg.wParam;
}
void MainWindow::setEventHandler() {
	AddEventHandler(WM_LBUTTONDOWN, (Window::Reaction)&MainWindow::OnLButtonDown);
	AddEventHandler(WM_LBUTTONUP, (Window::Reaction)&MainWindow::OnLButtonUp);
	AddEventHandler(WM_MOUSEMOVE, (Window::Reaction)&MainWindow::OnMouseMove);
	AddEventHandler(WM_CREATE, (Window::Reaction)&MainWindow::OnCreate);
	AddEventHandler(WM_DESTROY, (Window::Reaction)&MainWindow::OnDestroy);
	AddEventHandler(WM_PAINT, (Window::Reaction)&MainWindow::OnPaint);
	AddEventHandler(WM_KEYDOWN, (Window::Reaction)&MainWindow::OnKeyDown);
}
LRESULT MainWindow::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	return 0;
}
LRESULT MainWindow::OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	::PostQuitMessage(0);
	return 0;
}
LRESULT MainWindow::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hWnd, &ps);

	RECT rc;
	::GetClientRect(hWnd, &rc);
	::SetBkMode(hdc, TRANSPARENT);
	::DrawText(hdc, _T("ESC : Quit Program, F1 : About"), -1, &rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	::EndPaint(hWnd, &ps);
	return 0;
}
LRESULT MainWindow::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if (wParam == VK_ESCAPE) {
		::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
	}
	else if (wParam == VK_F1) {
		//::MessageBox(NULL, _T("Help!"), _T("Help"), MB_OK);

		if (winAbout.GetHwnd() == NULL) {
			winAbout.Initialize();
		}
		else {
			winAbout.Show();
		}
	}

	return 0;
}
LRESULT MainWindow::OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	bGripWindow = TRUE;
	::SetCapture(hWnd);
	::GetCursorPos(&ptMouse);

	return 0;
}
LRESULT MainWindow::OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	bGripWindow = FALSE;
	::ReleaseCapture();
	return 0;
}
LRESULT MainWindow::OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if (!bGripWindow) return 0;

	POINT ptOld = ptMouse;
	::GetCursorPos(&ptMouse);
	POINT ptDiff = { ptMouse.x - ptOld.x, ptMouse.y - ptOld.y };

	RECT rc;
	::GetWindowRect(hWnd, &rc);
	::MoveWindow(hWnd, rc.left + ptDiff.x, rc.top + ptDiff.y,
		rc.right - rc.left, rc.bottom - rc.top, TRUE);

	return 0;
}