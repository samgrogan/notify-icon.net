#pragma once

namespace NotifyIcon
{
	namespace Win32
	{
		ref class MessageListenerWindowLock abstract sealed
		{
		private:
			static Object^ _lock_object = gcnew Object();
		public:
			static Object^ GetLockObject();
		};
	}
}



