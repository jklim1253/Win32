#pragma once

#include <Windows.h>
#include <tchar.h>
#include <memory>

struct Operation {
	virtual void operator ()() = 0;
};
class SlideBoardImpl;
class SlideBoard {

public :
	SlideBoard();
	~SlideBoard();

public :
	HWND Create(HWND hParentWnd, const RECT& rc, const SIZE& cBlock);

	HWND GetHandle();

	void AddBox(LPCTSTR title, std::shared_ptr<Operation> op);
	size_t GetBoxLength() const;
protected :

private :
	SlideBoardImpl* impl;
};