#pragma once

#include <Windows.h>
#include <WinUser.h>

namespace NotifyIcon
{
	namespace Win32
	{
		public class WindowHelper
		{
		public:
			static void ConvertScreenPointToDeviceIndependent(HWND hwnd, POINT& position);
		};
	}
}


