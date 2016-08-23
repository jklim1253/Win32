#pragma once

#include <Windows.h>
#include <tchar.h>

class SlideBoardImpl;
class SlideBoard {

public :
	SlideBoard();
	~SlideBoard();

public :
	HWND Create(HWND hParentWnd, const RECT& rc, const SIZE& cBlock);

	HWND GetHandle();
protected :

private :
	SlideBoardImpl* impl;
};