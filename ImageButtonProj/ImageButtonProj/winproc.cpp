#include "resource.h"
#include "winmain.h"
#include "ImageButton.h"
#include "SlideBoard.h"
#include <sstream>
#include "Button.h"
#include <windowsx.h>

struct RunOperation : public EventResponser {
	virtual void operator ()() {
		::MessageBox(NULL, _T("What do u want?"), _T("Item Action"), MB_OK);
	}
};
ImageButton Add, MinMax, Close;
SlideBoard Board;
BOOL bFullScreen = FALSE;
BOOL bGrabWindow = FALSE;
POINT ptMouse;

struct AddResponse : public EventResponser {
	void operator()() {
		std::wostringstream oss;
		oss << (Board.length() + 1) << " item";
		std::shared_ptr<EventResponser> op(std::make_shared<RunOperation>());
		Board.AddItem(oss.str().c_str(), op);
	}
};
struct MinMaxResponse : public EventResponser {
	bool bFullScreen;
	MinMaxResponse(HWND hParent) : EventResponser(hParent), bFullScreen(false) {}
	void operator()() {
		if (bFullScreen) {
			::SendMessage(hParent, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else {
			::SendMessage(hParent, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		Rect rc = Rect::clientRect(hParent);
		// move close button
		Rect rcClose = Rect::clientRect(Close.GetHandle());
		Close.Move(rc.right - rcClose.width() - 5, 5);
		// move board
		Board.Move(rc.Deflate(10, 60, 10, 10));
		bFullScreen = !bFullScreen;
	}
};
struct CloseResponse : public EventResponser {
	CloseResponse(HWND hParent) :EventResponser(hParent) {}
	void operator()() {
		::SendMessage(hParent, WM_SYSCOMMAND, SC_CLOSE, 0);
	}
};
struct HoverResponse : public EventResponser {
	void operator()() {
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
	}
};
struct NormalResponse : public EventResponser {
	void operator()() {
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
};

HMENU hMenu = NULL;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_DESTROY) {
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_CREATE) {

		Rect rc = Rect::clientRect(hWnd);

		Size cBlock = { 100,100 };
		Rect rcButton = { 0,0,50,50 };
		rcButton += Size(5, 5);

		//Add.Create(hWnd, rcButton, _T(""), _T("add.bmp"), cBlock);
		Add.Create(hWnd, rcButton, _T(""), ADD_BITMAP, cBlock);
		MinMax.Move(5, 5);
		Add.setResponseClicked(std::make_shared<AddResponse>());
		Add.setResponseHover(std::make_shared<HoverResponse>());
		Add.setResponseNormal(std::make_shared<NormalResponse>());

		MinMax.Create(hWnd, rcButton, _T(""), MINMAX_BITMAP, cBlock);
		MinMax.Move(55, 5);
		MinMax.setResponseClicked(std::make_shared<MinMaxResponse>(hWnd));
		MinMax.setResponseHover(std::make_shared<HoverResponse>());
		MinMax.setResponseNormal(std::make_shared<NormalResponse>());

		Close.Create(hWnd, rcButton, _T(""), CLOSE_BITMAP, cBlock);
		Close.Move(rc.right - 55, 5);
		Close.setResponseClicked(std::make_shared<CloseResponse>(hWnd));
		Close.setResponseHover(std::make_shared<HoverResponse>());
		Close.setResponseNormal(std::make_shared<NormalResponse>());

		LONG margin = 10;
		Size cBoard = {100, 100};
		Board.Create(hWnd, Rect(), cBoard);
		Board.Move(rc.Deflate(margin, 50 + margin, margin, margin));

		return 0;
	}
	else if (uMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		Rect rc = Rect::clientRect(hWnd);

		// background, border
		HPEN hBorderPen = ::CreatePen(PS_SOLID, 1, RGB(100, 100, 200));
		HPEN hOldPen = Select(hdc, hBorderPen);
		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		Select(hdc, hOldPen);
		::DeleteObject(hBorderPen);

		// center text
		int nOldMode = ::SetBkMode(hdc, TRANSPARENT);
		::DrawText(hdc, _T("If want to move window, grab any point and then drag."), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		::SetBkMode(hdc, nOldMode);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_ESCAPE) {
			::SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return 0;
		}
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		::SetCapture(hWnd);

		::GetCursorPos(&ptMouse);

		// TODO : need grapping hand cursor.
		::SetCursor(::LoadCursor(NULL, IDC_HAND));

		bGrabWindow = TRUE;

		return 0;
	}
	else if (uMsg == WM_LBUTTONUP) {

		::ReleaseCapture();

		::SetCursor(::LoadCursor(NULL, IDC_ARROW));

		bGrabWindow = FALSE;

		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE) {
		if (bFullScreen) return 0;

		if (!bGrabWindow) return 0;

		POINT ptOld = ptMouse;

		::GetCursorPos(&ptMouse);

		POINT ptDiff = { ptMouse.x - ptOld.x, ptMouse.y - ptOld.y };

		RECT rcWin;
		::GetWindowRect(hWnd, &rcWin);
		::MoveWindow(hWnd, rcWin.left + ptDiff.x, rcWin.top + ptDiff.y, rcWin.right - rcWin.left, rcWin.bottom - rcWin.top, TRUE);

		return 0;
	}
	else if (uMsg == WM_CONTEXTMENU) {
		HWND hMenuWnd = (HWND)wParam;

		if (hMenuWnd != hWnd) return 0;

		Point ptMenu(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		// create menu.
		hMenu = ::CreatePopupMenu();
		MENUITEMINFO mii = { 0 };
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_STRING | MIIM_DATA | MIIM_ID;
		mii.dwTypeData = _T("Exit");
		mii.cch = _tcslen(mii.dwTypeData);
		mii.wID = 1000;
		::InsertMenuItem(hMenu, 0, TRUE, &mii);

		mii.dwTypeData = _T("Config...");
		mii.cch = _tcslen(mii.dwTypeData);
		mii.wID = 1001;
		::InsertMenuItem(hMenu, 1, TRUE, &mii);

		// show popup-menu.
		int nSelect = ::TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NOANIMATION | TPM_RETURNCMD | TPM_LEFTBUTTON, ptMenu.x, ptMenu.y, hMenuWnd, NULL);

		switch (nSelect) {
		case 1000 :
			::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
			break;
		case 1001 :
			::MessageBox(NULL, _T("Config"), _T("Out"), MB_OK);
			break;
		default :
			break;
		}

		::DestroyMenu(hMenu);

		return 0;
	}
	else if (uMsg == WM_NCPAINT) {
		HDC hdc = ::GetWindowDC(hWnd);
		// Paint into this DC 
		Rect rc = Rect::windowRect(hWnd);
		::FillRect(hdc, &rc, GetStockBrush(WHITE_BRUSH));
		ReleaseDC(hWnd, hdc);
		return 1;
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

