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

struct MinMaxResponse : public EventResponser {
	bool bFullScreen;
	MinMaxResponse(HWND hParent) : EventResponser(hParent), bFullScreen(false) {}
	void operator ()() {
		if (bFullScreen) {
			::SendMessage(hParent, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else {
			::SendMessage(hParent, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		Rect rc = Rect::clientRect(hParent);
		// move close button
		Rect rcClose = Rect::clientRect(Close.GetHandle());
		Close.Move(rc.right - rcClose.width(), 0);
		// move board
		Board.Move(rc.Deflate(10, 60, 10, 10));
		bFullScreen = !bFullScreen;
	}
};
struct CloseResponse : public EventResponser {
	CloseResponse(HWND hParent) :EventResponser(hParent) {}
	void operator ()() {
		::SendMessage(hParent, WM_SYSCOMMAND, SC_CLOSE, 0);
	}
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_DESTROY) {
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_CREATE) {

		SIZE cBlock = { 100,100 };
		RECT rcButton = { 0,0,50,50 };
		Add.Create(hWnd, rcButton, _T(""), _T("add.bmp"), cBlock);
		::OffsetRect(&rcButton, 50, 0);
		MinMax.Create(hWnd, rcButton, _T(""), _T("max.bmp"), cBlock);
		MinMax.setResponseClicked(std::make_shared<MinMaxResponse>(hWnd));

		RECT rc;
		::GetClientRect(hWnd, &rc);

		rcButton = { rc.right - 50, 0, rc.right, 50 };
		Close.Create(hWnd, rcButton, _T(""), _T("close.bmp"), cBlock);
		Close.setResponseClicked(std::make_shared<CloseResponse>(hWnd));

		LONG margin = 10;
		RECT rcBoard = { rc.left + margin, rc.top + 50 + margin, rc.right - margin, rc.bottom - margin };
		SIZE cBoard;
		if (rcBoard.right - rcBoard.left > rcBoard.bottom - rcBoard.top) {
			cBoard.cx = cBoard.cy = rcBoard.bottom - rcBoard.top;
		}
		else {
			cBoard.cx = cBoard.cy = rcBoard.right - rcBoard.left;
		}
		Board.Create(hWnd, rcBoard, cBoard);

		//test.Create(hWnd, Rect(200, 0, Size(100,50)), _T("Test"));
		//test.setResponseClicked(std::make_shared<TestResponse>());
		////test.setResponsePressing(std::make_shared<PressResponse>());
		////test.setResponseHover(std::make_shared<HoverResponse>());
		//test2.Create(hWnd, Rect(300, 0, Size(100,50)), _T("Test2"));
		//test2.setResponseClicked(std::make_shared<Test2Response>());
		return 0;
	}
	else if (uMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::SetBkMode(hdc, TRANSPARENT);
		::DrawText(hdc, _T("If want to move window, grab any point and then drag."), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		::EndPaint(hWnd, &ps);
	}
	else if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_ESCAPE) {
			::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
			return 0;
		}
	}
	else if (uMsg == WM_COMMAND) {
		UINT uId = LOWORD(wParam);
		UINT uNotify = HIWORD(wParam);
		if (uId == Add.GetId() && uNotify == BM_CLICKED) {
			//::MessageBox(NULL, _T("Add"), _T("Info"), MB_OK);
			std::wostringstream oss;
			oss << (Board.GetBoxLength() + 1) << " item";
			std::shared_ptr<Operation> op(std::make_shared<RunOperation>());
			Board.AddBox(oss.str().c_str(), op);
		}
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

		bGrabWindow = TRUE;
	}
	else if (uMsg == WM_LBUTTONUP) {

		::ReleaseCapture();

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

