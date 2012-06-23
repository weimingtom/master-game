#include <Shell.h>
#include <Rocket/Core.h>
#include <win32/InputWin32.h>
#include "ShellFileInterface.h"
#include <windows.h>
#include <stdio.h>

#include "../../../config.h"


static bool AttachOpenGL();
static void DetachOpenGL();
static LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

static bool activated = true;
static bool running = false;
static bool opengl_attached = false;
static const char* instance_name = NULL;
static HWND window_handle = NULL;
static HINSTANCE instance_handle = NULL;
static HDC device_context = NULL;
static HGLRC render_context = NULL;

static double time_frequency;
static LARGE_INTEGER time_startup;

static ShellFileInterface* file_interface = NULL;


void SysSetDisplayMode (int width, int height, int depth)
{
	DEVMODE dmode;

	memset(&dmode, 0, sizeof(DEVMODE));
	dmode.dmSize=sizeof(DEVMODE);
	dmode.dmPelsWidth = width;
	dmode.dmPelsHeight = height;
	dmode.dmBitsPerPel = depth;
	dmode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL;
	//dmode.dmDisplayFrequency =
    long res=ChangeDisplaySettings(&dmode, CDS_FULLSCREEN);
	// change resolution, if possible
	if (res!= DISP_CHANGE_SUCCESSFUL)
	{

	    printf("%i",res);
		// if not... failed to change resolution

		// this prevents SysShutdown from changing resolution back
		//screenmode = 0;

		MessageBox(NULL, "Your system doesn't support this screen resolution", "Display Error", MB_OK);
		//SysShutdown();
	}
}

bool Shell::Initialise(const Rocket::Core::String& path)
{
	instance_handle = GetModuleHandle(NULL);
	InputWin32::Initialise();

	LARGE_INTEGER time_ticks_per_second;
	QueryPerformanceFrequency(&time_ticks_per_second);
	QueryPerformanceCounter(&time_startup);

	time_frequency = 1.0 / (double) time_ticks_per_second.QuadPart;

	// Fetch the path of the executable, append the path onto that.
	char executable_file_name[MAX_PATH];
	if (GetModuleFileNameA(instance_handle, executable_file_name, MAX_PATH) >= MAX_PATH &&
		GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		executable_file_name[0] = 0;

	executable_path = Rocket::Core::String(executable_file_name);
	executable_path = executable_path.Substring(0, executable_path.RFind("\\") + 1);
	file_interface = new ShellFileInterface(executable_path + path);
	Rocket::Core::SetFileInterface(file_interface);

	return true;
}

void Shell::Shutdown()
{
	InputWin32::Shutdown();

	delete file_interface;
	file_interface = NULL;
}

bool Shell::OpenWindow(const char* name, bool attach_opengl)
{
	WNDCLASS window_class;

	// Fill out the window class struct.
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = WindowProcedure;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance_handle;
	window_class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = NULL;
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = name;

	if (!RegisterClass(&window_class))
	{
		DisplayError("Could not register window class.");

		CloseWindow();
		return false;
	}



	    window_handle = CreateWindowEx(WS_EX_TOPMOST,
                  name,
                  name,
                  WS_POPUP,
                  0, 0,
                  screen_width, screen_height,
                  NULL,
                  NULL,
                  instance_handle,
                  NULL);

        //SetWindowLong(window_handle, GWL_STYLE, GetWindowLong(window_handle, GWL_STYLE) &~ WS_CAPTION);
        //SetWindowPos(window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

	/*window_handle = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
								   name,	// Window class name.
								   name,
								   WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
								   0, 0,	// Window position.
								   0, 0,	// Window size.
								   NULL,
								   NULL,
								   instance_handle,
								   NULL);
								   */

	if (!window_handle)
	{
		DisplayError("Could not create window.");
		CloseWindow();

		return false;
	}

	instance_name = name;

    // Fullscreen without caption or windowed mode
    DWORD style;
    if(fullscreen)
    {
        style = WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_CAPTION;
        SysSetDisplayMode(screen_width, screen_height, 32);
    }
    else
        style = WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX;

	DWORD extended_style = WS_EX_APPWINDOW;

	//DWORD style = WS_POPUP& WS_CAPTION;
	//DWORD extended_style = WS_EX_TOPMOST;

	// Adjust the window size to take into account the edges
	RECT window_rect;
	window_rect.top = 0;
	window_rect.left = 0;
	window_rect.right = screen_width;
	window_rect.bottom = screen_height;
	AdjustWindowRectEx(&window_rect, style, FALSE, extended_style);

	SetWindowLong(window_handle, GWL_EXSTYLE, extended_style);
	SetWindowLong(window_handle, GWL_STYLE, style);

	// Resize the window.
	SetWindowPos(window_handle, HWND_TOP, 0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, SWP_NOACTIVATE);

	// Display the new window
	ShowWindow(window_handle, SW_SHOW);
	SetForegroundWindow(window_handle);
	SetFocus(window_handle);

	// Attach OpenGL if necessary
	if (attach_opengl)
	{
		opengl_attached = AttachOpenGL();
		if (!opengl_attached)
			return false;
	}

    return true;
}

void Shell::CloseWindow()
{
	// Shutdown opengl if necessary
	if (opengl_attached)
	{
		DetachOpenGL();
		opengl_attached = false;
	}

	DestroyWindow(window_handle);
	UnregisterClass(instance_name, instance_handle);
	ChangeDisplaySettings(NULL, 0);
}

// Returns a platform-dependent handle to the window.
void* Shell::GetWindowHandle()
{
	return window_handle;
}

// Flips the OpenGL buffers.
void Shell::FlipBuffers()
{
	SwapBuffers(device_context);
}

void Shell::EventLoop(ShellIdleFunction idle_function)
{
	MSG message;
	running = true;

	// Loop on PeekMessage() / GetMessage() until exit has been requested.
	while (running)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
		{
			GetMessage(&message, NULL, 0, 0);

			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		idle_function();

		if (!activated)
			Sleep(100);
	}
}

void Shell::RequestExit()
{
	running = false;
}

void Shell::DisplayError(const char* fmt, ...)
{
	const int buffer_size = 1024;
	char buffer[buffer_size];
	va_list argument_list;

	// Print the message to the buffer.
	va_start(argument_list, fmt);
	int len = vsnprintf(buffer, buffer_size - 2, fmt, argument_list);
	if ( len < 0 || len > buffer_size - 2 )
	{
		len = buffer_size - 2;
	}
	buffer[len] = '\n';
	buffer[len + 1] = '\0';
	va_end(argument_list);

	MessageBox(window_handle, buffer, "Shell Error", MB_OK);
}

void Shell::Log(const char* fmt, ...)
{
	const int buffer_size = 1024;
	char buffer[buffer_size];
	va_list argument_list;

	// Print the message to the buffer.
	va_start(argument_list, fmt);
	int len = vsnprintf(buffer, buffer_size - 2, fmt, argument_list);
	if ( len < 0 || len > buffer_size - 2 )
	{
		len = buffer_size - 2;
	}
	buffer[len] = '\n';
	buffer[len + 1] = '\0';
	va_end(argument_list);

	OutputDebugString(buffer);
}

float Shell::GetElapsedTime()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	return (float)((counter.QuadPart - time_startup.QuadPart) * time_frequency);
}

static bool AttachOpenGL()
{
	device_context = GetDC(window_handle);
	if (device_context == NULL)
	{
		Shell::DisplayError("Could not get device context.");
		return false;
	}

	PIXELFORMATDESCRIPTOR pixel_format_descriptor;
	memset(&pixel_format_descriptor, 0, sizeof(pixel_format_descriptor));
	pixel_format_descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixel_format_descriptor.nVersion = 1;
	pixel_format_descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixel_format_descriptor.iPixelType = PFD_TYPE_RGBA;
	pixel_format_descriptor.cColorBits = 32;
	pixel_format_descriptor.cRedBits = 8;
	pixel_format_descriptor.cGreenBits = 8;
	pixel_format_descriptor.cBlueBits = 8;
	pixel_format_descriptor.cAlphaBits = 8;
	pixel_format_descriptor.cDepthBits = 24;
	pixel_format_descriptor.cStencilBits = 8;

	int pixel_format = ChoosePixelFormat(device_context, &pixel_format_descriptor);
	if (pixel_format == 0)
	{
		Shell::DisplayError("Could not choose 32-bit pixel format.");
		return false;
	}

	if (SetPixelFormat(device_context, pixel_format, &pixel_format_descriptor) == FALSE)
	{
		Shell::DisplayError("Could not set pixel format.");
		return false;
	}

	render_context = wglCreateContext(device_context);
	if (render_context == NULL)
	{
		Shell::DisplayError("Could not create OpenGL rendering context.");
		return false;
	}

	// Activate the rendering context.
	if (wglMakeCurrent(device_context, render_context) == FALSE)
	{
		Shell::DisplayError("Unable to make rendering context current.");
		return false;
	}

	// Set up the GL state.
	glClearColor(0, 0, 0, 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screen_width, screen_height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}

static void DetachOpenGL()
{
	if (render_context != NULL)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(render_context);
		render_context = NULL;
	}

	if (device_context != NULL)
	{
		ReleaseDC(window_handle, device_context);
		device_context = NULL;
	}
}

static LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	// See what kind of message we've got.
	switch (message)
	{
		case WM_ACTIVATE:
		{
			if (LOWORD(w_param) != WA_INACTIVE)
			{
				activated = true;
			}
			else
			{
				activated = false;
			}
		}
		break;

		// When the window closes, request exit
		case WM_CLOSE:
		{
			running = false;
			return 0;
		}
		break;

		default:
		{
			InputWin32::ProcessWindowsEvent(message, w_param, l_param);
		}
		break;
	}

	// All unhandled messages go to DefWindowProc.
	return DefWindowProc(window_handle, message, w_param, l_param);
}
