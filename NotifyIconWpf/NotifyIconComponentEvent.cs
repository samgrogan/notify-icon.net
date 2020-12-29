using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NotifyIcon.Wpf
{
	// The *managed* version of the enum that describes an event in the notification icon window
	// This needs to stay in sync with the unmanaged version
	// The managed version cannot be used by the callback from the static window procedure, so both versions are needed
	public enum EventType : UInt32
	{
		Unknown = 0,
		Any = 1,
		SingleClick = 2,
		DoubleClick = 4,
		Select = 8,
		LeftButtonSingleClick = 16,
		LeftButtonDoubleClick = 32,
		MiddleButtonSingleClick = 64,
		MiddleButtonDoubleClick = 128,
		RightButtonSingleClick = 256,
		RightButtonDoubleClick = 512,
	};


}
