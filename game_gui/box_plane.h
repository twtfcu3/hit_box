#ifndef __BOX_PLANE_H__
#define __BOX_PLANE_H__
#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include "../sdlgui/sdlgui.h"
#include "../game_core/inc.h"
#include "../game_core/game_core.h"
#include "./float_box.h"
#include "./fill_box.h"
#include "./win_plane.h"
using namespace std;
typedef class box_plane : public GUI<box_plane,sdl_widget>
{
	public:
		box_plane();
		box_plane(const char*,int,int,int,int,Uint32);
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int init(int);
		int sysevent(SDL_Event*);
		int handle(int,SDL_Event*);
		int on_keyup(sdl_board*,void*);
		int on_win(sdl_board*,void*);
		int on_next(sdl_board*,void*);
	protected:
		int move_float_box(int);
		int push_box(int);
		int pull_box(int);
	public:
		fill_box _fill[3];
		fill_box* _fill_ptr[4];
		float_box _float;
		sdlsurface _box;
		sdl_widget _text;
		sdl_clip bg;
	protected:
		game_process game;
		win_plane win;
		stringstream str;
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
	_box.init(NULL,pw/column,(ph-info_height)/row,32,0,0,0,0);
	/* 加载背景 */
	bg.init(pw/column,(ph-info_height)/row,"img/ball_backgroup.jpg");
	/* 加载胜利画面 */
	win.init("",0,0,pw,ph,1);
	win.hide();
	add<win_plane>(&win);
	/* 注册事件 */
	win.connect_event("on_click",this,123456789);
	//
	_fill[0].init(NULL,0,0,pw/column,(ph-info_height)/row,1);
	_fill[1].init(NULL,0,0,pw/column,(ph-info_height)/row,1);
	_fill[2].init(NULL,0,0,pw/column,(ph-info_height)/row,1);
	_float.init(NULL,0,0,pw/column,(ph-info_height)/row,1);
	_text.init(NULL,0,(ph-info_height),pw,info_height,1);
	//
	add<float_box>(&_float);
	_float.connect_event("on_key_up",this,sdlgui_key_up);
	add<fill_box>(&_fill[0]);
	add<fill_box>(&_fill[1]);
	add<fill_box>(&_fill[2]);
	_fill[0].connect_event("on_key_up",this,sdlgui_key_up);
	_fill[1].connect_event("on_key_up",this,sdlgui_key_up);
	_fill[2].connect_event("on_key_up",this,sdlgui_key_up);
	//
	add<sdl_widget>(&_text);
	_text.fill_rect(NULL,0x00ff00);
	/* 初始化第一关游戏 */
	_box.surface_blend_mode(SDL_BLENDMODE_BLEND);
	init(0);
	return 0;
}
int box_plane::init(int l)
{
	int i;
	SDL_Rect rt;
	int s=row*column;
	/* 初始化游戏 */
	game.init(l);
	/* 初始化信息 */
	str.clear();
	str.str("");
	str<<"亲！我们在第"<<game.level()<<"关还有"<<game.box_count()<<"个方块要打，等你哦！！！";
	_text.text(str.str().c_str());
	/* 初始化填充容器 */
	_fill_ptr[0]=&_fill[0];
	_fill_ptr[1]=&_fill[1];
	_fill_ptr[2]=&_fill[2];
	/* 画好背景 */
	fill_rect(NULL,0xffffff);
	rt.w = bg.clip_width();
	rt.h = bg.clip_height();
	for(i=0;i<s;i++)
	{
		bg.clip(i,i,&_box,NULL);
		switch(game.box_type(i))
		{
			case float_box_type:
				_box.surface_alpha_mod(150);
			break;
			case empty_box_type:
				_box.surface_alpha_mod(255);
			break;
			case fill_box_type:
				_box.surface_alpha_mod(200);
			break;
			case full_box_type:
				_box.surface_alpha_mod(200);
			break;
			case bind_box_type:
				_box.surface_alpha_mod(50);
			break;
		}
		rt.x = i%column*bg.clip_width();
		rt.y = i/column*bg.clip_height();
		_box.blit_surface(NULL,this,&rt);
	}
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
	switch(id)
	{
		case 123456789:
			on_next(this,(void*)e);
		break;
	}
	return sdl_widget::handle(id,e);
}
int box_plane::on_keyup(sdl_board* obj,void*e)
{
	int s;
	switch(((SDL_Event*)e)->key.keysym.sym)
	{
		case SDLK_LEFT:
			s = game.move(move_left);
		break;
		case SDLK_RIGHT:
			s = game.move(move_right);
		break;
		case SDLK_UP:
			s = game.move(move_up);
		break;
		case SDLK_DOWN:
			s = game.move(move_down);
		break;
	}
	/* 如果是移动滑块 */
	if(s>=game_float)
	{
		s-=game_float;
		move_float_box(s);
	}
	else
	if(s>=game_push)
	{
		s-=game_push;
		push_box(s);
		if(!game.state())on_win(this,NULL);
	}
	else
	if(s>=game_pull)
	{
		s-=game_pull;
		pull_box(s);
	}
	else
	if(s>-1)
	{
		cout<<"win"<<endl;
	}
	else
	{
		cout<<"不能移动方块"<<endl;
	}
	return 0;
}
int box_plane::move_float_box(int id)
{
	int x = id%column*bg.clip_width();
	int y = id/column*bg.clip_height();
	_float.pos(x,y);
	return 0;
}
int box_plane::push_box(int id)
{
	int i;
	int x = id%column*bg.clip_width();
	int y = id/column*bg.clip_height();
	for(i=3;i;i--)
	{
		_fill_ptr[i]=_fill_ptr[i-1];
	}
	_fill_ptr[0]=_fill_ptr[3];
	_fill_ptr[0]->pos(_float.pos_x(),_float.pos_y());
	str.clear();
	str.str("");
	str<<"亲！我们在第"<<game.level()<<"关还有"<<game.box_count()<<"个方块要打，等你哦！！！";
	_text.text(str.str().c_str());
	return _fill_ptr[0]->push(x,y);
}
int box_plane::pull_box(int id)
{
	int x = _float.pos_x();
	int y = _float.pos_y();
	int box_x = id%column*bg.clip_width();
	int box_y = id/column*bg.clip_height();
	int i;
	for(i=0;i<(4-game.box_count());i++)
	{
		if(_fill_ptr[i]->_loc.x==box_x && _fill_ptr[i]->_loc.y==box_y)
		{
			_fill_ptr[3]=_fill_ptr[i];
			for(i;i<3;i++)
			{
				_fill_ptr[i]=_fill_ptr[i+1];	
			}
			str.clear();
			str.str("");
			str<<"亲！我们在第"<<game.level()<<"关还有"<<game.box_count()<<"个方块要打，等你哦！！！";
			_text.text(str.str().c_str());
			return _fill_ptr[3]->pull(x,y);
		}
	}
	return -1;
}
int box_plane::on_win(sdl_board* obj,void* data)
{
	win.show();
}
int box_plane::on_next(sdl_board* obj,void* data)
{
	int i;
	for(i=0;i<3;i++)
	{
		_fill[i].pull(0,0);
	}
	init(game.level()+1);
}
#endif
