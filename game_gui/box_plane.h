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
		~box_plane();
		int init();
		int init(const char*,int,int,int,int,Uint32); 
		int sysevent(SDL_Event*);
		int draw();
	public:
		int next_level();	
	protected:
		int move_float_box(int id);
		int push_box(int id);
		int pull_box(int id);
	protected:
		/* 背景剪辑 */
		sdl_clip bg;
		/* 游戏内核 */
		game_process _game;
		/* 活动方块 */
		float_box _float_box;
		/* 填充方块 */
		fill_box _fill_box[3];
		fill_box_ptr _fill_box_ptr[4];
		/* 过关画面 */
		win_plane _win;
		/* 信息画面 */
		sdl_widget _info;
		stringstream _info_str;
}*box_plane_ptr;
box_plane::box_plane()
	:
		GUI<box_plane,sdl_widget>()
{
	init();
}
box_plane::box_plane(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflags)
	:
		GUI<box_plane,sdl_widget>()
{
	init(ptitle,px,py,pw,ph,pflags);
}
box_plane::~box_plane()
{

}
int box_plane::init()
{
	if(sdl_widget::init())return -1;
	return 0;
}
int box_plane::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflags)
{
	/* 初始背景剪辑 */
	bg.init(clip_width,clip_height,bg_path);
	_info.init("亲，我们在第0关还有3个方块要打，等你哦!!",0,ph-info_height,pw,info_height,1);
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflags))return -1;
	/* 加入活动方块 */
	_float_box.init("",0,0,clip_width,clip_height,1);
	add<float_box>(&_float_box);
	/* 加入填充方块 */
	_fill_box[0].init("",0,0,clip_width,clip_height,1);
	_fill_box[1].init("",0,0,clip_width,clip_height,1);
	_fill_box[2].init("",0,0,clip_width,clip_height,1);
	add<fill_box>(&_fill_box[0]);
	add<fill_box>(&_fill_box[1]);
	add<fill_box>(&_fill_box[2]);
	_fill_box_ptr[0]=&_fill_box[0];
	_fill_box_ptr[1]=&_fill_box[1];
	_fill_box_ptr[2]=&_fill_box[2];
	/* 加入过关画面 */  

	_win.init ("",0,0,pw,ph,1);
	add<win_plane>(&_win);
	/* 加入信息画面 */
	_info.blend(SDL_BLENDMODE_BLEND);
	_info.fill_rect(NULL,0xffffff);
	_info.color_key(1,0xffffff);
	add<sdl_widget>(&_info);
	/* 填充窗口 */
	draw();
	/* 激活窗口 */
	active();
	/* 预定义事件 */
	on_keyup()=[this](sdl_board& obj,SDL_Event& e)
	{
		int s;
		switch(e.key.keysym.sym)
		{
			case SDLK_LEFT:
				s = _game.move(move_left);
			break;
			case SDLK_RIGHT:
				s = _game.move(move_right);
			break;
			case SDLK_UP:
				s = _game.move(move_up);
			break;
			case SDLK_DOWN:
				s = _game.move(move_down);
			break;
		}
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
			_info_str.clear();
			_info_str.str("");
			_info_str<<"亲，我们在第"<<_game.level()<<"关还有"<<_game.box_count()<<"方块要打，等你哦!!";
			_info.text(_info_str.str().c_str());
			if(!_game.state())
			{
				_win.show();
			}
		}
		else
		if(s>=game_pull)
		{
			s-=game_pull;
			pull_box(s);
			_info_str.clear();
			_info_str.str("");
			_info_str<<"亲，我们在第"<<_game.level()<<"关还有"<<_game.box_count()<<"方块要打，等你哦!!";
			_info.text(_info_str.str().c_str());
		}
		else
		if(s>-1)
		{
			std::cout<<"win"<<std::endl;
		}
		else
		{
			std::cout<<"不能移动"<<std::endl;
		}
		return 0;
	};
	_win.on_hide()=[this](sdl_board& obj,SDL_Event& e)
	{
		next_level();	
		return 0;
	};
	return 0;
}
int box_plane::sysevent(SDL_Event* e)
{
	return sdl_widget::sysevent(e);
}
int box_plane::draw()
{
	int i=0;
	SDL_Rect rt;
	rt.w = clip_width; 
	rt.h = clip_height; 
	fill_rect(NULL,0xffffff);
	sdlsurface tsur(0,rt.w,rt.h,32,0,0,0,0);
	tsur.surface_blend_mode(SDL_BLENDMODE_BLEND);
	_fill_box_ptr[0]=&_fill_box[0];
	_fill_box_ptr[1]=&_fill_box[1];
	_fill_box_ptr[2]=&_fill_box[2];
	_fill_box[0].pull(0,0);
	_fill_box[1].pull(0,0);
	_fill_box[2].pull(0,0);
	for(i=0;i<row*column;i++)
	{
		bg.clip(i,i,&tsur,NULL);
		switch(_game.box_type(i))	
		{
			case float_box_type:
				tsur.surface_alpha_mod(float_box_alpha);
			break;
			case empty_box_type:
				tsur.surface_alpha_mod(empty_box_alpha);
			break;
			case fill_box_type:
				tsur.surface_alpha_mod(fill_box_alpha);
			break;
			case full_box_type:
				tsur.surface_alpha_mod(full_box_alpha);
			break;
			case bind_box_type:
				tsur.surface_alpha_mod(bind_box_alpha);
			break;
		}
		rt.x = i%column*bg.clip_width();
		rt.y = i/column*bg.clip_height();
		tsur.blit_surface(NULL,this,&rt);
	}
	_info_str.clear();
	_info_str.str("");
	_info_str<<"亲，我们在第"<<_game.level()<<"关还有"<<_game.box_count()<<"方块要打，等你哦!!";
	_info.text(_info_str.str().c_str());
	return 0;
	//return sdl_widget::draw();
}
int box_plane::next_level()
{
	_game.next();
	return draw();
}
int box_plane::move_float_box(int id)
{
	int x = id%column*bg.clip_width();
	int y = id/column*bg.clip_height();
	return _float_box.pos(x,y);
}
int box_plane::push_box(int id)
{
	int i;
	int x = id%column*bg.clip_width();
	int y = id/column*bg.clip_height();
	for(i=3;i;i--)
	{
		_fill_box_ptr[i]=_fill_box_ptr[i-1];
	}
	_fill_box_ptr[0]=_fill_box_ptr[3];
	_fill_box_ptr[0]->pos(_float_box.pos_x(),_float_box.pos_y());
	return _fill_box_ptr[0]->push(x,y);
}
int box_plane::pull_box(int id)
{
	int i;
	int x = _float_box.pos_x();
	int y = _float_box.pos_y();
	int box_x = id%column*bg.clip_width();
	int box_y = id/column*bg.clip_height();

	for(i=0;i<(4-_game.box_count());i++)
	{
		if(_fill_box_ptr[i]->_loc.x==box_x && _fill_box_ptr[i]->_loc.y==box_y)
		{
			_fill_box_ptr[3]=_fill_box_ptr[i];
			for(i;i<3;i++)
			{
				_fill_box_ptr[i]=_fill_box_ptr[i+1];
			}
			return _fill_box_ptr[3]->pull(x,y);
		}
	}
	return -1;
}
#endif
