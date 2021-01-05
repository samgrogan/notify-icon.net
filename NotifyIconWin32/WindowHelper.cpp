#include "WindowHelper.h"

using namespace NotifyIcon::Win32;

void WindowHelper::ConvertScreenPointToDeviceIndependent(HWND hwnd, POINT& position)
{
	if (hwnd != nullptr)
	{
		// Get the DPI for the window
		const UINT window_dpi = GetDpiForWindow(hwnd);
		if (window_dpi != 0)
		{
			// 96 dpi is equal to 100%
			position.x = static_cast<int>(static_cast<float>(position.x) / (static_cast<float>(window_dpi) / 96.0F));
			position.y = static_cast<int>(static_cast<float>(position.y) / (static_cast<float>(window_dpi) / 96.0F));
		}
	}
}
