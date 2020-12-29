#pragma once

using namespace System;

namespace NotifyIcon {
	namespace Win32 {

		public enum class NotifyIconEventType : UInt32 {
			Unknown,
			Any,
			SingleClick,
			DoubleClick,
			Select,
			LeftButtonSingleClick,
			LeftButtonDoubleClick,
			MiddleButtonSingleClick,
			MiddleButtonDoubleClick,
			RightButtonSingleClick,
			RightButtonDoubleClick,
		};

		public ref class NotifyIconEventArgs : EventArgs
		{
		private:
			NotifyIconEventType _type = NotifyIconEventType::Unknown;
		public:
			property NotifyIconEventType Type { NotifyIconEventType get(); void set(NotifyIconEventType value); }
		};

		public delegate void NotifyIconEventHandler(Object^ sender, NotifyIconEventArgs^ e);
	}
}

