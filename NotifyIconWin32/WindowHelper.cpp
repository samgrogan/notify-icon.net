#include "WindowHelper.h"

using namespace NotifyIcon::Win32;

void WindowHelper::ConvertScreenPointToDeviceIndependent(HWND hwnd, POINT& position)
{
	if (hwnd != nullptr)
	{
		// Get the DPI for the window
		UINT windowDpi = GetDpiForWindow(hwnd);
		if (windowDpi != 0)
		{
			// 96 dpi is equal to 100%
			position.x = (int)((float)position.x / ((float)windowDpi / 96.0F));
			position.y = (int)((float)position.y / ((float)windowDpi / 96.0F));
		}
	}
}
