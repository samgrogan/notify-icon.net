#include "NotificationAreaIcon.h"

using namespace System::Runtime::InteropServices;
using namespace NotifyIcon::Win32;

// Constructor
NotificationAreaIcon::NotificationAreaIcon(Guid^ ItemGuid)
{
	// Create and initialize the listener window to capture events
	InitializeListenerWindow();

	// Initialize the data for the tray icon
	InitializeIconData(ItemGuid);
}

// Show the notification area icon
void NotificationAreaIcon::ShowIcon()
{
	// Update the visibility
	_icon_data->dwState = 0;

	// Add or modify the icon
	AddOrModify();

	// Set the version
	SetVersion();
}

// Hide the notification area icon
void NotificationAreaIcon::HideIcon()
{
	// Update the visibility
	_icon_data->dwState = NIS_HIDDEN;

	// Add or modify the icon
	AddOrModify();

	// Set the version
	SetVersion();
}

// Set the foreground window to the given window
// Used to show the context menu
bool NotificationAreaIcon::SetForegroundWindow(IntPtr hwndWindow)
{
	return ::SetForegroundWindow(static_cast<HWND>(hwndWindow.ToPointer()));
}

// Add the icon to the notification area
bool NotificationAreaIcon::AddOrModify()
{
	if (!_is_added)
	{
		// Delete the icon, in case a previous version exists
		Delete();
		// Now try to add the new icon
		_is_added = Shell_NotifyIcon(NIM_ADD, _icon_data);
		if (!_is_added) {
			Error^ error = gcnew Error();
			error->ThrowAsException();
		}
	}
	else
	{
		return Modify();
	}
	return _is_added;
}

// Update the properties of the notification icon
bool NotificationAreaIcon::Modify()
{
	if (_is_added)
	{
		return Shell_NotifyIcon(NIM_MODIFY, _icon_data);
	}
	return false;
}

// Remove the icon from the notification area
bool NotificationAreaIcon::Delete()
{
	Shell_NotifyIcon(NIM_DELETE, _icon_data);
	_is_added = false;

	return true;
}

// Set the version flag
bool NotificationAreaIcon::SetVersion()
{
	return Shell_NotifyIcon(NIM_SETVERSION, _icon_data);
}

// Set the focus to the notification icon
bool NotificationAreaIcon::SetFocus()
{
	return Shell_NotifyIcon(NIM_SETFOCUS, _icon_data);;
}

// Proxy events from the listener to the delegate
void NotificationAreaIcon::ProxyEventHandler(EventType eventType, int cursorX, int cursorY)
{
	NotifyIconEventArgs^ eventArgs = gcnew NotifyIconEventArgs();
	eventArgs->Type = eventType;
	eventArgs->CursorX = cursorX;
	eventArgs->CursorY = cursorY;

	// Pass the event on to the registered handler(s)
	NotificationIconEventHandler(this, eventArgs);
}

// The tooltip to display when the cursor if over the notification icon
void NotificationAreaIcon::ToolTip::set(String^ toolTip)
{
	if (_icon_data != nullptr)
	{
		if (toolTip == nullptr) {
			wcscpy_s(_icon_data->szTip, L"\0");
		}
		else {
			// Make sure the string isn't too long
			String^ short_tool_tip = toolTip->Substring(0, min(127, toolTip->Length));

			// Get a pointer to the characters in the managed string
			const pin_ptr<const wchar_t> tool_tip_ptr = PtrToStringChars(short_tool_tip);

			// Copy to the buffer
			wcscpy_s(_icon_data->szTip, tool_tip_ptr);
		}
		_icon_data->uFlags |= NIF_TIP;

		// If already visible, update with the changes
		Modify();
	}
}

// The icon to display in the notification area
void NotificationAreaIcon::Icon::set(IntPtr^ hicon)
{
	if (hicon != nullptr)
	{
		_icon = static_cast<HICON>(hicon->ToPointer());
		if (_icon_data != nullptr)
		{
			_icon_data->hIcon = _icon;
			_icon_data->uFlags |= NIF_ICON;

			// If already visible, update with the changes
			Modify();
		}
	}
}

// Initialize the message listener window
void NotificationAreaIcon::InitializeListenerWindow()
{
	// Create the listener window
	_message_listener = new MessageListenerWindow();

	// Attach the event handler proxy
	InitializeProxyEventHandler();

	// Create the listener window
	Monitor::Enter(MessageListenerWindowLock::GetLockObject());
	bool result;
	try {
		result = _message_listener->CreateListenerWindow();
	}
	finally
	{
		Monitor::Exit(MessageListenerWindowLock::GetLockObject());
	}

	if (!result)
	{
		Error^ error = gcnew Error();
		error->ThrowAsException();
	}
}

// Initialize the proxy event handler
void NotificationAreaIcon::InitializeProxyEventHandler()
{
	if (_message_listener != nullptr && _managed_delegate == nullptr)
	{
		// Create the delegate to the managed method
		_managed_delegate = gcnew ProxyEventHandlerDelegate(this, &NotificationAreaIcon::ProxyEventHandler);
		
		// Protect the delegate from garbage collection
		_managed_delegate_gchandle = GCHandle::Alloc(_managed_delegate);

		// Get a pointer to the delegate
		IntPtr managedPointer = Marshal::GetFunctionPointerForDelegate(_managed_delegate);

		// Pass the pointer to the listener
		_message_listener->SetEventHandlerCallback(static_cast<ProxyEventHandlerMethod>(managedPointer.ToPointer()));
	}
}


// Initialize the icon data structure
void NotificationAreaIcon::InitializeIconData(Guid^ ItemGuid)
{
	if (_icon_data == nullptr)
	{
		// Allocate and initialize the structure to hold the notify icon data
		_icon_data = new NOTIFYICONDATA;
		memset(_icon_data, 0, sizeof(NOTIFYICONDATA));
		_icon_data->cbSize = sizeof(NOTIFYICONDATA);
	}

	// Set the id of the messages from the notification icon
	_icon_data->uCallbackMessage = CALLBACK_MESSAGE_ID;

	// Store the GUID
	array<Byte>^ guidData = ItemGuid->ToByteArray();
	const pin_ptr<Byte> data = &(guidData[0]);
	_icon_data->guidItem = *reinterpret_cast<_GUID*>(data);

	// Attach the listener window
	_icon_data->hWnd = _message_listener->GetWindow();

	// Set the version
	// Configure for Vista or later
	_icon_data->uVersion = NOTIFYICON_VERSION_4;

	// Initially hidden
	_icon_data->dwState = NIS_HIDDEN;
	_icon_data->dwStateMask = NIS_HIDDEN;

	// Configure the flags for the data to pass to the notify function
	_icon_data->uFlags = NIF_MESSAGE | NIF_STATE | NIF_GUID | NIF_SHOWTIP;
}

// Destructor
NotificationAreaIcon::~NotificationAreaIcon()
{
	// Delete the icon
	Delete();

	// Remove the listener
	if (_message_listener != nullptr)
	{
		delete _message_listener;
		_message_listener = nullptr;
	}

	// Free the gc handle
	if (_managed_delegate_gchandle != nullptr)
	{
		_managed_delegate_gchandle->Free();
		_managed_delegate_gchandle = nullptr;
	}
}