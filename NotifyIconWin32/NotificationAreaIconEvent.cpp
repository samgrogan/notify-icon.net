#include "NotificationAreaIconEvent.h"

using namespace NotifyIcon::Win32;

NotifyIconEventType NotifyIconEventArgs::Type::get()
{
	return _type;
}

void NotifyIconEventArgs::Type::set(NotifyIconEventType type)
{
	_type = type;
}
