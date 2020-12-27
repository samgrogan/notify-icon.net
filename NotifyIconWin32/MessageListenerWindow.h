#pragma once

#include <Windows.h>
#include <libloaderapi.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")

#include "NotificationAreaIconEvent.h"

using namespace System;

namespace NotifyIcon {
	namespace Win32 {

		// The id of the callback message from the notify icon
		const UINT CALLBACK_MESSAGE_ID = WM_APP + 1;

		typedef void (*ProxyEventHandlerMethod)(EventType eventType);

		class MessageListenerWindow
		{
		private:
			// The id of the message sent when the taskbar restarts
			UINT _taskbar_created_message_id = 0;

			// Handle to the app that contains the window/class
			HINSTANCE _app_instance = nullptr;

			// The unique identifier of the window class
			ATOM _window_class = 0;

			// The window handle
			HWND _window = nullptr;

			// The function to call when an event occurs in the notification icon area
			static ProxyEventHandlerMethod _eventHandlerMethod;

			// Register the window class, if needed
			bool RegisterWindowClass();

			// Called when a message is received by the window
			static LRESULT CALLBACK OnMessageReceived(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			// Pass a message on to the event handler, if any
			static void PassEventToHandler(EventType eventType);

		public:
			// Properties
			// Set the function to call when an even occurs in the notification icon area
			static void SetEventHandlerCallback(ProxyEventHandlerMethod eventHandlerMethod);

			// The handle to the window
			HWND GetWindow();

			// Create the window to listen for events
			bool CreateListenerWindow();

			// Constructor
			MessageListenerWindow();

			// Destructor
			~MessageListenerWindow();
		};
	}
}