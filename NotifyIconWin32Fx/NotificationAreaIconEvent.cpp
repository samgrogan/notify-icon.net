#include "NotificationAreaIconEvent.h"

using namespace NotifyIcon::Win32;

EventType NotifyIconEventArgs::Type::get()
{
	return _type;
}

void NotifyIconEventArgs::Type::set(EventType type)
{
	_type = type;
}

int NotifyIconEventArgs::CursorX::get()
{
	return _cursor_x;
}

void NotifyIconEventArgs::CursorX::set(int cursorX)
{
	_cursor_x = cursorX;
}

int NotifyIconEventArgs::CursorY::get()
{
	return _cursor_y;
}

void NotifyIconEventArgs::CursorY::set(int cursorY)
{
	_cursor_y = cursorY;
}