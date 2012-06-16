#if defined __MY_UNIX__
#include <ShellSystemInterface.h>
#include <Shell.h>
#endif
#if defined __MY_WIN32__
#include "../include/ShellSystemInterface.h"
#include "../include/Shell.h"
#endif




// Get the number of seconds elapsed since the start of the application
float ShellSystemInterface::GetElapsedTime()
{
	return Shell::GetElapsedTime();
}
