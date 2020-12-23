#include "MessageListenerWindow.h"

using namespace NotifyIcon::Win32;

// Constructor
MessageListenerWindow::MessageListenerWindow()
{
	CreateListenerWindow();
}

// Create the window to listen for events
void MessageListenerWindow::CreateListenerWindow()
{
	// Populate the window class
	WNDCLASSW wndclass = {};
	wndclass.lpszClassName = L"NotifyIconListenerWindowClass";
	wndclass.lpfnWndProc = (WNDPROC)OnMessageReceived;

	// Register the window class
	RegisterClass(&wndclass);

	// Create the window
	_window = CreateWindowEx(0, wndclass.lpszClassName, L"", 0, 0, 0, 1, 1, nullptr, nullptr, nullptr, nullptr);

	// Get the id of the taskbar created message
	_taskbar_created_message_id = RegisterWindowMessage(L"TaskbarCreated");
}

// Called when a message is received by the window
LRESULT MessageListenerWindow::OnMessageReceived(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Finally pass the message to the default window procedure
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// The handle to the window
HWND MessageListenerWindow::Window::get()
{
	return _window;
}
