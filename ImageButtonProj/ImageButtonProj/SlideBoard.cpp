#include "SlideBoard.h"
#include "utility.hpp"
#include "ImageDepot.h"
#include <list>
#include <sstream>

struct Box {
	Box(LONG c) : cBlock({c, c}), rcBox({0, 0, c, c}), nState(false) {}
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
		// background
		//HPEN hItemPen = ::CreatePen(PS_SOLID, 1, RGB(200,100,100));
		//HPEN hOldPen = Select(hdc, hItemPen);
		//HBRUSH hItemBrush = ::CreateSolidBrush(RGB(230,130,130));
		//HBRUSH hOldBrush = Select(hdc, hItemBrush);

		//::Rectangle(hdc, rcBox.left, rcBox.top, rcBox.right, rcBox.bottom);

		//Select(hdc, hOldBrush);
		//::DeleteObject(hItemBrush);
		//Select(hdc, hOldPen);
		//::DeleteObject(hItemPen);

		// background image
		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		HBITMAP hBitmap = ImageDepot[_T("item.bmp")];
		HBITMAP hOldBitmap = Select(hBitmapDC, ImageDepot[_T("item.bmp")]);

		BITMAP bm;
		::GetObject(hBitmap, sizeof(BITMAP), &bm);
		int cx = bm.bmHeight;
		int cy = bm.bmHeight;

		if (nState) {
			::StretchBlt(hdc,
				rcBox.left, rcBox.top, rcBox.right - rcBox.left, rcBox.bottom - rcBox.top,
				hBitmapDC,
				cx, 0,
				cx, cy,
				SRCCOPY);
		}
		else {
			::StretchBlt(hdc,
				rcBox.left, rcBox.top, rcBox.right - rcBox.left, rcBox.bottom - rcBox.top,
				hBitmapDC,
				0, 0,
				cx, cy,
				SRCCOPY);
		}

		Select(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);

		// text
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
		return cBlock;
	}
	void setOperation(std::shared_ptr<Operation> _op) {
		op = _op;
	}
	bool HitTest(HWND hParentWnd) {
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hParentWnd, &ptMouse);
		if (::PtInRect(&rcBox, ptMouse)) {
			nState = true;
		}
		else {
			nState = false;
		}
		return nState;
	}
private :
	bool nState;
	SIZE cBlock;
	RECT rcBox;
	std::wstring title;
	std::shared_ptr<Operation> op;
};
class SlideBoardImpl {
public :
	HWND hMainWnd;
	static WNDPROC lpfnWndProc;
	std::list<Box> boxlist;
	BOOL bRegistered;
	LPCTSTR szClassName;

	SlideBoardImpl() : hMainWnd(NULL), bRegistered(FALSE), szClassName(_T("SlideBoardClass")) {

	}

	void Invalidate(BOOL bErase = TRUE) {
		RECT rc;
		::GetClientRect(hMainWnd, &rc);
		::InvalidateRect(hMainWnd, &rc, bErase);
	}
	void AddBox(LPCTSTR title, std::shared_ptr<Operation> op) {
		RECT boardRect;
		::GetClientRect(hMainWnd, &boardRect);

		Box item(100);
		RECT rc = { 0, 0, 100, 100 };
		::OffsetRect(&rc, 10, 10);
		for (size_t i = 0; i < boxlist.size(); i++) {
			::OffsetRect(&rc, 110, 0);
			if (rc.right + 10 >= boardRect.right) {
				::OffsetRect(&rc, 0, 110);
				rc.left = 10;
				rc.right = 100 + 10;
			}
		}
		item.Move(rc);
		item.Title(title);
		item.setOperation(op);

		boxlist.push_back(item);

		Invalidate();
	}
	BOOL Register() {
		HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);

		WNDCLASSEX wcex = { 0 };
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wcex.hIcon = NULL;
		wcex.hInstance = hInstance;
		wcex.lpfnWndProc = SlideBoardImpl::WndProc;
		wcex.lpszClassName = szClassName;
		wcex.style = CS_VREDRAW | CS_HREDRAW;

		if (!::RegisterClassEx(&wcex)) {
			DWORD dwError = ::GetLastError();
			return FALSE;
		}

		bRegistered = TRUE;
		return TRUE;
	}
	HWND Create(HWND hParentWnd, const RECT& rc, const SIZE& cBlock) {
		if (!bRegistered) {
			Register();
		}
		HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);

		hMainWnd = ::CreateWindowEx(0, szClassName, _T(""),
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			hParentWnd, NULL, hInstance, this);

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
			for each (Box box in boxlist) {
				box.Draw(hdc);
			}

			::EndPaint(hWnd, &ps);
			return 0;
		}
		else if (uMsg == WM_LBUTTONDOWN) {

			return 0;
		}
		else if (uMsg == WM_LBUTTONUP) {

			return 0;
		}
		else if (uMsg == WM_MOUSEMOVE) {

			HDC hdc = ::GetDC(hWnd);
			for each (Box box in boxlist) {
				box.HitTest(hWnd);
				box.Draw(hdc);
			}
			::ReleaseDC(hWnd, hdc);
			return 0;
		}
		else if (uMsg == WM_SIZE) {
			RECT rc;
			::GetClientRect(hWnd, &rc);
			if (!boxlist.empty()) {
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

					::OffsetRect(&rcBox, 10 + cBox.cx, 0);
				}

			}
		}

		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		SlideBoardImpl* pThis = reinterpret_cast<SlideBoardImpl*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (pThis) {
			return pThis->UserProc(hWnd, uMsg, wParam, lParam);
		}
		else {
			if (uMsg == WM_CREATE) {
				LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
				SlideBoardImpl* pThis = reinterpret_cast<SlideBoardImpl*>(lpcs->lpCreateParams);
				::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToLong(pThis));

				pThis->UserProc(hWnd, uMsg, wParam, lParam);
			}

		}
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
};
WNDPROC SlideBoardImpl::lpfnWndProc;

SlideBoard::SlideBoard() : impl(new SlideBoardImpl) {

}
SlideBoard::~SlideBoard() {

}
HWND SlideBoard::Create(HWND hParentWnd, const RECT& rc, const SIZE& cBlock) {
	hOwner = impl->Create(hParentWnd, rc, cBlock);
	return hOwner;
}
void SlideBoard::AddBox(LPCTSTR title, std::shared_ptr<Operation> op) {
	impl->AddBox(title, op);
}
size_t SlideBoard::GetBoxLength() const {
	return impl->boxlist.size();
}