#pragma once

#include <Windows.h>
#pragma comment(lib, "user32.lib")

namespace NotifyIcon {
	namespace Win32 {

		class MessageListenerWindow
		{
		private:
			// The window handle
			HWND _window;

			// The id of the message sent when the taskbar restarts
			UINT _taskbar_created_message_id = 0;

			// Called when a message is received by the window
			static LRESULT CALLBACK OnMessageReceived(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		public:
			// Properties

			// The handle to the window
			HWND GetWindow();

			// Create the window to listen for events
			void CreateListenerWindow();

			// Constructor
			MessageListenerWindow();

			// Destructor
			~MessageListenerWindow();
		};
	}
}