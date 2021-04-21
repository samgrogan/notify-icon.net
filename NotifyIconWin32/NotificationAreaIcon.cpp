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

	// Store a reference to the dispatcher of the current thread
	_ui_dispatcher = Dispatcher::CurrentDispatcher;
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
		// Using just NIM_ADD can lead to issues if Explorer crashes, mitigated by also calling NIM_MODIFY
		_is_added = Shell_NotifyIcon(NIM_ADD, _icon_data) && Shell_NotifyIcon(NIM_MODIFY, _icon_data);
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
		const bool result = Shell_NotifyIcon(NIM_MODIFY, _icon_data);
		return result;
	}
	return false;
}

// Remove the icon from the notification area
bool NotificationAreaIcon::Delete()
{
	_is_added = false;
	const bool result = Shell_NotifyIcon(NIM_DELETE, _icon_data);	
	return result;
}

// Set the version flag
bool NotificationAreaIcon::SetVersion()
{
	const bool result = Shell_NotifyIcon(NIM_SETVERSION, _icon_data);
	return result;
}

// Set the focus to the notification icon
bool NotificationAreaIcon::SetFocus()
{
	const bool result = Shell_NotifyIcon(NIM_SETFOCUS, _icon_data);
	return result;
}

// Proxy events from the listener to the delegate
void NotificationAreaIcon::ProxyEventHandler(UINT uMsg, int cursorX, int cursorY)
{
	// Translate from unmanaged to managed event type
	NotifyIconEventArgs^ eventArgs = TranslateEvent(uMsg, cursorX, cursorY);

	// Stop any existing timers
	// Double-click event takes priority
	if (_click_event_timer != nullptr && !_handling_double_click)
	{
		delete _click_event_timer;
		_click_event_timer = nullptr;
	}
	
	if (eventArgs->Type == EventType::Select || eventArgs->Type == EventType::DoubleClick)
	{
		// Wait for a double click event to finish
		if (!_handling_double_click) {
			TimerCallback^ timer_callback = gcnew TimerCallback(this, &NotificationAreaIcon::OnDeliverEventTimer);
			_click_event_timer = gcnew Threading::Timer(timer_callback, eventArgs, 500, Timeout::Infinite);
			_handling_double_click = eventArgs->Type == EventType::DoubleClick;
		}
	}
	else
	{
		// Otherwise deliver the message now
		DeliverEvent(eventArgs);
	}
}

// Translates an event from the window procedure event type to a managed event type
NotifyIconEventArgs^ NotificationAreaIcon::TranslateEvent(UINT uMsg, int cursorX, int cursorY)
{
	NotifyIconEventArgs^ eventArgs = gcnew NotifyIconEventArgs();
	eventArgs->CursorX = cursorX;
	eventArgs->CursorY = cursorY;
	eventArgs->Type = EventType::Unknown;
	
	switch (uMsg)
	{
	case WM_LBUTTONDBLCLK:
		eventArgs->Type = EventType::DoubleClick;
		break;
	case NIN_SELECT:
	case NIN_KEYSELECT:
		eventArgs->Type = EventType::Select;
		break;
	case WM_CONTEXTMENU:
		eventArgs->Type = EventType::ContextMenu;
	default:
		break;
	}
	
	return eventArgs;
}

// Delivers the given event to the delegates, if any
void NotificationAreaIcon::DeliverEvent(NotifyIconEventArgs^ eventArgs)
{
	// Pass the event on to the registered handler
	// Must be invoked on the ui thread
	if (_ui_dispatcher != nullptr && NotificationIconEventHandler != nullptr)
	{
		_ui_dispatcher->Invoke(NotificationIconEventHandler, this, eventArgs);
	}
}

// Handle an event timer
void NotificationAreaIcon::OnDeliverEventTimer(Object^ source)
{
	NotifyIconEventArgs^ eventArgs = static_cast<NotifyIconEventArgs^>(source);
	DeliverEvent(eventArgs);

	// Mark double click as handled
	if (eventArgs->Type == EventType::DoubleClick) {
		_handling_double_click = false;
	}
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
		GC::KeepAlive((_managed_delegate));

		// Get a pointer to the delegate
		IntPtr managed_pointer = Marshal::GetFunctionPointerForDelegate(_managed_delegate);

		// Pass the pointer to the listener
		_message_listener->SetEventHandlerCallback(static_cast<ProxyEventHandlerMethod>(managed_pointer.ToPointer()));
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
	_icon_data->uCallbackMessage = MessageListenerWindow::CALLBACK_MESSAGE_ID;

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
	// Remove the timer, if any
	if (_click_event_timer != nullptr)
	{
		delete _click_event_timer;
		_click_event_timer = nullptr;
	}
	
	// Delete the icon
	Delete();

	// Remove the listener
	if (_message_listener != nullptr)
	{
		delete _message_listener;
		_message_listener = nullptr;
	}
}