//Macros
#define is_down(b) input -> buttons[b].is_down
#define pressed(b) (input -> buttons[b].is_down && input -> buttons[b].changed) 
#define released(b) (!input -> buttons[b].is_down && input -> buttons[b].changed) 

float player_pos_x = 0.f;
float player_pos_y = 0.f;


internal void
simulate_game(Input* input, float deltatime)
{
	//as we know we made a black screen appear in the Render section below
	//now its time to change the colors of our pixels

	//render_background();

	// if you want to clear the screen so that the last frame doesnt appear on this frame
	clear_screen(0xff5500);

	float speed = 50.f; //Units per second

	if (is_down(BUTTON_UP)) player_pos_y += speed * deltatime; 
	if (is_down(BUTTON_DOWN)) player_pos_y -= speed * deltatime;
	if (is_down(BUTTON_RIGHT)) player_pos_x += speed * deltatime;
	if (is_down(BUTTON_LEFT)) player_pos_x -= speed * deltatime;
	

		draw_rect(player_pos_x, player_pos_y, 1, 1, 0x00ff22);//draw a rectangle


}