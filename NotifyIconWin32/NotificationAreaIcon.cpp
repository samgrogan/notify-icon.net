#include "NotificationAreaIcon.h"

using namespace NotifyIcon::Win32;

// Constructor
NotificationAreaIcon::NotificationAreaIcon(Guid^ ItemGuid)
{
	// Allocate the structure to hold the notify icon data
	_icon_data = new NOTIFYICONDATA;
	_icon_data->cbSize = sizeof(NOTIFYICONDATA);

	// Store the GUID
	array<Byte>^ guidData = ItemGuid->ToByteArray();
	pin_ptr<Byte> data = &(guidData[0]);

	_icon_data->guidItem = *(_GUID*)data;
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
