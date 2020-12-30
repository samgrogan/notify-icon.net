#pragma once

using namespace System;

namespace NotifyIcon {
	namespace Win32 {

		// The *unmanaged* version of the enum that describes an event in the notification icon window
		public enum EventType : UInt32 {
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

		public ref class NotifyIconEventArgs : EventArgs
		{
		private:
			EventType _type = EventType::Unknown;
			int _cursor_x = -1;
			int _cursor_y = -1;
		public:
			property EventType Type { EventType get(); void set(EventType value); }
			property int CursorX { int get(); void set(int cursorX); }
			property int CursorY { int get(); void set(int cursorY); }
		};

		public delegate void NotifyIconEventHandler(Object^ sender, NotifyIconEventArgs^ e);
	}
}

