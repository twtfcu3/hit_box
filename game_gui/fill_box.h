#ifndef __FILL_BOX_HEAD__
#define __FILL_BOX_HEAD__
#include <SDL2/SDL.h>
#include <cmath>
#include "../sdlgui/sdlgui.h"
class box_plane;
using namespace std;
typedef class fill_box : public GUI<fill_box,sdl_widget>
{
	public:
		friend class box_plane;
	public:
		fill_box();
		fill_box(const char*,int,int,int,int,Uint32);
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event*);
		int handle(int,SDL_Event*);
		int on_timer(sdl_board*,void*);
		int push(int,int);
		int pull(int,int);
	protected:
		int _alpha;
		int _f;
		int _dst;
		SDL_Point _loc;
		SDL_Point _loc_f;
		SDL_TimerID _timer;
}*fill_box_ptr;
fill_box::fill_box()
	:
		GUI<fill_box,sdl_widget>()
{
	init();
}
fill_box::fill_box(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
	:
		GUI<fill_box,sdl_widget>()
{
	init(ptitle,px,py,pw,ph,pflag);
}
int fill_box::init()
{
	if(sdl_widget::init())return -1;
	_timer = 0;
	return 0;
}
int fill_box::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	fill_rect(NULL,0x0000ff);
	blend(SDL_BLENDMODE_BLEND);
	alpha(0);
	return 0;
}
int fill_box::sysevent(SDL_Event*e)
{
	switch(e->type)
	{
		default:
			break;
	}
	return sdl_widget::sysevent(e);
}
int fill_box::handle(int id,SDL_Event* e)
{
	return sdl_widget::handle(id,e);
}
int fill_box::on_timer(sdl_board* obj,void* data)
{
	int dx = _rect.x;
	int dy = _rect.y;
	int dst = sqrt(pow((dx-_loc.x),2)+pow((dy-_loc.y),2));
	if(dst>5)
	{
		_rect.x += (_loc.x-_rect.x)/3;
		_rect.y += (_loc.y-_rect.y)/3;
		alpha(abs(dst/_dst-_f)*255);
	}
	else
	{
		SDL_RemoveTimer(_timer);
		_timer = 0;
		_rect.x = _loc.x;
		_rect.y = _loc.y;
	}
	return 0;
}
int fill_box::push(int x,int y)
{
	_loc.x = x;
	_loc.y = y;
	_loc_f.x = (_loc.x > _rect.x)?1:-1;
	_loc_f.y = (_loc.y > _rect.y)?1:-1;
	_dst = sqrt(pow((_rect.x-_loc.x),2)+pow((_rect.y-_loc.y),2));
	_f = 1;
	if(!_timer)
	{
		_timer = add_timer(100);
	}
	return 0;
}
int fill_box::pull(int x,int y)
{
	_loc.x = x;
	_loc.y = y;
	_loc_f.x = (_loc.x > _rect.x)?1:-1;
	_loc_f.y = (_loc.y > _rect.y)?1:-1;
	_dst = sqrt(pow((_rect.x-_loc.x),2)+pow((_rect.y-_loc.y),2));
	_f = 0;
	if(!_timer)
	{
		_timer = add_timer(100);
	}
	return 0;
}
#endif
