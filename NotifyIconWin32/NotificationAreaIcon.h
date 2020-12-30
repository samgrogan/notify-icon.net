#pragma once

#pragma comment(lib, "shell32.lib")

#include <Windows.h>
#include <shellapi.h>
#include <vcclr.h>
#include <cwchar>

#include "Error.h"
#include "MessageListenerWindow.h"
#include "MessageListenerWindowLock.h"
#include "NotificationAreaIconEvent.h"

using namespace System;

namespace NotifyIcon {
	namespace Win32 {
		public ref class NotificationAreaIcon : IDisposable
		{
		private:
			// Delegate type for the callback from unmanaged to managed code
			delegate void ProxyEventHandlerDelegate(EventType eventType, int cursorX, int cursorY);

			// The window that is used to receive events from the notification icon
			MessageListenerWindow* _message_listener = nullptr;

			// Delegate to the managed method to call when an even occurs;
			ProxyEventHandlerDelegate^ _managed_delegate = nullptr;

			// The icon to display in the notification area
			HICON _icon = nullptr;

			// The struct that describes this notification icon
			PNOTIFYICONDATA _icon_data = nullptr;

			// Has the notification area icon been added?
			bool _is_added = false;

			// Add the icon to the notification area
			bool AddOrModify();

			// Update the properties of the notification icon
			bool Modify();

			// Remove the icon from the notification area
			bool Delete();

			// Set the focus to the notification icon
			bool SetFocus();

			// Set the version flag
			bool SetVersion();

			// Initialize the message listener window
			void InitializeListenerWindow();

			// Initialize the proxy event handler
			void InitializeProxyEventHandler();

			// Initialize the icon data structure
			void InitializeIconData(Guid^ ItemGuid);

			// Proxy events from the listener to the delegate
			void ProxyEventHandler(EventType eventType, int cursorX, int cursorY);


		public:
			// Properties
			// The tooltip to display when the cursor if over the notification icon
			property String^ ToolTip { void set(String^ toolTip); }

			// The icon to display in the notification area
			property IntPtr^ Icon { void set(IntPtr^ icon); }

			// Function to call when an event occurs in the notify icon
			event NotifyIconEventHandler^ NotificationIconEventHandler;

			// Constructor
			NotificationAreaIcon(Guid^ ItemGuid);

			// Show the notification area icon
			void ShowIcon();

			// Hide the notification area icon
			void HideIcon();

			// Set the foreground window to the given window
			// Used to show the context menu
			bool SetForegroundWindow(IntPtr hwndWindow);

			// Destructor
			~NotificationAreaIcon();
		};
	}
}