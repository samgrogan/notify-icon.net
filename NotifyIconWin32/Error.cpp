#include "Error.h"

using namespace NotifyIcon::Win32;


// Construct an error based on GetLastError
Error::Error()
{
	// Get and format the error message
	_last_error = GetLastError();
}


// Gets the error code
DWORD Error::GetErrorCode()
{
	return _last_error;
}


// Gets the error message
String^ Error::GetErrorMessage()
{
	void* format_message_buffer = nullptr;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		_last_error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<wchar_t*>(&format_message_buffer),
		0, nullptr);

	// Convert to a string
	String^ error_message = gcnew String(static_cast<wchar_t*>(format_message_buffer));
	LocalFree(format_message_buffer);

	return error_message;
}


// Throws this error as a managed exception
void Error::ThrowAsException()
{
	throw ToException();
}


// Converts the error to a managed exception
Exception^ Error::ToException()
{
	return gcnew System::Exception(GetErrorCode() + L": " + GetErrorMessage());
}


Error::~Error()
{
}