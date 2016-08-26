#include "type.h"

Rect::Rect() {
	left = top = right = bottom = 0;
}
Rect::Rect(LONG l, LONG t, LONG r, LONG b) {
	left = l;
	top = t;
	right = r;
	bottom = b;
}
Rect::Rect(LONG l, LONG t, Size cs) {
	left = l;
	top = t;
	right = l + cs.cx;
	bottom = t + cs.cy;
}
Rect::Rect(Point lt, Point rb)
{
	left = lt.x;
	top = lt.y;
	right = rb.x;
	bottom = rb.y;
}
Rect::Rect(Point ct, Size cs)
{
	left = ct.x - cs.cx / 2;
	top = ct.y - cs.cy / 2;
	right = left + cs.cx;
	bottom = bottom + cs.cy;
}
Rect::Rect(RECT rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;
}
Rect Rect::toScreen(HWND hWnd) const {
	Point lt(left, top);
	Point rb(right, bottom);

	return Rect(lt.toScreen(hWnd), rb.toScreen(hWnd));
}
Rect Rect::toClient(HWND hWnd) const {
	Point lt(left, top);
	Point rb(right, bottom);

	return Rect(lt.toClient(hWnd), rb.toClient(hWnd));
}

Rect Rect::clientRect(HWND hWnd)
{
	Rect tmp;
	::GetClientRect(hWnd, &tmp);
	return tmp;
}

Rect Rect::windowRect(HWND hWnd)
{
	Rect tmp;
	::GetWindowRect(hWnd, &tmp);
	return tmp;
}

void Rect::invalidate(HWND hWnd, BOOL bErase)
{
	Rect rc;
	::GetClientRect(hWnd, &rc);
	::InvalidateRect(hWnd, &rc, bErase);
}

LONG Rect::width() const
{
	return (right - left);
}

LONG Rect::height() const
{
	return (bottom - top);
}

Point Rect::center() const
{
	return Point((left+right)/2, (top+bottom)/2);
}

Size Rect::size() const
{
	return Size(width(), height());
}

Rect Rect::Inflate(LONG cx, LONG cy)
{
	return Rect(left - cx, top - cy, right + cx, bottom + cy);
}

Rect Rect::Inflate(LONG l, LONG t, LONG r, LONG b)
{
	return Rect(left - l, top - t, right + r, bottom + b);
}

Rect Rect::Deflate(LONG cx, LONG cy)
{
	return Rect(left + cx, top + cy, right - cx, bottom - cy);
}

Rect Rect::Deflate(LONG l, LONG t, LONG r, LONG b)
{
	return Rect(left + l, top + t, right - r, bottom - b);
}

Rect Rect::operator+(Size cs) const
{
	return Rect(left+cs.cx,top+cs.cy,right+cs.cx,bottom+cs.cy);
}

Rect Rect::operator-(Size cs) const
{
	return Rect(left-cs.cx,top-cs.cy,right-cs.cx,bottom-cs.cy);
}

Rect & Rect::operator+=(Size cs)
{
	left += cs.cx;
	right += cs.cx;
	top += cs.cy;
	bottom += cs.cy;
	return *this;
}

Rect & Rect::operator-=(Size cs)
{
	left -= cs.cx;
	right -= cs.cx;
	top -= cs.cy;
	bottom -= cs.cy;
	return *this;
}

Size::Size()
{
	cx = cy = 0;
}

Size::Size(LONG cs)
{
	cx = cy = cs;
}

Size::Size(LONG x, LONG y)
{
	cx = x;
	cy = y;
}

Size::Size(SIZE cs)
{
	cx = cs.cx;
	cy = cs.cy;
}

Point::Point() {
	x = y = 0;
}
Point::Point(LONG _x, LONG _y) {
	x = _x;
	y = _y;
}

Point::Point(POINT pt)
{
	x = pt.x;
	y = pt.y;
}

Point Point::toScreen(HWND hWnd) const
{
	Point tmp(*this);
	::ClientToScreen(hWnd, &tmp);
	return tmp;
}

Point Point::toClient(HWND hWnd) const
{
	Point tmp(*this);
	::ScreenToClient(hWnd, &tmp);
	return tmp;
}

