//Macros
#define is_down(b) input -> buttons[b].is_down
#define pressed(b) (input -> buttons[b].is_down && input -> buttons[b].changed) 
#define released(b) (!input -> buttons[b].is_down && input -> buttons[b].changed) 


//store the velocity across frames just like we did to position
//Derivative of the position
float player_1_p, player_1_dp, player_2_p, player_2_dp;

//Arena & Player half size for Collision Detection
float arena_half_size_x = 80, arena_half_size_y = 45;

float player_half_size_x = 2.5, player_half_size_y = 12;

//Ball will linerally flow around the arena not losing or gaining energy, but set an initial velocity.
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y, ball_half_size = 1;


internal void 
simulate_player(float *p, float *dp, float ddp, float deltatime ) 
{

}


internal void
simulate_game(Input* input, float deltatime)
{
	//as we know we made a black screen appear in the Render section below
	//now its time to change the colors of our pixels

	//render_background();

	// if you want to clear the screen so that the last frame doesnt appear on this frame
	clear_screen(0xff5500); //background

	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33); // Arena

	//Player 1
	float player_1_ddp = 0.f; //accelerate 
	//player 1 controller
	if (is_down(BUTTON_UP)) player_1_ddp += 2000;
	if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;

	//Player 2
	float player_2_ddp = 0.f; //accelerate 
	//player 2 controller
	if (is_down(BUTTON_W)) player_2_ddp += 2000;
	if (is_down(BUTTON_S)) player_2_ddp -= 2000;



	//Ball position equation with the first derivative
	ball_p_x += ball_dp_x * deltatime;
	ball_p_y += ball_dp_y * deltatime;


	draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);  //ball

	//Add collision on ball PLAYER 1
	if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
		ball_p_x - ball_half_size < 80 + player_half_size_x &&
		ball_p_y + ball_half_size > player_1_p - player_half_size_y &&
		ball_p_y + ball_half_size < player_1_p + player_half_size_y) //AABB vs AABB collision
	{
		ball_p_x = 80 - player_half_size_x - ball_half_size;//when we aren't colliding, set the ball pin to limit position
		ball_dp_x *= -1; //velocity
		ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * 0.75f; //change the direction of the ball based on where he hit on player
	}
	//Add collision on ball PLAYER 2
	else if (ball_p_x + ball_half_size > -80 - player_half_size_x && 
		ball_p_x - ball_half_size < -80 + player_half_size_x &&
		ball_p_y + ball_half_size > player_2_p - player_half_size_y &&
		ball_p_y + ball_half_size < player_2_p + player_half_size_y) //AABB vs AABB collision
		 {
			ball_p_x = -80 + player_half_size_x + ball_half_size;//when we aren't colliding, set the ball pin to limit position
			ball_dp_x *= -1; //velocity
			ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * 0.75f; //change the direction of the ball based on where he hit on player
		 }

	//Collision for bottom and top arena area 
	if (ball_p_y + ball_half_size  > arena_half_size_y) 
	{//top
		ball_p_y = arena_half_size_y - ball_half_size;
		ball_dp_y *= -1;
	}
	else if (ball_p_y - ball_half_size < -arena_half_size_y)
	{//bottom
		ball_p_y = -arena_half_size_y + ball_half_size;
		ball_dp_y *= -1;
	}

	//Collision for Right and Left arean area
	if (ball_p_x + ball_half_size > arena_half_size_x)
	{
		ball_dp_x *= -1;
		ball_dp_y = 0;
		ball_p_x = 0;
		ball_p_y = 0;
	}
	else if (ball_p_x - ball_half_size < -arena_half_size_x)
	{
		ball_dp_x *= -1;
		ball_dp_y = 0;
		ball_p_x = 0;
		ball_p_y = 0;
	}

	//draw_rect(player_pos_x, player_pos_y, 1, 1, 0x00ff22);//draw a rectangle
	draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000); //Player 1
	draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000); // Player 2



}