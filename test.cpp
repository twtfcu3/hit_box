#include <SDL2/SDL.h>
#include "sdlgui/sdlgui.h"
#include "game_gui/float_box.h"
#include "game_gui/fill_box.h"
#include "game_gui/box_plane.h"
#include "game_core/game_core.h"
using namespace std;
int main(int argc, char * argv[])
{
	box_plane_ptr plane;
	sdl_frame f("hitbox",100,100,320,350,SDL_WINDOW_SHOWN);	
	plane = f.add<box_plane>("",0,0,320,350,1);
	sdl_frame::run();
	return 0;
}
