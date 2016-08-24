#include "ImageDepot.h"
#include "ImageButton.h"
#include <string>

class ImageButtonImpl {
public :
	ImageButtonImpl()
		: hBitmap(NULL)
	{}
	LRESULT OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	}
	HBITMAP hBitmap;
	Size cBlock;
};
ImageButton::ImageButton() 
	: impl(std::make_shared<ImageButtonImpl>())
{

}
ImageButton::~ImageButton() {
}

HWND ImageButton::Create(HWND hParentWnd, Rect rc, LPCTSTR szText, LPCTSTR szImageFileName, Size cBlock) {
	Button::Create(hParentWnd, rc, szText);

	// load bitmap from file.
	impl->hBitmap = ImageDepot[szImageFileName];
	impl->cBlock = cBlock;

	SetMessageHandler(this, WM_PAINT, (Button::MessageHandler)&ImageButton::OnPaint);

	return hOwner;
}

HWND ImageButton::Create(HWND hParentWnd, Rect rc, LPCTSTR szText, UINT uResourceId, Size cBlock) {
	Button::Create(hParentWnd, rc, szText);

	// load bitmap from resource.
	impl->hBitmap = ImageDepot[uResourceId];
	impl->cBlock = cBlock;

	SetMessageHandler(this, WM_PAINT, (Button::MessageHandler)&ImageButton::OnPaint);

	return hOwner;
}

LRESULT ImageButton::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hWnd, &ps);

	Rect rc = Rect::clientRect(hWnd);
	HDC hBitmapDC = ::CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = Select(hBitmapDC, impl->hBitmap);

	LONG left = 0;
	BUTTON_STATE bs = Button::GetState();
	if (bs == HOVER) {
		left = impl->cBlock.cx;
	}
	else if (bs == PRESSED || bs == CLICKED || bs == PRESSING) {
		left = impl->cBlock.cx * 2;
	}
	else {
		left = 0;
	}

	::StretchBlt(hdc,
		rc.left, rc.top, rc.width(), rc.height(),
		hBitmapDC,
		left, 0, impl->cBlock.cx, impl->cBlock.cy,
		SRCCOPY);

	Select(hBitmapDC, hOldBitmap);
	::DeleteDC(hBitmapDC);

	std::wstring szText(GetText());
	if (szText.length() != 0) {
		int nOldMode = ::SetBkMode(hdc, TRANSPARENT);
		::DrawText(hdc, szText.c_str(), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		::SetBkMode(hdc, nOldMode);
	}

	::EndPaint(hWnd, &ps);
	return 0;
}
