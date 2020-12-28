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
		// Pass a pointer to the class instance so that the static window function can communicate back to the instance
		_window = CreateWindowEx(0, MAKEINTATOM(_window_class), L"", 0, 0, 0, 1, 1, HWND_MESSAGE, nullptr, _app_instance, reinterpret_cast<LPVOID>(this));
	}

	return (_window != nullptr);
}

// Called when a message is received by the window
LRESULT CALLBACK MessageListenerWindow::OnMessageReceived(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Get or set the pointer to "this" (the instance for the current message)
	// See: https://docs.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-?redirectedfrom=MSDN

	MessageListenerWindow* ptr_this = nullptr;
	if (uMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		if (create_struct != nullptr)
		{
			ptr_this = reinterpret_cast<MessageListenerWindow*>(create_struct->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr_this));
		}
	}
	else
	{
		ptr_this = reinterpret_cast<MessageListenerWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	// If we got a pointer to the owning instance, pass on the events
	if (ptr_this != nullptr)
	{
		switch (uMsg)
		{
		case CALLBACK_MESSAGE_ID:
			switch (LOWORD(lParam))
			{
			case WM_LBUTTONDOWN:
				break;
			case WM_LBUTTONUP:
				ptr_this->PassEventToHandler(LeftButtonSingleClick);
				break;
			case WM_LBUTTONDBLCLK:
				ptr_this->PassEventToHandler(LeftButtonDoubleClick);
				break;
			case WM_RBUTTONDOWN:
				break;
			case WM_RBUTTONUP:
				ptr_this->PassEventToHandler(RightButtonSingleClick);
				break;
			case WM_RBUTTONDBLCLK:
				ptr_this->PassEventToHandler(RightButtonDoubleClick);
				break;
			case WM_MBUTTONDOWN:
				break;
			case WM_MBUTTONUP:
				ptr_this->PassEventToHandler(MiddleButtonSingleClick);
				break;
			case WM_MBUTTONDBLCLK:
				ptr_this->PassEventToHandler(MiddleButtonDoubleClick);
				break;
			case NIN_SELECT:
				ptr_this->PassEventToHandler(Select);
				break;
			case NIN_KEYSELECT:
				ptr_this->PassEventToHandler(Select);
				break;
			}
			break;
		}
	}

	// Finally pass the message to the default window procedure
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
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

// Pass a message on to the event handler, if any
void MessageListenerWindow::PassEventToHandler(EventType eventType)
{
	if (_eventHandlerMethod != nullptr)
	{
		_eventHandlerMethod(eventType);
	}
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
