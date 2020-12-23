#include "MessageListenerWindow.h"
#include <string>

using namespace NotifyIcon::Win32;

// Constructor
MessageListenerWindow::MessageListenerWindow()
{
}

// Create the window to listen for events
void MessageListenerWindow::CreateListenerWindow()
{
	const wchar_t class_name[] = L"NotifyIconListenerWindowClass";

	// Populate the window class
	WNDCLASSW wndclass = {};
	wndclass.lpszClassName = class_name;
	wndclass.lpfnWndProc = &OnMessageReceived;

	// Register the window class
	RegisterClass(&wndclass);

	// Create the window
	_window = CreateWindowEx(0, class_name, L"", 0, 0, 0, 1, 1, nullptr, nullptr, nullptr, nullptr);

	// Get the id of the taskbar created message
	_taskbar_created_message_id = RegisterWindowMessage(L"TaskbarCreated");
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
