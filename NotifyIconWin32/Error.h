#pragma once

//#include <string>
//#include <iostream>

#include <Windows.h>

using namespace System;

namespace NotifyIcon
{
	namespace Win32
	{
		public ref class Error
		{
		private:
			DWORD _last_error;

		public:
			// Construct an error based on GetLastError
			Error();

			// Gets the error code
			DWORD GetErrorCode();

			// Gets the error message
			String^ GetErrorMessage();

			// Throws this error as a managed exception
			void ThrowAsException();

			// Converts the error to a managed exception
			Exception^ ToException();

			virtual ~Error();
		};
	}
}
