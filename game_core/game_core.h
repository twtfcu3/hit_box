#ifndef __GAME_CORE_H__
#define __GAME_CORE_H__
#include "inc.h"
#include <cmath>
#include <iostream>
#include <string.h>
/* 
	每关由两个障碍块和三个填充块组成 
 */
struct game_box_status
{
	int a;
	int b;
	int c;
	int d;
	int e;
};
/* 
游戏一共有6关 
 */
class game_level_struct
{
	public:
		game_level_struct();	
		/* 
			取得指定级别的指定数据 
		 */
		int value(int,int);	
		/* 
			改变指定级别的指定数据 
		 */
		int value(int,int,int);	
		/* 
			取得当前游戏级别 
		 */
		int level();
		/* 
			设置当前游戏级别 
		 */
		int level(int);
	protected:
		int _level[6][5];
		int _level_count;
		int _cur_level;
};
game_level_struct::game_level_struct()
{
	/* 设置游戏层级 */
	_level_count = 6;
	_cur_level = 0;
	/* 设置游戏关卡 */
}
int game_level_struct::value(int level,int id)
{
	if(id>4 || level>=_level_count)return -1;
	return _level[level][id];
}
int game_level_struct::value(int level,int id,int v)
{
	if(id>4 || level>=_level_count)return -1;
	_level[level][id]=v;
	return 0;
}
int game_level_struct::level()
{
	return _cur_level;
}
int game_level_struct::level(int l)
{
	if(l>=_level_count)return -1;
	_cur_level = l;
	return 0;
}
class game_process
{
	public:
		game_process();
		/* 
			初始化指定层级的游戏状态 
		 */
		int init(int);
		/* 
			清除当前层级的游戏状态 
		 */
		int clear();
		/* 
			进入下一级游戏 
		 */
		int next();
		/* 
			进入上一级游戏 
		 */
		int last();
		/* 
			改变方块状态 
		 */
		int move(int);
		/* 
			返回当前方块可用个数 
		 */
		int box_count();
		/* 
			返回当前游戏层级 
		 */
		int level();
		/* 
			返回当前游戏状态 
		 */
		int state();
	protected:
		game_level_struct _level;
		int _box_count;
		int _box_plane[row*column];
		int _float_box_id;
};
game_process::game_process()
{
	init(0);
}
int game_process::init(int l)
{
	int i;
	if(l>_level.level())return -1;
	/* 初始化方块个数为3 */
	_float_box_id = 0;
	_box_count = 3;
	memset((char*)&_box_plane,0x00,sizeof(int)*row*column);
	_box_plane[0]=float_box_type;
	for(i=0;i<3;i++)
	{
		_box_plane[_level.value(l,i)] = fill_box_type;
	}
	for(i=3;i<5;i++)
	{
		_box_plane[_level.value(l,i)] = bind_box_type;
	}
	return 0;
}
int game_process::clear()
{
	return init(_level.level());
}
int game_process::next()
{
	return init(_level.level()+1);
}
int game_process::last()
{
	return init(_level.level()-1);
}
int game_process::box_count()
{
	return _box_count;
}
int game_process::level()
{
	return _level.level();
}
/* 
	移动时先确定移动的方向是否为滑动方块
	   如果是则移动滑动块,返回滑动ID
	   否则确定是填充方块还是摘取方块
		    如果是填充方块，确定是否还能填充方块
	         如果还有填充方块
	            移动的方向是否有障碍物
								如果有障碍物则填充,并判断游戏是否成功
									如果成功返回成功值
									否则返回填充ID
								否则返回错误值
		       否则返回错误值
		    如果是摘取方块
						移动的方向是否有方块可摘取
						    如果有方块则摘取,返回摘取ID
								否则返回错误值

 */
int game_process::move(int key)
{
	int t_float_id;
	int i;
	/* 是 */
	switch(key)
	{
		case move_up:
			/*
				 如果滑动ID是8的倍数或滑动ID加1后是8的倍数，
				 并且滑动ID大或等于8,
				 表示可以滑动
			 */
			if(!((_float_box_id%8) || !(_float_box_id+1)%8) && _float_box_id>=column)
			{
				_float_box_id-=column;
				return game_float;
			}
			else
			/* 
				如果当前滑块不是移动而是填充 
			 */
			if(_float_box_id > (row-1)*column)
			{
				for(t_float_id = _float_box_id;t_float_id>column;t_float_id-=column)
				{
					/* 如果可以填充则返回填充ID */
					if(t_float_id>fill_box_type)
					{
						if(!state())return game_win;
						return game_push;
					}
				}
			}
			else
			/* 
				如果当前滑块不是移动而是摘取 
			 */
			if(_float_box_id<row)
			{
				for(t_float_id = _float_box_id;t_float_id<((row-1)*column);t_float_id+=column)
				{
					/* 如果可以摘取则返回摘取ID */
					if(t_float_id==full_box_type)
					{
						return game_pull;
					}
					else
					/* 如果有固定方块返回错误 */
					if(t_float_id==bind_box_type)
					{
						return game_error;
					}
				}
			}
			else
			{
				return game_error;
			}
		break;
		case move_down:
			/*
				 如果滑动ID是8的倍数或滑动ID加1后是8的倍数，
				 并且滑动ID大或等于8,
				 表示可以滑动
			 */
			if(!((_float_box_id%8) || !(_float_box_id+1)%8) && _float_box_id>=column)
			{
				_float_box_id+=column;
			}
			else
			/* 
				如果当前滑块不是移动而是填充 
			 */
			if(_float_box_id<row)
			{
				for(t_float_id = _float_box_id;t_float_id<((row-1)*column);t_float_id+=column)
				{
					/* 如果可以填充则返回填充ID */
					if(t_float_id>fill_box_type)
					{
						if(!state())return game_win;
						return game_push;
					}
				}
			}
			else
			/* 
				如果当前滑块不是移动而是摘取 
			 */
			if(_float_box_id > (row-1)*column)
			{
				for(t_float_id = _float_box_id;t_float_id>column;t_float_id-=column)
				{
					/* 如果可以摘取则返回摘取ID */
					if(t_float_id==full_box_type)
					{
						return game_pull;
					}
					else
					/* 如果是固定方块返回错误 */
					if(t_float_id==bind_box_type)
					{
						return game_error;
					}
				}
			}
			else
			{
				return game_error;
			}
		break;
		case move_left:
			/* 
				如果滑动方块ID小于column或大于最后一行ID
				并且滑动方块ID加1后不是8的倍数
				表示可以滑动
			 */
			if((_float_box_id<column || _float_box_id>((row-1)*column-1)) && (_float_box_id+1)%8)
			{
				_float_box_id+=1;
			}
			else
			/* 如果滑动方块不是滑动而是摘取 */
			if(!((_float_box_id+1)%8))
			{
				for(t_float_id = _float_box_id-1;t_float_id>_float_box_id+(column-1);t_float_id-=1)
				{
					/* 如果可以摘取则返回摘取ID */
					if(t_float_id==full_box_type)
					{
						return game_pull;
					}
					else
					/* 如果是固定方块返回错误 */
					if(t_float_id==bind_box_type)
					{
						return game_error;
					}
				}
			}
			else
			/* 如果滑动方块不是滑动而是填充 */
			if(!(_float_box_id%8))
			{
				for(t_float_id = _float_box_id+1;t_float_id<_float_box_id+(column-1);t_float_id+=1)
				{
					/* 如果可以摘取则返回摘取ID */
					if(t_float_id==full_box_type)
					{
						if(!state())return game_win;
						return game_push;
					}
					else
					/* 如果是固定方块返回错误 */
					if(t_float_id==bind_box_type)
					{
						return game_error;
					}
				}
			}
			else
			{
				return game_error;
			}
		break;
		case move_right:
			/* 
				如果滑动方块ID小于column或大于最后一行ID
				并且滑动方块ID加1后不是8的倍数
				表示可以滑动
			 */
			if((_float_box_id<column || _float_box_id>((row-1)*column-1)) && (_float_box_id+1)%8)
			{
				_float_box_id-=1;
			}
			else
			/* 如果滑动方块不是滑动而是摘取 */
			if(!(_float_box_id%8))
			{
				for(t_float_id = _float_box_id+1;t_float_id<_float_box_id+(column-1);t_float_id+=1)
				{
					/* 如果可以摘取则返回摘取ID */
					if(t_float_id==full_box_type)
					{
						return game_pull;
					}
					else
					/* 如果是固定方块返回错误 */
					if(t_float_id==bind_box_type)
					{
						return game_error;
					}
				}
			}
			else
			/* 如果滑动方块不是滑动而是填充 */
			if(!((_float_box_id+1)%8))
			{
				for(t_float_id = _float_box_id-1;t_float_id>_float_box_id+(column-1);t_float_id-=1)
				{
					/* 如果可以摘取则返回摘取ID */
					if(t_float_id==full_box_type)
					{
						if(!state())return game_win;
						return game_push;
					}
					else
					/* 如果是固定方块返回错误 */
					if(t_float_id==bind_box_type)
					{
						return game_error;
					}
				}
			}
			else
			{
				return game_error;
			}
		break;
	}
	return 0;
}
int game_process::state()
{
	int i,c;
	for(i=0;i<3;i++)
	{
		if(_box_plane[_level.value(_level.level(),i)] == fill_box_type)return -1;
	}
	return 0;
}
#endif
