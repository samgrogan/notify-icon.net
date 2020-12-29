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
