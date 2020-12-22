#include "NotificationAreaIcon.h"

using namespace NotifyIcon::Win32;

// Constructor
NotificationAreaIcon::NotificationAreaIcon(String^ ItemGuid)
{

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
