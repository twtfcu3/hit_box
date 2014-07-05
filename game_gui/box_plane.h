#ifndef __BOX_PLANE_H__
#define __BOX_PLANE_H__
#include <SDL2/SDL.h>
#include "../sdlgui/sdlgui.h"
#include "../game_core/inc.h"
#include "./float_box.h"
#include "./fill_box.h"
using namespace std;
typedef class box_plane : public GUI<box_plane,sdl_widget>
{
	public:
		box_plane();
		box_plane(const char*,int,int,int,int,Uint32);
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event*);
		int handle(int,SDL_Event*);
		int on_keyup(sdl_board*,void*);
	public:
		fill_box _fill[3];
		float_box _float;
		sdl_widget _text;
	protected:
}*box_plane_ptr;
box_plane::box_plane()
	:
		GUI<box_plane,sdl_widget>()
{
	init();
}
box_plane::box_plane(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
	:
		GUI<box_plane,sdl_widget>()
{
	init(ptitle,px,py,pw,ph,pflag);
}
int box_plane::init()
{
	if(sdl_widget::init())return -1;
	return 0;
}
int box_plane::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	_fill[0].init(NULL,0,0,pw/column,(ph-info_height)/row,1);
	_fill[1].init(NULL,0,0,pw/column,(ph-info_height)/row,1);
	_fill[2].init(NULL,0,0,pw/column,(ph-info_height)/row,1);
	_float.init(NULL,0,0,pw/column,(ph-info_height)/row,1);
	_text.init(NULL,0,0,pw/column,(ph-info_height)/row,1);
	return 0;
}
int box_plane::sysevent(SDL_Event*e)
{
	switch(e->type)
	{
		default:
			break;
	}
	return sdl_widget::sysevent(e);
}
int box_plane::handle(int id,SDL_Event*e)
{
	return sdl_widget::handle(id,e);
}
int box_plane::on_keyup(sdl_board* obj,void*e)
{
	switch(((SDL_Event*)e)->key.keysym.sym)
	{
		case SDLK_LEFT:
			cout<<this<<endl;
		break;
	}
}
#endif
