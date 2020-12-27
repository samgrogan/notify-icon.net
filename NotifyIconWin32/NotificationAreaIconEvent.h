#pragma once

using namespace System;

namespace NotifyIcon {
	namespace Win32 {

		public enum EventType {
			Unknown,
			LeftButtonSingleClick,
			LeftButtonDoubleClick,
			MiddleButtonSingleClick,
			MiddleButtonDoubleClick,
			RightButtonSingleClick,
			RightButtonDoubleClick,
			Select
		};

		public ref class NotifyIconEventArgs : EventArgs
		{
		private:
			EventType _type = Unknown;
		public:
			property EventType Type { EventType get(); void set(EventType value); }
		};

		public delegate void NotifyIconEventHandler(Object^ sender, NotifyIconEventArgs^ e);
	}
}

