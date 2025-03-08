#pragma once

using namespace System;

namespace NotifyIcon {
	namespace Win32 {

		// The *unmanaged* version of the enum that describes an event in the notification icon window
		public enum class EventType : UInt32 {
			Unknown = 0,
			Any = 1,
			Select = 2,
			DoubleClick = 4,
			ContextMenu = 8,
			SelectOrContextMenu = 16,
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

