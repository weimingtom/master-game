#include <ShellSystemInterface.h>
#include <Shell.h>

// Get the number of seconds elapsed since the start of the application
float ShellSystemInterface::GetElapsedTime()
{
	return Shell::GetElapsedTime();
}
