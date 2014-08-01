#include <SDL2/SDl.h>
#include "../sdlgui/sdlgui.h"
#include <iostream>
using namespace std;
typedef class win_plane : public GUI<win_plane,sdl_widget>
{
	public:
		win_plane();
		win_plane(const char*,int,int,int,int,Uint32);
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event*);
		int show();
		int hide();
	public:
		event_signal on_hide;
	protected:
		SDL_TimerID _timer;
		int _state;
		int _alpha;
}*win_plane_ptr;
win_plane::win_plane()
	:
		GUI<win_plane,sdl_widget>()
{
	init();
}
win_plane::win_plane(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
	:
		GUI<win_plane,sdl_widget>()
{
	init(ptitle,px,py,pw,ph,pflag);
}
int win_plane::init()
{
	if(sdl_widget::init())return -1;
	return 0;
}
int win_plane::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	SDL_Event e;
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	_state = 0;
	_alpha = 0;
	img_load(mouth_path);
	color_key(1,0x00);
	blend(SDL_BLENDMODE_BLEND);
	alpha(_alpha);
	sdl_widget::hide();
	/* 预定义事件 */
	on_timer()=[this](sdl_board& obj,SDL_Event& e)
	{
		sdl_board::show();
		if(_state)
		{
			if(_alpha<255)
			{
				_alpha+=(_alpha+1);
			}
			else
			{
				_alpha = 255;
				SDL_RemoveTimer(_timer);
				_timer = 0;
			}
		}
		else
		{
			if(_alpha>=10)
			{
				_alpha-=(_alpha/3);
			}
			else
			{
				_alpha = 0;
				SDL_RemoveTimer(_timer);
				_timer = 0;
				sdl_board::hide();
				on_hide(*this,e);
			}
		}
		alpha(_alpha);
		return 0;
	};
	on_click()=[this](sdl_board& obj,SDL_Event& e)
	{
		hide();
		return 0;
	};
	return 0;
}
int win_plane::sysevent(SDL_Event*e)
{
	switch(e->type)
	{
		default:
			break;
	}
	return sdl_widget::sysevent(e);
}
int win_plane::show()
{
	if(_timer)return -1;
	_state = 1;
	_timer = add_timer(100);
	return sdl_board::show();
}
int win_plane::hide()
{
	if(_timer)return -1;
	_state = 0;
	_timer = add_timer(100);
	return 0;
}
