#ifndef __FLOAT_BOX_HEAD__
#define __FLOAT_BOX_HEAD__
#include <SDL2/SDL.h>
#include "../sdlgui/sdlgui.h"
using namespace std;
typedef class float_box : public GUI<float_box,sdl_widget>
{
	public:
		float_box();
		float_box(const char*,int,int,int,int,Uint32);
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event*);
		int handle(int,SDL_Event*);
		int on_timer(sdl_board*,void*);
	protected:
		float _alpha;
		int f;
		SDL_TimerID _timer;
}*float_box_ptr;
float_box::float_box()
	:
		GUI<float_box,sdl_widget>()
{
	init();
}
float_box::float_box(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
	:
		GUI<float_box,sdl_widget>()
{
	init(ptitle,px,py,pw,ph,pflag);
}
int float_box::init()
{
	if(sdl_widget::init())return -1;
	return 0;
}
int float_box::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	fill_rect(NULL,0x0000ff);
	_timer = add_timer(100);
	blend(SDL_BLENDMODE_BLEND);
	_alpha = 255;
	f = -1;
	return 0;
}
int float_box::sysevent(SDL_Event*e)
{
	switch(e->type)
	{
		default:
			break;
	}
	return sdl_widget::sysevent(e);
}
int float_box::handle(int id,SDL_Event* e)
{
	return sdl_board::handle(id,e);
}
int float_box::on_timer(sdl_board* obj,void* data)
{
	_alpha+=f*(_alpha/10);
	if(_alpha>255)
	{
		_alpha=255;
		f*=-1;
	}
	else
	if(_alpha<100)
	{
		_alpha = 100;
		f*=-1;
	}
	alpha(_alpha);
}
#endif //__FLOAT_BOX_HEAD__
