#include "Window.h"

Window::Window() 
	: hMainWnd(NULL) {

}
Window::~Window() {

}

LPCTSTR Window::Register(LPCTSTR szClassName) {
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	wcex.lpfnWndProc = &Window::WndProc;
	wcex.lpszClassName = szClassName;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	if (!::RegisterClassEx(&wcex)) {
		DWORD dwError = ::GetLastError();
		return NULL;
	}

	return szClassName;
}
HWND Window::Create(HWND hParentWnd, DWORD dwExStyle, DWORD dwStyle, RECT& rc, LPCTSTR szClassName, LPCTSTR szTitle) {
	hMainWnd = ::CreateWindowEx(dwExStyle, szClassName, szTitle, dwStyle,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		hParentWnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), this);
	if (hMainWnd == NULL) {
		DWORD dwError = ::GetLastError();
		return NULL;
	}

	return hMainWnd;
}

HWND Window::GetHwnd() const {
	return hMainWnd;
}
void Window::Show(int nCmdShow/*=SW_NORMAL*/) {
	::ShowWindow(hMainWnd, nCmdShow);
}

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	EventIter it = EventMap.find(uMsg);
	if (it != EventMap.end()) {
		return (this->*(it->second))(hWnd, wParam, lParam);
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::EventMapType& Window::getEventMap() {
	return EventMap;
}
void Window::AddEventHandler(UINT uMsg, const Reaction& reaction) {
	this->getEventMap().insert(std::make_pair(uMsg, reaction));
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Window* pThis = reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (pThis) {
		return pThis->WindowProc(hWnd, uMsg, wParam, lParam);
	}
	else {
		if (uMsg == WM_NCCREATE) {
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);

			pThis = reinterpret_cast<Window*>(lpcs->lpCreateParams);

			::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToLong(pThis));

			return pThis->WindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

