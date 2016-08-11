#pragma once

#include <Windows.h>
#include <tchar.h>

#pragma comment(lib, "Msimg32.lib")

class ImageButton {
	enum BUTTONSTATE {
		BST_NORMAL,
		BST_HOVER,
		BST_CLICK
	};
public :
	ImageButton();
	~ImageButton();

public :
	HWND Create(HWND hParentWnd, RECT& rc, UINT uId, LPCTSTR szFileName, SIZE& cBlock, COLORREF clrTrans = RGB(255,255,255));

	LRESULT CALLBACK ButtonProc(HWND, UINT, WPARAM, LPARAM);
protected :
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

private :
	HBITMAP hBitmap;
	BUTTONSTATE nState;
	static WNDPROC lpfnButtonProc;
	INT cx, cy;
	BOOL bTrackMouseEvent;
	COLORREF clrTransparent;
};