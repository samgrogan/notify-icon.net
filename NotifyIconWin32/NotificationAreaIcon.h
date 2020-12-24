#pragma once

#pragma comment(lib, "shell32.lib")

#include <Windows.h>
#include <shellapi.h>
#include <vcclr.h>
#include <cwchar>

#include "Error.h"
#include "MessageListenerWindow.h"

using namespace System;

namespace NotifyIcon {
	namespace Win32 {

		public ref class NotificationAreaIcon
		{
		private:
			// The window that is used to receive events from the notification icon
			MessageListenerWindow* _message_listener = nullptr;

			// The icon to display in the notification area
			HICON _icon = nullptr;

			// The struct that describes this notification icon
			PNOTIFYICONDATA _icon_data = nullptr;

			// Add the icon to the notification area
			bool Add();

			// Remove the icon from the notification area
			bool Delete();

			// Set the focus to the notification icon
			bool SetFocus();

			// Update the properties of the notification icon
			bool Modify();

			// Set the version flag
			bool SetVersion();

			// Initialize the icon data structure
			void InitializeIconData(Guid^ ItemGuid);

		public:
			// Properties
			// The tooltip to display when the cursor if over the notification icon
			property String^ ToolTip { void set(String^ toolTip); }

			// The icon to display in the notification area
			property IntPtr^ Icon { void set(IntPtr^ icon); }

			// Constructor
			NotificationAreaIcon(Guid^ ItemGuid);

			// Show the notification area icon
			void ShowIcon();

			// Hide the notification area icon
			void HideIcon();

			// Destructor
			~NotificationAreaIcon();
		};
	}
}