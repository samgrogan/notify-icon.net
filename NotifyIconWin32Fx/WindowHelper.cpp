#include "WindowHelper.h"

using namespace NotifyIcon::Win32;

void WindowHelper::ConvertScreenPointToDeviceIndependent(HWND hwnd, POINT& position)
{
	if (hwnd != nullptr)
	{
		// Get the DPI for the window
		//const UINT window_dpi = GetDpiForWindow(hwnd);
		const HDC hdc = GetDC(hwnd);
		if (hdc != nullptr)
		{
			const UINT dpi_x = GetDeviceCaps(hdc, LOGPIXELSX);
			const UINT dpi_y = GetDeviceCaps(hdc, LOGPIXELSY);

			if (dpi_x != 0 && dpi_y != 0)
			{
				// 96 dpi is equal to 100%
				position.x = static_cast<int>(static_cast<float>(position.x) / (static_cast<float>(dpi_x) / 96.0F));
				position.y = static_cast<int>(static_cast<float>(position.y) / (static_cast<float>(dpi_y) / 96.0F));
			}
		}
	}
}
