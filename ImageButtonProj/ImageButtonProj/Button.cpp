#include <string>
#include <ctime>
#include <map>
#include <list>
#include "Button.h"
#include "utility.hpp"

typedef LRESULT(Button::*MessageHandler)(HWND, WPARAM, LPARAM);
class ButtonImpl {
public :
	ButtonImpl()
		: registered(false)
		, szClassName(_T("ButtonClass"))
		, bTrackMouseEvent(false)
		, bButtonDown(false)
		, resp_normal(nullptr)
		, resp_hover(nullptr)
		, resp_clicked(nullptr)
		, resp_pressing(nullptr)
		, pOwnerObject(nullptr)
	{
	}
	void registerClass() {
		HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);

		WNDCLASSEX wcex = { 0 };
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wcex.hInstance = hInstance;
		wcex.lpfnWndProc = &ButtonImpl::WndProc;
		wcex.lpszClassName = szClassName;
		wcex.style = CS_VREDRAW | CS_HREDRAW;
		//wcex.cbClsExtra = sizeof(ButtonImpl*);
		//wcex.cbWndExtra = sizeof(ButtonImpl*);

		if (!::RegisterClassEx(&wcex)) {
			DWORD dwError = ::GetLastError();
			return;
		}

		registered = true;
	}
	void SetMessageHandler(UINT uMsg, MessageHandler handler) {
		MessageMap[uMsg] = handler;
	}
	LRESULT UserProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (MessageMap[uMsg]) {
			return (pOwnerObject->*MessageMap[uMsg])(hWnd, wParam, lParam);
		}

		return BaseProc(hWnd, uMsg, wParam, lParam);
	}
	LRESULT BaseProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (uMsg == WM_CREATE) {

			return 0;
		}
		else if (uMsg == WM_PAINT) {
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hWnd, &ps);

			Rect rc = Rect::clientRect(hWnd);

			HPEN hBGPen = ::CreatePen(PS_SOLID, 1, RGB(100, 100, 200));
			HPEN hOldPen = Select(hdc, hBGPen);

			HBRUSH hBGBrush;
			if (btnState == HOVER) {
				hBGBrush = ::CreateSolidBrush(RGB(100, 100, 200));
			}
			else if (btnState == PRESSED) {
				hBGBrush = ::CreateSolidBrush(RGB(200, 100, 200));
			}
			else if (btnState == CLICKED) {
				hBGBrush = ::CreateSolidBrush(RGB(200, 100, 100));
			}
			else if (btnState == PRESSING) {
				hBGBrush = ::CreateSolidBrush(RGB(100, 200, 200));
			}
			else {
				hBGBrush = ::CreateSolidBrush(RGB(255, 255, 255));
			}
			HBRUSH hOldBrush = Select(hdc, hBGBrush);

			::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			Select(hdc, hOldBrush);
			::DeleteObject(hBGBrush);

			Select(hdc, hOldPen);
			::DeleteObject(hBGPen);


			int nOldMode = ::SetBkMode(hdc, TRANSPARENT);
			::DrawText(hdc, szText.c_str(), -1, &Rect::clientRect(hWnd), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			::SetBkMode(hdc, nOldMode);

			::EndPaint(hWnd, &ps);
			return 0;
		}
		else if (uMsg == WM_MOUSEHOVER) {
			bTrackMouseEvent = false;
			btnState = HOVER;

			Rect::invalidate(hWnd, FALSE);
			if (resp_hover) {
				(*resp_hover)();
			}

			HWND hParent = (HWND)::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT);
			LONG nId = (LONG)::GetWindowLongPtr(hWnd, GWLP_ID);

			::SendMessage(hParent, WM_COMMAND, MAKEWPARAM(nId, BM_HOVER), (LPARAM)hWnd);
			return 0;
		}
		else if (uMsg == WM_MOUSELEAVE) {
			bTrackMouseEvent = false;
			btnState = NORMAL;
			bButtonDown = false;

			::KillTimer(hWnd, 1);

			Rect::invalidate(hWnd, FALSE);
			if (resp_normal) {
				(*resp_normal)();
			}

			HWND hParent = (HWND)::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT);
			LONG nId = (LONG)::GetWindowLongPtr(hWnd, GWLP_ID);

			::SendMessage(hParent, WM_COMMAND, MAKEWPARAM(nId, BM_NORMAL), (LPARAM)hWnd);
			return 0;
		}
		else if (uMsg == WM_MOUSEMOVE) {
			if (!bTrackMouseEvent) {
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_HOVER | TME_LEAVE;
				tme.dwHoverTime = 1;
				tme.hwndTrack = hWnd;
				::TrackMouseEvent(&tme);
				bTrackMouseEvent = true;
			}
			return 0;
		}
		else if (uMsg == WM_LBUTTONDOWN) {
			bButtonDown = true;
			btnState = PRESSED;
			Rect::invalidate(hWnd, FALSE);

			e = s = std::clock();
			::SetTimer(hWnd, 1, 100, NULL);
			return 0;
		}
		else if (uMsg == WM_LBUTTONUP) {
			if (bButtonDown) {
				bButtonDown = false;
				btnState = CLICKED;
				Rect::invalidate(hWnd, FALSE);

				::KillTimer(hWnd, 1);

				if (resp_clicked) {
					(*resp_clicked)();
				}

				HWND hParent = (HWND)::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT);
				LONG nId = (LONG)::GetWindowLongPtr(hWnd, GWLP_ID);

				::SendMessage(hParent, WM_COMMAND, MAKEWPARAM(nId, BM_CLICKED), (LPARAM)hWnd);
			}
			return 0;
		}
		else if (uMsg == WM_TIMER) {
			e = std::clock();
			if (e - s > 2000) {
				bButtonDown = false;
				btnState = PRESSING;
				::KillTimer(hWnd, 1);

				Rect::invalidate(hWnd, FALSE);
				if (resp_pressing) {
					(*resp_pressing)();
				}

				HWND hParent = (HWND)::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT);
				LONG nId = (LONG)::GetWindowLongPtr(hWnd, GWLP_ID);

				::SendMessage(hParent, WM_COMMAND, MAKEWPARAM(nId, BM_PRESSING), (LPARAM)hWnd);
			}
			return 0;
		}
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		ButtonImpl* pThis = reinterpret_cast<ButtonImpl*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (pThis) {
			pThis->UserProc(hWnd, uMsg, wParam, lParam);
		}
		else {
			if (uMsg == WM_CREATE) {
				LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
				ButtonImpl* pThis = reinterpret_cast<ButtonImpl*>(lpcs->lpCreateParams);
				::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToLong(pThis));
				
				return pThis->UserProc(hWnd, uMsg, wParam, lParam);
			}
		}

		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	std::wstring GetText() const {
		return szText;
	}
	void SetText(const std::wstring& szText) {
		this->szText = szText;
	}
	BUTTON_STATE GetState() const {
		return btnState;
	}
public :
	bool registered;
	LPCTSTR szClassName;
	std::wstring szText;
	BUTTON_STATE btnState;
	bool bTrackMouseEvent;
	bool bButtonDown;
	clock_t s, e;
	std::shared_ptr<EventResponser> resp_normal;
	std::shared_ptr<EventResponser> resp_hover;
	std::shared_ptr<EventResponser> resp_clicked;
	std::shared_ptr<EventResponser> resp_pressing;
	std::map<UINT, MessageHandler> MessageMap;
	Button* pOwnerObject;
};
Button::Button()
	: impl(std::make_shared<ButtonImpl>())
{
}

Button::~Button()
{
}

HWND Button::Create(HWND hParentWnd, Rect rc, LPCTSTR szText)
{
	if (!impl->registered) {
		impl->registerClass();
	}
	HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
	impl->SetText(szText);
	hOwner = ::CreateWindowEx(0, impl->szClassName, szText, dwStyle, rc.left, rc.top, rc.width(), rc.height(),
		hParentWnd, (HMENU)generateId(), hInstance, impl.get());

	this->hParentWnd = hParentWnd;

	return HWND();
}

std::wstring Button::GetText() const
{
	return impl->GetText();
}

void Button::SetText(const std::wstring& szText)
{
	impl->SetText(szText);
}

BUTTON_STATE Button::GetState() const
{
	return impl->GetState();
}

unsigned long Button::GetId() const
{
	return (unsigned long)::GetWindowLongPtr(hOwner, GWLP_ID);
}

void Button::setResponseNormal(std::shared_ptr<EventResponser> res)
{
	impl->resp_normal = res;
}

void Button::setResponseHover(std::shared_ptr<EventResponser> res)
{
	impl->resp_hover = res;
}

void Button::setResponseClicked(std::shared_ptr<EventResponser> res)
{
	impl->resp_clicked = res;
}

void Button::setResponsePressing(std::shared_ptr<EventResponser> res)
{
	impl->resp_pressing = res;
}

void Button::SetMessageHandler(Button* obj, UINT uMsg, MessageHandler handler)
{
	impl->pOwnerObject = obj;
	impl->SetMessageHandler(uMsg, handler);
}

LPCTSTR Button::GetButtonClassName() const
{
	return impl->szClassName;
}
