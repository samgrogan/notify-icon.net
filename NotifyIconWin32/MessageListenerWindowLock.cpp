#include "MessageListenerWindowLock.h"

using namespace System;
using namespace NotifyIcon::Win32;

Object^ MessageListenerWindowLock::GetLockObject()
{
	return _lock_object;
}