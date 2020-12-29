using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NotifyIcon.Wpf
{
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
