// button state struct which will hold is_down & changed variable
struct Button_State
{
	bool is_down;
	bool changed;
};

// enumerate which buttons we care about
enum 
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,

	BUTTON_COUNT, //should be the last item
};

//Input struct will just be an array of buttons
struct Input 
{
	Button_State buttons[BUTTON_COUNT];

};