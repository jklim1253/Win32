#include "About.h"
#include <fstream>
#include <list>
#include <string>
#include <locale>
#include <codecvt>

#if defined(UNICODE) || defined(_UNICODE)
#define tfstream wfstream
#define tstring wstring
#else
#define tfstream fstream
#define tstring string
#endif

struct AboutImpl {
	bool bTextfileLoaded;
	std::list<std::tstring> contents;

	bool loadtextfile(const std::tstring& filename) {
		std::tfstream file;
		std::locale utf8_locale = std::locale(std::locale::empty(), new std::codecvt_utf8<TCHAR>());
		file.imbue(utf8_locale);
		file.open(filename, std::ios_base::in);
		if (!file.good()) {
			return false;
		}

		do {
			std::tstring line;
			std::getline(file, line);

			contents.push_back(line);

		} while (file.good());

		file.close();
		return true;
	}
	void drawcontents(HDC hdc, const RECT& rc) {
		TEXTMETRIC tm;
		::GetTextMetrics(hdc, &tm);
		INT nLineCount = contents.size();

		HDC hMemDC = ::CreateCompatibleDC(hdc);
		HBITMAP hMemBitmap = ::CreateCompatibleBitmap(hdc, rc.right - rc.left, nLineCount*tm.tmHeight);
		HBITMAP hOldMemBitmap = Lim::Select(hMemDC, hMemBitmap);

		HBRUSH hWhiteBrush = ::CreateSolidBrush(RGB(255, 255, 255));
		HBRUSH hOldBrush = Lim::Select(hMemDC, hWhiteBrush);
		RECT rcBitmap = { 0, 0, rc.right - rc.left, nLineCount*tm.tmHeight };
		::FillRect(hMemDC, &rcBitmap, hWhiteBrush);
		Lim::Select(hMemDC, hWhiteBrush);
		::DeleteObject(hWhiteBrush);

		LOGFONT logfont = { 0 };
		logfont.lfHeight = 12;
		_tcsncpy_s(logfont.lfFaceName, _T("Gulim"), 5);

		HFONT hFont = ::CreateFontIndirect(&logfont);
		HFONT hOldFont = Lim::Select(hMemDC, hFont);

		std::list<std::tstring>::iterator it;
		std::tstring all(_T(""));
		for (it = contents.begin(); it != contents.end(); it++) {
			all += *it;
			all += _T("\r\n");
		}

		RECT rcText = { 0, 0, rc.right - rc.left, rc.bottom - rc.top };
		::DrawText(hMemDC, all.c_str(), -1, &rcText, DT_CENTER | DT_WORDBREAK);
		//RECT rcLine = { 0, 0, rc.right - rc.left, tm.tmHeight };
		//std::list<std::tstring>::iterator it;
		//int expectedHeight;
		//for (it = contents.begin(); it != contents.end(); it++) {
		//	::DrawText(hMemDC, it->c_str(), -1, &rcLine, DT_CENTER | DT_WORDBREAK);
		//	::OffsetRect(&rcLine, 0, tm.tmHeight);
		//}

		Lim::Select(hMemDC, hOldFont);
		::DeleteObject(hFont);

		::BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);

		Lim::Select(hMemDC, hOldMemBitmap);
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);
	}
	void scrollcontexts(HDC hdc, const RECT& rc) {

	}
};

AboutWindow::AboutWindow() {
	pimpl = new AboutImpl;
}
AboutWindow::~AboutWindow() {
	Lim::SafeDelete(pimpl);
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

	pimpl->loadtextfile(_T("about.txt"));

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
	AddEventHandler(WM_TIMER, (Window::Reaction)&AboutWindow::OnTimer);
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
	pimpl->drawcontents(hdc, rc);
	//::SetBkMode(hdc, TRANSPARENT);
	//::DrawText(hdc, _T("About This...Hello"), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	::EndPaint(hWnd, &ps);
	//return ::DefWindowProc(hWnd, WM_PAINT, wParam, lParam);
	return 0;
}
LRESULT AboutWindow::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Invalidate();
	return 0;
}