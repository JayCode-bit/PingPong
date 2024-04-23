#pragma region Render Background
//// this a  rendering file we can use this whenever we need 
//// like a draw a rectangle (or) clear the screen etc,.
//internal void
//render_background()
//{
//	//create a current pixel pointer going to point your face
//	u32* pixel = (u32*)render_state.memory; //First pixel in our buffer
//
//	for (int y = 0; y < render_state.height; y++) //iterate through all the height and the width pixels
//	{
//		for (int x = 0; x < render_state.width; x++)
//		{
//			//for every pixel we gonna change its color to a orange and increment it.
//			*pixel++ = 0xff5500;  // we make gradients and play with colors example:- *pixel++ = 0xff00ff*x + 0x00ff00*y;
//
//		}
//	}
//}
#pragma endregion


// if you want to clear the screen so that the last frame doesnt appear on this frame
internal void 
clear_screen(u32 color) 
{
	//create a current pixel pointer going to point your face
	u32* pixel = (u32*)render_state.memory; //First pixel in our buffer

	for (int y = 0; y < render_state.height; y++) //iterate through all the height and the width pixels
	{
		for (int x = 0; x < render_state.width; x++)
		{
			//for every pixel we gonna change its color and increment it.
			*pixel++ = color;  
		}
	}
}



//procedure to draw a rectangle
internal void
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) 
{
	//clamp x and y value so it's min value = 0 & max value = width or height
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	//iterate through all the height and the width pixels
	for (int y = y0; y < y1; y++) 
	{
		//create a current pixel pointer going to point your face
		//First pixel in our buffer
		//for every row we're gonna draw, set the current pixel to the memory and offset that first pixel
		//by the first x pixel in the current y pixel times to width
		u32* pixel = (u32*)render_state.memory + x0 + y*render_state.width; 
		

		for (int x = x0; x < x1; x++)
		{
			//for every pixel we gonna change its color and increment it.
			*pixel++ = color;
		}
	}
}
global_variable float render_scale = 0.01f;

internal void
draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) 
{
	//its going to take 20% of the screen no matter what the screen size is
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	// to not to lose your world when decreased, add x & y to the half_width & half_height
	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	//change to pixels
	//simple mapping and generate the min and max points
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;


	draw_rect_in_pixels(x0,y0,x1,y1,color);

}

