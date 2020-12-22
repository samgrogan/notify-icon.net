#include "NotificationAreaIcon.h"

using namespace NotifyIcon::Win32;

// Constructor
NotificationAreaIcon::NotificationAreaIcon(Guid^ ItemGuid)
{
	// Create the listener window
	_message_listener = gcnew MessageListenerWindow();

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

}

// The tooltip to display when the cursor if over the notification icon
void NotificationAreaIcon::ToolTip::set(String^ toolTip)
{

}

// Initialize the icon data structure
void NotificationAreaIcon::InitializeIconData(Guid^ ItemGuid)
{
	if (_icon_data != nullptr)
	{
		// Allocate and initialize the structure to hold the notify icon data
		_icon_data = new NOTIFYICONDATA;
		memset(_icon_data, 0, sizeof(NOTIFYICONDATA));
		_icon_data->cbSize = sizeof(NOTIFYICONDATA);

		// Store the GUID
		array<Byte>^ guidData = ItemGuid->ToByteArray();
		pin_ptr<Byte> data = &(guidData[0]);
		_icon_data->guidItem = *(_GUID*)data;
	}
}
