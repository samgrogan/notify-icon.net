#include "MessageListenerWindow.h"
#include <string>

using namespace NotifyIcon::Win32;

// Constructor
MessageListenerWindow::MessageListenerWindow()
{
}

// Register the window class, if needed
bool MessageListenerWindow::RegisterWindowClass()
{
	const wchar_t class_name[] = L"NotifyIconListenerWindowClass";

	// Get the handle to the application that is using the window
	HINSTANCE app_instance;
	if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, nullptr, &app_instance))
	{
		app_instance = nullptr;
	}

	// Populate the window class
	WNDCLASSEX wndclass = {};
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = app_instance;
	wndclass.lpszClassName = class_name;
	wndclass.lpfnWndProc = &OnMessageReceived;

	// Register the window class, if it isn't already registered
	if (!GetClassInfoEx(app_instance, MAKEINTATOM(_window_class), &wndclass))
	{
		_window_class = RegisterClassEx(&wndclass);
		if (_window_class == 0)
		{
			return false;
		}
	}

	return true;
}


// Create the window to listen for events
bool MessageListenerWindow::CreateListenerWindow()
{
	if (_window == nullptr)
	{
		if (!RegisterWindowClass())
		{
			return false;
		}

		// Get the id of the taskbar created message
		_taskbar_created_message_id = RegisterWindowMessage(L"TaskbarCreated");

		// Create the window
		_window = CreateWindowEx(0, MAKEINTATOM(_window_class), L"", 0, 0, 0, 1, 1, HWND_MESSAGE, nullptr, _app_instance, nullptr);
	}

	return (_window != nullptr);
}

// Called when a message is received by the window
LRESULT CALLBACK MessageListenerWindow::OnMessageReceived(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case CALLBACK_MESSAGE_ID:
		switch (LOWORD(lParam))
		{
			case WM_LBUTTONDOWN:

				break;
			case WM_LBUTTONUP:
				break;
			case WM_LBUTTONDBLCLK:
				break;
			case WM_RBUTTONDOWN:
				break;
			case WM_RBUTTONUP:
				break;
			case WM_RBUTTONDBLCLK:
				break;
			case WM_MBUTTONDOWN:
				break;
			case WM_MBUTTONUP:
				break;
			case WM_MBUTTONDBLCLK:
				break;
			case NIN_SELECT:
				break;
			case NIN_KEYSELECT:
				break;		
		}
		break;

	default:
		// Finally pass the message to the default window procedure
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

// The handle to the window
HWND MessageListenerWindow::GetWindow()
{
	return _window;
}

// Set the function to call when an even occurs in the notification icon area
void MessageListenerWindow::SetEventHandlerCallback(ProxyEventHandlerMethod eventHandlerMethod)
{
	_eventHandlerMethod = eventHandlerMethod;
}

// Destructor
MessageListenerWindow::~MessageListenerWindow()
{
	// if the window exists, destroy it
	if (_window != nullptr)
	{
		DestroyWindow(_window);
		_window = nullptr;
	}

	// Remove the window class, if it exists
	if (_window_class != 0)
	{
		UnregisterClass(MAKEINTATOM(_window_class), _app_instance);
		_window_class = 0;
	}
}
