#pragma once

#include <Windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")

using namespace System;

namespace NotifyIcon {
	namespace Win32 {

		public ref class NotificationAreaIcon
		{
		private:
			// The window that is used to receive events from the notification icon
			HWND _message_window;

			// The icon to display in the notification area
			HICON _icon;

			// The struct that describes this notification icon
			PNOTIFYICONDATA _icon_data;

			// Update the properties of the notification icon
			bool Modify();

			// Set the version flag
			bool SetVersion();

		public:
			// Properties
			// The tooltip to display when the cursor if over the notification icon
			property String^ ToolTip { void set(String^ toolTip); }

			// Constructor
			NotificationAreaIcon(Guid^ ItemGuid);

			// Add the icon to the notification area
			bool Add();

			// Remove the icon from the notification area
			bool Delete();

			// Set the focus to the notification icon
			bool SetFocus();

			// Destructor
			~NotificationAreaIcon();
		};
	}
}