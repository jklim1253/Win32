#include "ImageButton.h"

WNDPROC ImageButton::lpfnButtonProc;

#define TIMER_UPDATE 1
#define BM_SETIMAGELIST (WM_USER+1)

ImageButton::ImageButton() 
	: hBitmap(NULL)
	, nState(BST_NORMAL)
	, cx(0)
	, cy(0)
	, bTrackMouseEvent(FALSE)
	, clrTransparent(RGB(255,255,255)) {

}
ImageButton::~ImageButton() {

}

HWND ImageButton::Create(HWND hParentWnd, RECT& rc, UINT uId, LPCTSTR szFileName, SIZE& cBlock, COLORREF clrTrans/*=RGB(255,255,255)*/) {
	HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);

	HWND hWnd = ::CreateWindowEx(0, _T("Button"), _T(""),
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		hParentWnd, (HMENU)uId, hInstance, NULL);
	::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToLong(this));
	lpfnButtonProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, PtrToLong(&ImageButton::WndProc));

	hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	::SendMessage(hWnd, BM_SETIMAGELIST, MAKEWPARAM(cBlock.cx, cBlock.cy), 0);

	clrTransparent = clrTrans;

	return hWnd;
}

LRESULT CALLBACK ImageButton::ButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);
		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hBitmap);

		RECT rc;
		::GetClientRect(hWnd, &rc);

		HBRUSH hBarBrush = ::CreateSolidBrush(RGB(100, 100, 200));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBarBrush);

		::FillRect(hdc, &rc, hBarBrush);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBarBrush);

		::TransparentBlt(hdc,
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			hBitmapDC,
			nState*cx, 0,
			cx, cy,
			clrTransparent);

		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);

		::EndPaint(hWnd, &ps);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		nState = BST_CLICK; 
	}
	else if (uMsg == WM_LBUTTONUP) {
		nState = BST_NORMAL;
	}
	else if (uMsg == WM_MOUSEHOVER) {
		if (bTrackMouseEvent) {
			bTrackMouseEvent = FALSE;

			if (nState != BST_HOVER) {
				nState = BST_HOVER;
				::InvalidateRect(hWnd, NULL, FALSE);
			}
		}
	}
	else if (uMsg == WM_MOUSELEAVE) {
		if (bTrackMouseEvent) {
			bTrackMouseEvent = FALSE;

			if (nState != BST_NORMAL) {
				nState = BST_NORMAL;
				::InvalidateRect(hWnd, NULL, FALSE);
			}
		}
	}
	else if (uMsg == BM_SETIMAGELIST) {
		cx = LOWORD(wParam);
		cy = HIWORD(wParam);

		if (!bTrackMouseEvent) {
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.dwHoverTime = 1;
			tme.hwndTrack = hWnd;
			::TrackMouseEvent(&tme);
			bTrackMouseEvent = TRUE;
		}
	}
	else if (uMsg == WM_MOUSEMOVE) {
		if (!bTrackMouseEvent) {
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.dwHoverTime = 1;
			tme.hwndTrack = hWnd;
			::TrackMouseEvent(&tme);
			bTrackMouseEvent = TRUE;
		}
	}

	return ::CallWindowProc(lpfnButtonProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ImageButton::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ImageButton* pThis = reinterpret_cast<ImageButton*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (pThis) {
		return pThis->ButtonProc(hWnd, uMsg, wParam, lParam);
	}

	return ::CallWindowProc(lpfnButtonProc, hWnd, uMsg, wParam, lParam);
}