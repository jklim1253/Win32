#include "SlideBoard.h"
#include "utility.hpp"
#include <list>
#include <sstream>
struct Box {
	Box(LONG c) : rcBox({ 0, 0, c, c }) {}
	void Title(const std::wstring& _title) {
		title = _title;
	}
	void Move(const RECT& rc) {
		rcBox = rc;
	}
	void Draw(HDC hdc, const RECT& rc) {
		Move(rc);
		Draw(hdc);
	}
	void Draw(HDC hdc) {
		HPEN hItemPen = ::CreatePen(PS_SOLID, 1, RGB(200,100,100));
		HPEN hOldPen = Select(hdc, hItemPen);
		HBRUSH hItemBrush = ::CreateSolidBrush(RGB(230,130,130));
		HBRUSH hOldBrush = Select(hdc, hItemBrush);

		::Rectangle(hdc, rcBox.left, rcBox.top, rcBox.right, rcBox.bottom);

		Select(hdc, hOldBrush);
		::DeleteObject(hItemBrush);
		Select(hdc, hOldPen);
		::DeleteObject(hItemPen);

		int oldMode = ::SetBkMode(hdc, TRANSPARENT);
		if (!title.empty()) {
			::DrawText(hdc, title.c_str(), -1, &rcBox, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		::SetBkMode(hdc, oldMode);
	}
	RECT getRect() const {
		return rcBox;
	}
	SIZE getSize() const {
		return SIZE({rcBox.right-rcBox.left, rcBox.bottom-rcBox.top});
	}
private :
	RECT rcBox;
	std::wstring title;
};
std::list<Box> boxlist;
class SlideBoardImpl {
public :
	HWND hMainWnd;
	static WNDPROC lpfnWndProc;

	HWND Create(HWND hParentWnd, const RECT& rc, const SIZE& cBlock) {
		HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);

		hMainWnd = ::CreateWindowEx(0, _T("STATIC"), _T(""),
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			hParentWnd, NULL, hInstance, NULL);
		::SetWindowLongPtr(hMainWnd, GWLP_USERDATA, PtrToLong(this));

		lpfnWndProc = (WNDPROC)::SetWindowLongPtr(hMainWnd, GWLP_WNDPROC, PtrToLong(&SlideBoardImpl::WndProc));

		for (int i = 0; i < 20; i++) {
			std::wostringstream oss;
			oss << i << " item";
			Box item(120);
			item.Title(oss.str());
			boxlist.push_back(item);
		}

		return hMainWnd;
	}

	LRESULT UserProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		if (uMsg == WM_PAINT) {
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hWnd, &ps);

			RECT rc;
			::GetClientRect(hWnd, &rc);

			// background
			HPEN hBGPen = ::CreatePen(PS_SOLID, 1, RGB(220, 120, 120));
			HPEN hOldPen = Select(hdc, hBGPen);
			HBRUSH hBGBrush = ::CreateSolidBrush(RGB(255, 255, 255));
			HBRUSH hOldBrush = Select(hdc, hBGBrush);

			::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			Select(hdc, hOldBrush);
			::DeleteObject(hBGBrush);
			Select(hdc, hOldPen);
			::DeleteObject(hBGPen);

			// items
			SIZE cBox = boxlist.front().getSize();
			RECT rcBox = { 0, 0, cBox.cx, cBox.cy };
			::OffsetRect(&rcBox, 10, 10);
			for (std::list<Box>::iterator it = boxlist.begin(); it != boxlist.end(); it++) {
				// move box
				it->Move(rcBox);
				if (it->getRect().right > rc.right) {
					rcBox.left = 10;
					rcBox.right = rcBox.left + cBox.cx;
					rcBox.top += 10 + cBox.cy;
					rcBox.bottom += 10 + cBox.cy;

					it->Move(rcBox);
				}

				// draw box
				it->Draw(hdc);

				::OffsetRect(&rcBox, 10 + cBox.cx, 0);
			}

			::EndPaint(hWnd, &ps);
		}
		else if (uMsg == WM_LBUTTONDOWN) {

		}
		else if (uMsg == WM_LBUTTONUP) {

		}
		else if (uMsg == WM_MOUSEMOVE) {

		}

		return ::CallWindowProc(lpfnWndProc, hWnd, uMsg, wParam, lParam);
	}
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		SlideBoardImpl* pThis = reinterpret_cast<SlideBoardImpl*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (pThis) {
			return pThis->UserProc(hWnd, uMsg, wParam, lParam);
		}
		return ::CallWindowProc(lpfnWndProc, hWnd, uMsg, wParam, lParam);
	}
};
WNDPROC SlideBoardImpl::lpfnWndProc;

SlideBoard::SlideBoard() : impl(new SlideBoardImpl) {

}
SlideBoard::~SlideBoard() {

}
HWND SlideBoard::Create(HWND hParentWnd, const RECT& rc, const SIZE& cBlock) {

	return impl->Create(hParentWnd, rc, cBlock);
}
HWND SlideBoard::GetHandle() {
	return impl->hMainWnd;
}
