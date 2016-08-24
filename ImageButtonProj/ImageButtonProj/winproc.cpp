#include "resource.h"
#include "winmain.h"
#include "ImageButton.h"
#include "SlideBoard.h"
#include <sstream>
#include "Button.h"

struct RunOperation : public Operation {
	virtual void operator ()() {
		::MessageBox(NULL, _T("Test"), _T("Test"), MB_OK);
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
		oss << (Board.GetBoxLength() + 1) << " item";
		std::shared_ptr<Operation> op(std::make_shared<RunOperation>());
		Board.AddBox(oss.str().c_str(), op);
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

		HPEN hBorderPen = ::CreatePen(PS_SOLID, 1, RGB(100, 100, 200));
		HPEN hOldPen = Select(hdc, hBorderPen);
		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		Select(hdc, hOldPen);
		::DeleteObject(hBorderPen);

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
	else if (uMsg == WM_COMMAND) {
		//UINT uId = LOWORD(wParam);
		//UINT uNotify = HIWORD(wParam);
		//if (uId == Add.GetId() && uNotify == BM_CLICKED) {
		//	//::MessageBox(NULL, _T("Add"), _T("Info"), MB_OK);
		//	std::wostringstream oss;
		//	oss << (Board.GetBoxLength() + 1) << " item";
		//	std::shared_ptr<Operation> op(std::make_shared<RunOperation>());
		//	Board.AddBox(oss.str().c_str(), op);
		//}
		//else if (uId == MinMax.GetId() && uNotify == BM_CLICKED) {
		//	if (bFullScreen) {
		//		::SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		//		RECT rc;
		//		::GetClientRect(hWnd, &rc);
		//		Close.Move(rc.right - 50, 0);

		//		::MoveWindow(Board.GetHandle(), 10, 60, rc.right - rc.left - 20, rc.bottom - rc.top - 70, TRUE);
		//	}
		//	else {
		//		::SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		//		RECT rc;
		//		::GetClientRect(hWnd, &rc);
		//		Close.Move(rc.right - 50, 0);
		//		::MoveWindow(Board.GetHandle(), 10, 60, rc.right - rc.left - 20, rc.bottom-rc.top - 70, TRUE);
		//	}
		//	bFullScreen = !bFullScreen;
		//}
		//else if (uId == Close.GetId() && uNotify == BM_CLICKED) {
		//	::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		//}
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		::SetCapture(hWnd);

		::GetCursorPos(&ptMouse);

		// TODO : need grapping hand cursor.
		::SetCursor(::LoadCursor(NULL, IDC_HAND));

		bGrabWindow = TRUE;
	}
	else if (uMsg == WM_LBUTTONUP) {

		::ReleaseCapture();

		::SetCursor(::LoadCursor(NULL, IDC_ARROW));

		bGrabWindow = FALSE;
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
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

