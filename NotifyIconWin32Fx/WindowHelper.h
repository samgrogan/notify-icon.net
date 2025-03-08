#pragma once

#pragma comment(lib, "gdi32.lib")

#include <Windows.h>
#include <wingdi.h>

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


