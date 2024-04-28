#include "utilities.cpp"
#include <Windows.h>

global_variable bool running = true;

//variables to Struct
struct Render_State
{
	void* memory; // Buffer Memory
	int height, width;
	BITMAPINFO bitmap_info; //pixel info about how its look like or compression info and how to copy that to video card
};
//Now make this struct as a global variable
global_variable Render_State render_state;

#include "platform_common.cpp"

// there is not need to Renderer.cpp to compile separately, so we need to Right click >> Properties >> Excluded from build >> Yes.
#include "Renderer.cpp" // Unity bulid, merging other .cpp files to this current .cpp files and makes a single .obj file for .exe
#include "game.cpp"

//callBack function ; is the way windows use to pass messages down to us
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
	{
		running = false;
	} break;

	// in case we receive (WM)Window size message : that the window changed size
	case WM_SIZE:
	{
		//now we must know the new size by getting the clients Rect
		//this function takes the window and a pointer to a rect struct to attain the left/right,  up/bottom values
		RECT rect;
		GetClientRect(hwnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;
		//now our buffer contains width times height pixels: number_of_pixels = w*h
		//render_state.size =  number_of_pixels * sizeof(pixel)

		int size = render_state.width * render_state.height * sizeof(unsigned int);

		//what if the player changed the size again, in our case we already have allocated memory
		// we have to check if our pointer already has some value memory and if does free it first before allocating new memory
		if (render_state.memory)
			VirtualFree(render_state.memory, 0, MEM_RELEASE);

		/* Asking windows for a new HEAP memory, for clearing and reallocating different window sizes in that memory
		   VirtualAlloc(); takes you to the address we want to create.
		   the memory you're gonna take is void pointer, which means it's a pointer to somwhere in the memory */

		render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


		//BITMAPINFOHEADER structure which contains information about the dimentions and color format of a DIB
		render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader); //size
		render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
		render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
		render_state.bitmap_info.bmiHeader.biPlanes = 1; // planes should be 1 for some reason
		render_state.bitmap_info.bmiHeader.biBitCount = 32; // remember 32bit unsigned integer
		render_state.bitmap_info.bmiHeader.biCompression = BI_RGB; //compression

	}
	break;

	default:
	{
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	}

	return result;
}


//entry point for graphical windows based application
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	ShowCursor(FALSE);

	//First thing to do is to show a window so we can draw on that
	//In order to draw a window on windows we need 03 things

	//1. Create Window Class
	WNDCLASS window_class = {}; // empty class

	//this will make sure that we redraw the window Horizantally and Vertically whenever we want
	window_class.style = CS_HREDRAW | CS_VREDRAW;

	//Class Name STRING to identify our window class; not visible to the normal user
	window_class.lpszClassName = "Game Window Class";

	//callBack function ; is the way windows use to pass messages down to us
	window_class.lpfnWndProc = window_callback;


	//2. Register Class
	//Before creating a window of a specific class, your application must register that class 
	// using either RegisterClassA (for ANSI character set) or RegisterClassW (for Unicode character set).
	RegisterClass(&window_class);


	//3. Create Window


	HWND window = CreateWindow(window_class.lpszClassName, TEXT("Ping Pong"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	{
		//Game FullScreen
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi)};
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	HDC hdc = GetDC(window);

	//create Input Struct and zero that out
	Input input = { };

	//time variable it stands  for how much time lapsed in one frame
	float delta_time = 0.016666f;  // assume first frame 60FPS

	//counter at the end of the frame
	//CPU time
	LARGE_INTEGER frame_begin_time;
	//performance counter function
	QueryPerformanceCounter(&frame_begin_time);

	// Call query performance frequency function
	//this function returns how many cycles in one second
	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}


	//Game Loop
	while (running)
	{
		//Input
		MSG message;

		// Reset the change the beginning of every frame
		//because you only want to change to last one frame
		// so whenever we start a new frame we're good to run through all the buttons and set the change to false
		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			input.buttons[i].changed = false;
		}


		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
			case WM_KEYUP:
			case WM_KEYDOWN:
			{
				// we need to extract two things from this message
			   // 1.what key is pressed?, Since we are processing them together
				u32 vk_code = (u32)message.wParam;

				// 2. we need to knw if it was a key up or key down event
				bool is_down = ((message.lParam & (1 << 31)) == 0);

				//Define a macro called "process_button" that takes two arguments:
				// The macro updates the state of the specified button based on whether it is pressed or released.
#define process_button(b, vk)\
						case vk: {\
							input.buttons[b].is_down = is_down;\
							input.buttons[b].changed = true;\
						}break;\

					//The backslash '\' ensures that the entire macro definition is treated as a single logical line during macro expansion.

				switch (vk_code)
				{
					process_button(BUTTON_UP, VK_UP);
					process_button(BUTTON_DOWN, VK_DOWN);
					process_button(BUTTON_W, 'W');
					process_button(BUTTON_S, 'S');
					process_button(BUTTON_LEFT, VK_LEFT);
					process_button(BUTTON_RIGHT, VK_RIGHT);
					process_button(BUTTON_ENTER, VK_RETURN);
				}

			}break;

			default:
			{

				TranslateMessage(&message);

				DispatchMessage(&message);

			}break;
			}


		}


		//Simulate 
		simulate_game(&input, delta_time);



		//Render
		//As we have a render_state.memory we need to save to windows and ask to use it 
		// to do that we need the function called StretchDIBits();
		// Now we will have perfect black screen, we have black because our render_state.memory is Zeroed out. 
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		//counter at the end of the frame 
		//CPU time
		LARGE_INTEGER frame_end_time;
		//performance counter function
		QueryPerformanceCounter(&frame_end_time);

		//Difference but not in seconds yet,  this function works on CPU time  // Call "query performance frequency" (search in find) function << 
		// and divide the subraction by the frequency you'll get the data time in seconds
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;


		frame_begin_time = frame_end_time; //new frame

	}

	//^^^^^^^^ BASIC GAME ENGINE IS READY TO PROGRAM THE GAME (above this line) ^^^^^^^^^^^^ 
}

