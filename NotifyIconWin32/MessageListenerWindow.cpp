#include "MessageListenerWindow.h"
#include <string>

using namespace NotifyIcon::Win32;

// Constructor
MessageListenerWindow::MessageListenerWindow()
{
}

// Create the window to listen for events
bool MessageListenerWindow::CreateListenerWindow()
{
	const wchar_t class_name[] = L"NotifyIconListenerWindowClass";

	if (_window == nullptr)
	{
		// Get the handle to the application that is using the window
		HINSTANCE app_instance;
		if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, nullptr, &app_instance))
		{
			app_instance = nullptr;
		}

		// Get the id of the taskbar created message
		_taskbar_created_message_id = RegisterWindowMessage(L"TaskbarCreated");

		// Populate the window class
		WNDCLASSEXW wndclass = {};
		wndclass.hInstance = app_instance;
		wndclass.lpszClassName = class_name;
		wndclass.lpfnWndProc = &OnMessageReceived;

		// Register the window class, if it isn't already registered
		if (!GetClassInfoEx(app_instance, class_name, &wndclass))
		{
			RegisterClassEx(&wndclass);
		}

		// Create the window
		_window = CreateWindowEx(0, class_name, L"", 0, 0, 0, 1, 1, nullptr, nullptr, nullptr, nullptr);
	}

	return (_window != nullptr);
}

// Called when a message is received by the window
LRESULT CALLBACK MessageListenerWindow::OnMessageReceived(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Finally pass the message to the default window procedure
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// The handle to the window
HWND MessageListenerWindow::GetWindow()
{
	return _window;
}

// Destructor
MessageListenerWindow::~MessageListenerWindow()
{

}
