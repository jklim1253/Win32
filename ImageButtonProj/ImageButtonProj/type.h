#pragma once

#include <Windows.h>

struct Rect;
struct Size;
struct Point;

struct Rect : public RECT {
	Rect();
	Rect(LONG l, LONG t, LONG r, LONG b);
	Rect(LONG l, LONG t, Size cs);
	Rect(Point lt, Point rb);
	Rect(Point ct, Size cs);
	Rect(RECT rc);

	Rect toScreen(HWND hWnd) const;
	Rect toClient(HWND hWnd) const;
	static Rect clientRect(HWND hWnd);
	static Rect windowRect(HWND hWnd);
	static void invalidate(HWND hWnd, BOOL bErase = TRUE);

	LONG width() const;
	LONG height() const;
	Point center() const;

	Rect Inflate(LONG cx, LONG cy);
	Rect Inflate(LONG l, LONG t, LONG r, LONG b);
	Rect Deflate(LONG cx, LONG cy);
	Rect Deflate(LONG l, LONG t, LONG r, LONG b);

	// OffsetRect
	Rect operator+(Size cs) const;
	Rect operator-(Size cs) const;
	Rect& operator+=(Size cs);
	Rect& operator-=(Size cs);
};
struct Size : public SIZE {
	Size();
	Size(LONG cs);
	Size(LONG x, LONG y);
	Size(SIZE cs);
};
struct Point : public POINT {
	Point();
	Point(LONG _x, LONG _y);
	Point(POINT pt);

	Point toScreen(HWND hWnd) const;
	Point toClient(HWND hWnd) const;
};