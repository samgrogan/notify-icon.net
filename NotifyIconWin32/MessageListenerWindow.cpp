#include "MessageListenerWindow.h"

using namespace NotifyIcon::Win32;

// The unique identifier of the window class
// Shared between all instances of the class
ATOM MessageListenerWindow::_window_class = 0;

// How many windows are using the window clas
int MessageListenerWindow::_window_class_count = 0;

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
	// if (!GetClassInfoEx(app_instance, MAKEINTATOM(_window_class.load()), &wndclass))
	if (_window_class == 0)
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
		if (_window != nullptr)
		{
			_window_class_count++;
		}
	}

	return (_window != nullptr);
}

// Called when a message is received by the window
LRESULT CALLBACK NotifyIcon::Win32::OnMessageReceived(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	// Pass on the events to the class instance that owns the window
	switch (uMsg)
	{
	case CALLBACK_MESSAGE_ID:
		switch (LOWORD(lParam))
		{
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			MessageListenerWindow::ForwardWindowEventToHandler(ptr_this, hwnd, EventType::LeftButtonSingleClick);
			break;
		case WM_LBUTTONDBLCLK:
			MessageListenerWindow::ForwardWindowEventToHandler(ptr_this, hwnd, EventType::LeftButtonDoubleClick);
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			MessageListenerWindow::ForwardWindowEventToHandler(ptr_this, hwnd, EventType::RightButtonSingleClick);
			break;
		case WM_RBUTTONDBLCLK:
			MessageListenerWindow::ForwardWindowEventToHandler(ptr_this, hwnd, EventType::RightButtonDoubleClick);
			break;
		case WM_MBUTTONDOWN:
			break;
		case WM_MBUTTONUP:
			MessageListenerWindow::ForwardWindowEventToHandler(ptr_this, hwnd, EventType::MiddleButtonSingleClick);
			break;
		case WM_MBUTTONDBLCLK:
			MessageListenerWindow::ForwardWindowEventToHandler(ptr_this, hwnd, EventType::MiddleButtonDoubleClick);
			break;
		case NIN_SELECT:
			MessageListenerWindow::ForwardWindowEventToHandler(ptr_this, hwnd, EventType::Select);
			break;
		case NIN_KEYSELECT:
			MessageListenerWindow::ForwardWindowEventToHandler(ptr_this, hwnd, EventType::Select);
			break;
		}
		break;
	}

	// Finally pass the message to the default window procedure
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void MessageListenerWindow::ForwardWindowEventToHandler(MessageListenerWindow* ptrThis, HWND hwnd, EventType eventType)
{
	if (ptrThis != nullptr)
	{
		// Get the coordinates when the event occured
		POINT cursor_position = { -1, -1 };
		GetPhysicalCursorPos(&cursor_position);
		// Convert to device independent coordinates
		WindowHelper::ConvertScreenPointToDeviceIndependent(hwnd, cursor_position);

		// Pass to the instance handler
		ptrThis->ForwardWindowEventToHandler(eventType, cursor_position.x, cursor_position.y);
	}
}

// The handle to the window
HWND MessageListenerWindow::GetWindow()
{
	return _window;
}

// Set the function to call when an even occurs in the notification icon area
void MessageListenerWindow::SetEventHandlerCallback(ProxyEventHandlerMethod eventHandlerMethod)
{
	if (_window == nullptr)
	{
		_eventHandlerMethod = eventHandlerMethod;
	}
	else
	{
		// Can't change the event handler after the window is created
		throw gcnew System::Exception("Can't set Notify Icon Listener Window event handler after window is created.");
	}
}

// Called to pass an event on to the handler, if registered
void MessageListenerWindow::ForwardWindowEventToHandler(EventType eventType, int cursorX, int cursorY)
{
	if (_eventHandlerMethod != nullptr)
	{
		_eventHandlerMethod(eventType, cursorX, cursorY);
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
		_window_class_count--;
	}

	// Remove the window class, if it exists, but only if no other windows are using it
	if (_window_class != 0 && _window_class_count == 0)
	{
		UnregisterClass(MAKEINTATOM(_window_class), _app_instance);
		_window_class = 0;
	}
}
