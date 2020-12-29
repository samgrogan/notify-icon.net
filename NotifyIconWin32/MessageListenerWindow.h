#pragma once

#include <Windows.h>
#include <libloaderapi.h>
#include <string>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")

#include "NotificationAreaIconEvent.h"

using namespace System;
using namespace System::Threading;

namespace NotifyIcon {
	namespace Win32 {

		// The id of the callback message from the notify icon
		const UINT CALLBACK_MESSAGE_ID = WM_APP + 1;

		typedef void (*ProxyEventHandlerMethod)(EventType eventType);

		// Called when a message is received by the window
		static LRESULT CALLBACK OnMessageReceived(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		class MessageListenerWindow
		{
		private:
			// The id of the message sent when the taskbar restarts
			UINT _taskbar_created_message_id = 0;

			// Handle to the app that contains the window/class
			HINSTANCE _app_instance = nullptr;

			// The unique identifier of the window class
			// Shared between all instances of the class
			static ATOM _window_class;

			// How many windows are using the window clas
			static int _window_class_count;

			// The window handle
			HWND _window = nullptr;

			// The function to call when an event occurs in the notification icon area
			ProxyEventHandlerMethod _eventHandlerMethod = nullptr;

			// Register the window class, if needed
			bool RegisterWindowClass();

		public:
			// Properties
			// Set the function to call when an even occurs in the notification icon area
			void SetEventHandlerCallback(ProxyEventHandlerMethod eventHandlerMethod);

			// The handle to the window
			HWND GetWindow();

			// Create the window to listen for events
			bool CreateListenerWindow();

			// Called to pass an event on to the handler, if registered
			void PassWindowEventToHandler(EventType eventType);

			// Constructor
			MessageListenerWindow();

			// Destructor
			~MessageListenerWindow();
		};
	}
}