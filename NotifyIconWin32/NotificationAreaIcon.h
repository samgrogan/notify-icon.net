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
using namespace System::ComponentModel;
using namespace System::Timers;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Threading;

namespace NotifyIcon {
	namespace Win32 {

		// Delegate type for the callback from unmanaged to managed code
		[UnmanagedFunctionPointerAttribute(CallingConvention::Cdecl)]
		delegate void ProxyEventHandlerDelegate(UINT uMsg, int cursorX, int cursorY);
		
		public ref class NotificationAreaIcon : IDisposable
		{
		private:

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

			// Timer for delivering click events to differentiate single and double clicks
			Threading::Timer^ _click_event_timer = nullptr;

			// Store the dispatcher for the UI thread to ensure events are raised on that thread
			Dispatcher^ _ui_dispatcher = nullptr;

			// Track when we are handling a double click
			bool _handling_double_click = false;
			
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
			void ProxyEventHandler(UINT uMsg, int cursorX, int cursorY);

			// Translates an event from the window procedure event type to a managed event type
			static NotifyIconEventArgs^ TranslateEvent(UINT uMsg, int cursorX, int cursorY);

			// Delivers the given event to the delegates, if any
			void DeliverEvent(NotifyIconEventArgs^ eventArgs);

			// Handle an event timer
			void OnDeliverEventTimer(Object^ source);
		
		public:
			// Properties
			// The tooltip to display when the cursor if over the notification icon
			property String^ ToolTip { void set(String^ toolTip); };

			// The icon to display in the notification area
			property IntPtr^ Icon { void set(IntPtr^ icon); }

			// Function to call when an event occurs in the notify icon
			NotifyIconEventHandler^ NotificationIconEventHandler = nullptr;

			// Constructor
			NotificationAreaIcon(Guid^ ItemGuid);

			// Show the notification area icon
			void ShowIcon();

			// Hide the notification area icon
			void HideIcon();

			// Used to periodically add/modify icon, used mitigate issues with Explorer crashing
			void RefreshIcon();

			// Set the foreground window to the given window
			// Used to show the context menu
			static bool SetForegroundWindow(IntPtr hwndWindow);

			// Destructor
			~NotificationAreaIcon();
		};
	}
}