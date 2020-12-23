#include "NotificationAreaIcon.h"

using namespace NotifyIcon::Win32;

// Constructor
NotificationAreaIcon::NotificationAreaIcon(Guid^ ItemGuid)
{
	// Create the listener window
	_message_listener = new MessageListenerWindow();
	if (!_message_listener->CreateListenerWindow()) 
	{
		Error^ error = gcnew Error();
		error->ThrowAsException();
	}

	// Initialize the data for the tray icon
	InitializeIconData(ItemGuid);
}

// Add the icon to the notification area
bool NotificationAreaIcon::Add()
{
	return false;
}

// Update the properties of the notification icon
bool NotificationAreaIcon::Modify()
{
	return false;
}

// Remove the icon from the notification area
bool NotificationAreaIcon::Delete()
{
	return false;
}

// Set the version flag
bool NotificationAreaIcon::SetVersion()
{
	return false;
}

// Set the focus to the notification icon
bool NotificationAreaIcon::SetFocus()
{
	return false;
}

// Destructor
NotificationAreaIcon::~NotificationAreaIcon()
{
	if (_message_listener != nullptr)
	{
		delete _message_listener;
		_message_listener = nullptr;
	}
}

// The tooltip to display when the cursor if over the notification icon
void NotificationAreaIcon::ToolTip::set(String^ toolTip)
{
	if (_icon_data != nullptr)
	{
		if (toolTip == nullptr) {
			wcsncpy_s(_icon_data->szTip, L"\0", 128);
		}
		else {
			pin_ptr<const wchar_t> _device_id_ptr = PtrToStringChars(toolTip);
			wcsncpy_s(_icon_data->szTip, _device_id_ptr, 128);
		}
	}
}

// The icon to display in the notification area
void NotificationAreaIcon::Icon::set(HICON icon)
{

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

	// Store the GUID
	array<Byte>^ guidData = ItemGuid->ToByteArray();
	pin_ptr<Byte> data = &(guidData[0]);
	_icon_data->guidItem = *(_GUID*)data;

	// Attach the listener window
	_icon_data->hWnd = _message_listener->GetWindow();

	// Set the version
	// Configure for Vista or later
	_icon_data->uVersion = NOTIFYICON_VERSION_4;

	// Initially hidden
	_icon_data->dwState = NIS_HIDDEN;
	_icon_data->dwStateMask = NIS_HIDDEN;
}
