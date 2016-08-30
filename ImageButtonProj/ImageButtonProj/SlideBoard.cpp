#include "SlideBoard.h"
#include "utility.hpp"
#include "ImageButton.h"
#include "resource.h"
#include <list>
#include <sstream>
#include <windowsx.h>

#define SBI_DELETE	(WM_USER+1)
class Box : public ImageButton {
public :
	Box(HWND hParent, LPCTSTR szText) {
		ImageButton::Create(hParent, Rect(), szText, ITEM_BITMAP, Size(400,400));

		SetMessageHandler(this, WM_CONTEXTMENU, (Button::MessageHandler)&Box::OnContextMenu);
	}
	LRESULT OnContextMenu(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		HWND hMenuWnd = (HWND)wParam;

		if (hMenuWnd != hWnd) return 0;

		Point ptMenu(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		HMENU hMenu = ::CreatePopupMenu();
		MENUITEMINFO mii;
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_STRING | MIIM_DATA | MIIM_ID;
		mii.dwTypeData = _T("Edit...");
		mii.cch = _tcslen(mii.dwTypeData);
		mii.wID = 1000;
		::InsertMenuItem(hMenu, 0, TRUE, &mii);

		mii.dwTypeData = _T("Delete");
		mii.cch = _tcslen(mii.dwTypeData);
		mii.wID = 1001;
		::InsertMenuItem(hMenu, 1, TRUE, &mii);

		int nSelect = ::TrackPopupMenuEx(hMenu, 
			TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NOANIMATION | TPM_RETURNCMD | TPM_LEFTBUTTON, 
			ptMenu.x, ptMenu.y, 
			hMenuWnd, NULL);

		switch (nSelect) {
		case 1000 :
			// TODO : call Edit item.
			// popup edit window for item.
			::MessageBox(NULL, _T("Edit"), _T("Edit"), MB_OK);
			break;
		case 1001 :
			// TODO : call delete item.
			// using hWnd, who is occur event.
			::SendMessage(GetParentHandle(), SBI_DELETE, (WPARAM)hWnd, 0);
			break;
		default :
			break;
		}

		::DestroyMenu(hMenu);

		return 0;
	}

private :
};
class SlideBoardImpl {
public :
	HWND hMainWnd;
	static WNDPROC lpfnWndProc;
	std::list<std::shared_ptr<Box>> boxlist;
	BOOL bRegistered;
	LPCTSTR szClassName;

	SlideBoardImpl() : hMainWnd(NULL), bRegistered(FALSE), szClassName(_T("SlideBoardClass")) {

	}

	void Invalidate(BOOL bErase = TRUE) {
		Rect::invalidate(hMainWnd, bErase);
	}
	Rect get_last_position() {
		// TODO : implement SlideBoardImpl::get_last_position()
		if (boxlist.empty()) {
			return Rect(10, 10, 110, 110);
		}

		// get last element coordinate relative to parent window.
		Rect rcParent = Rect::windowRect(hMainWnd);
		Rect last = boxlist.back()->GetWindowRect();
		last -= Size(rcParent.left, rcParent.top);

		return get_next_position(last);
	}
	Rect get_next_position(Rect& current) {
		// TODO : implement SlideBoardImpl::get_next_position()
		Rect rc = Rect::clientRect(hMainWnd);
		LONG margin = 10;
		Rect rcBox = { 0, 0, 100, 100 };

		// if calculated next position is greater than client area.
		if (current.right + rcBox.width() + margin >= rc.right) {
			current += Size(0, margin + rcBox.height());
			current.left = margin;
			current.right = margin + rcBox.width();
		}
		else {
			current += Size(margin + rcBox.width(), 0);
		}

		return current;
	}
	void AddItem(LPCTSTR title, std::shared_ptr<EventResponser> op) {
		// create new item box.
		std::shared_ptr<Box> item(std::make_shared<Box>(hMainWnd, title));

		// set item box information.
		Rect rcBox = get_last_position();
		item->Move(rcBox);
		item->setResponseClicked(op);

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
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
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
			//for each (Box box in boxlist) {
			//	box.Draw(hdc);
			//}

			::EndPaint(hWnd, &ps);
			return 0;
		}
		//else if (uMsg == WM_LBUTTONDOWN) {

		//	return 0;
		//}
		//else if (uMsg == WM_LBUTTONUP) {

		//	return 0;
		//}
		//else if (uMsg == WM_MOUSEMOVE) {

		//	HDC hdc = ::GetDC(hWnd);
		//	for each (Box box in boxlist) {
		//		box.HitTest(hWnd);
		//		box.Draw(hdc);
		//	}
		//	::ReleaseDC(hWnd, hdc);
		//	return 0;
		//}
		else if (uMsg == WM_SIZE) {
			// re-arrange items
			Rect current = { 0,0,100,100 };
			current += Size(-100, 10);
			Rect next;
			for (std::shared_ptr<Box>& box : boxlist) {
				next = get_next_position(current);
				box->Move(next);
				current = next;
			}
		}
		else if (uMsg == SBI_DELETE) {
			::MessageBox(NULL, _T("Delete Item"), _T("Delete"), MB_OK);
			return 0;
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

				return pThis->UserProc(hWnd, uMsg, wParam, lParam);
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
void SlideBoard::AddItem(LPCTSTR title, std::shared_ptr<EventResponser> op) {
	impl->AddItem(title, op);
}
size_t SlideBoard::length() const {
	return impl->boxlist.size();
}