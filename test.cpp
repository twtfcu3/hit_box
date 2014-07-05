#include <SDL2/SDL.h>
#include "sdlgui/sdlgui.h"
#include "game_gui/float_box.h"
#include "game_gui/fill_box.h"
#include "game_gui/box_plane.h"
#include "game_core/game_core.h"
using namespace std;
int main(int argc, char * argv[])
{
	float_box_ptr box;
	fill_box_ptr fill;
	sdl_widget_ptr w;
	//game_process game;
	box_plane_ptr plane;
	sdl_frame f("hitbox",100,100,300,330,SDL_WINDOW_SHOWN);	
	f.img_load("backgroup.jpg");
	plane = f.add<box_plane>("",0,0,300,300,1);
	sdl_event_manager::start();
	sdl_frame::run();
	return 0;
}
