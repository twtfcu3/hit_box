#ifndef __GAME_CORE_H__
#define __GAME_CORE_H__
#include "inc.h"
#include <cmath>
#include <iostream>
#include <string.h>
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
		int value(int,int,int,int,int,int);
		/* 取得游戏级别数 */
		int level_count();
		/* 
			取得当前游戏级别 
		 */
		int level();
		/* 
			设置当前游戏级别 
		 */
		int level(int);
	protected:
		/* 
			一共有6关,每关由两个障碍块和三个填充块组成 
		*/
		int _level[6][5];
		int _level_count;
		int _cur_level;
};
game_level_struct::game_level_struct()
{
	/* 设置游戏层级 */
	_level_count = 6;
	_cur_level = 0;
	/* 设置游戏关卡 
		 一共有6关,每关由两个障碍块和三个填充块组成 
	 */
	value(0,44,45,36,43,52);
	value(1,34,37,27,28,35);
	value(2,36,49,28,35,42);
	value(3,28,44,30,37,43);
	value(4,36,38,29,45,53);
	value(5,11,43,27,28,29);
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
int game_level_struct::value(int level,int a,int b,int c,int d,int e)
{
	if(level>=_level_count)return -1;
	_level[level][0]=a;
	_level[level][1]=b;
	_level[level][2]=c;
	_level[level][3]=d;
	_level[level][4]=e;
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
int game_level_struct::level_count()
{
		return _level_count;
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
			返回指定方块的类型
		 */
		int box_type(int);
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
	_float_box_id = 0;
	init(0);
}
int game_process::init(int l)
{
	int i;
	if(l>_level.level_count())return -1;
	_level.level(l);
	/* 初始化方块个数为3 */
	_box_count = 3;
	//memset((char*)&_box_plane,0x00,sizeof(int)*row*column);
	/* 设置空白方块或活动方块 */
	for(i=0;i<column*row;i++)
	{
		if(i<column || i>((row-1)*column) || !(i%column) || !((i+1)%column))
		{
			_box_plane[i] = float_box_type;
		}
		else
		{
			_box_plane[i] = empty_box_type;
		}
		//cout<<i<<endl;
	}
	/* 设置固定方块ID */
	for(i=0;i<2;i++)
	{
		_box_plane[_level.value(l,i)] = bind_box_type;
	}
	/* 设置填充方块ID */
	for(i=2;i<5;i++)
	{
		_box_plane[_level.value(l,i)] = fill_box_type;
	}
	_box_plane[_float_box_id]=float_box_type;
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
int game_process::box_type(int id)
{
	if(id>(row*column-1))return -1;
	return _box_plane[id];
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
	switch(key)
	{
		case move_up:
			/*
				 如果滑动ID是8的倍数或滑动ID加1后是8的倍数，
				 并且滑动ID大或等于8,
				 表示可以滑动
			 */
			if((!(_float_box_id%column) || !((_float_box_id+1)%column)) && _float_box_id>=column)
			{
				_float_box_id-=column;
				return game_float+_float_box_id;
			}
			else
			/* 
				如果当前滑块不是移动而是填充 
			 */
			if(_float_box_id > (row-1)*column)
			{
				if(!_box_count)return game_error;
				for(t_float_id = _float_box_id-column;t_float_id>column;t_float_id-=column)
				{
					/* 如果可以填充则返回填充ID */
					if(_box_plane[t_float_id]>fill_box_type)
					{
						t_float_id+=column;
						if(t_float_id>((row-1)*column))
						{
							return game_error;
						}
						else
						{
							_box_plane[t_float_id]+=box_switch_type;
							_box_count--;
							//if(!state())return game_win;
							return t_float_id+game_push;
						}
					}
				}
			}
			else
			/* 
				如果当前滑块不是移动而是摘取 
			 */
			if(_float_box_id<column)
			{
				for(t_float_id = _float_box_id+column;t_float_id<((row-1)*column);t_float_id+=column)
				{
					/* 如果可以摘取则返回摘取ID */
					if(_box_plane[t_float_id]<bind_box_type && _box_plane[t_float_id]>fill_box_type)
					{
						_box_plane[t_float_id]-=box_switch_type;
						_box_count++;
						return game_pull+t_float_id;
					}
					else
					/* 如果有固定方块返回错误 */
					if(_box_plane[t_float_id]==bind_box_type)
					{
						return game_error;
					}
				}
			}
			return game_error;
		break;
		case move_down:
			/*
				 如果滑动ID是8的倍数或滑动ID加1后是8的倍数，
				 并且滑动ID大或等于8,
				 表示可以滑动
			 */
			if((!(_float_box_id%column) || !((_float_box_id+1)%column)) && _float_box_id<column*(row-1))
			{
				_float_box_id+=column;
				return game_float + _float_box_id;
			}
			else
			/* 
				如果当前滑块不是移动而是填充 
			 */
			if(_float_box_id<column)
			{
				if(!_box_count)return game_error;
				//if(!state())return game_win;
				for(t_float_id = _float_box_id+column;t_float_id<((row-1)*column);t_float_id+=column)
				{
					/* 如果可以填充则返回填充ID */
					if(_box_plane[t_float_id]>fill_box_type)
					{
						t_float_id-=column;
						/* 如果填充后的ID到了上边界，返回错误值 */
						if(t_float_id<column)
						{
							return game_error;
						}
						else
						{
							_box_plane[t_float_id]+=box_switch_type;
							_box_count--;
							return t_float_id+game_push;
						}
					}
				}
			}
			else
			/* 
				如果当前滑块不是移动而是摘取 
			 */
			if(_float_box_id > (row-1)*column)
			{
				for(t_float_id = _float_box_id-column;t_float_id>column;t_float_id-=column)
				{
					/* 如果可以摘取则返回摘取ID */
					if(_box_plane[t_float_id]<bind_box_type && _box_plane[t_float_id]>fill_box_type)
					{
						_box_count++;
						_box_plane[t_float_id]-=box_switch_type;
						return game_pull+t_float_id;
					}
					else
					/* 如果是固定方块返回错误 */
					if(_box_plane[t_float_id]==bind_box_type)
					{
						return game_error;
					}
				}
			}
			return game_error;
		break;
		case move_left:
			/* 
				如果滑动方块ID小于column或大于最后一行ID
				并且滑动方块ID加1后不是8的倍数
				表示可以滑动
			 */
			if((_float_box_id<column || _float_box_id>((row-1)*column-1)) && _float_box_id%column)
			{
				_float_box_id-=1;
				return _float_box_id+game_float;
			}
			else
			/* 如果滑动方块不是滑动而是填充 */
			if(!((_float_box_id+1)%column))
			{
				if(!_box_count)return game_error;
				//if(!state())return game_win;
				for(t_float_id = _float_box_id-1;t_float_id>_float_box_id-(column-1);t_float_id-=1)
				{
					/* 如果可以填充则返回填充ID */
					if(_box_plane[t_float_id]>fill_box_type)
					{
						t_float_id+=1;
						if(!(t_float_id%column))
						{
							return game_error;
						}
						else
						{
							_box_plane[t_float_id]+=box_switch_type;
							_box_count--;
							return game_push+t_float_id;
						}
					}
				}
			}
			else
			/* 如果滑动方块不是滑动而是摘取 */
			if(!(_float_box_id%column))
			{
				for(t_float_id = _float_box_id+1;t_float_id<_float_box_id+(column-1);t_float_id+=1)
				{
					/* 如果可以摘取则返回摘取ID */
					if(_box_plane[t_float_id]<bind_box_type && _box_plane[t_float_id]>fill_box_type)
					{
						_box_plane[t_float_id]-=box_switch_type;
						_box_count++;
						return game_pull+t_float_id;
					}
					else
					/* 如果是固定方块返回错误 */
					if(_box_plane[t_float_id]==bind_box_type)
					{
						return game_error;
					}
				}
			}
			return game_error;
		break;
		case move_right:
			/* 
				如果滑动方块ID小于column或大于最后一行ID
				并且滑动方块ID加1后不是8的倍数
				表示可以滑动
			 */
			if((_float_box_id<column || _float_box_id>((row-1)*column-1)) && (_float_box_id+1)%column)
			{
				_float_box_id+=1;
				return _float_box_id+ game_float;
			}
			else
			/* 如果滑动方块不是滑动而是填充 */
			if(!(_float_box_id%column))
			{
				if(!_box_count)return game_error;
				//if(!state())return game_win;
				for(t_float_id = _float_box_id+1;t_float_id<_float_box_id+(column-1);t_float_id+=1)
				{
					/* 如果可以填充则返回填充ID */
					if(_box_plane[t_float_id]>fill_box_type)
					{
						t_float_id-=1;
						if(!(t_float_id%column))
						{
							return game_error;
						}
						else
						{
							_box_plane[t_float_id]+=box_switch_type;
							_box_count--;
							return game_push+t_float_id;
						}
					}
				}
			}
			else
			/* 如果滑动方块不是滑动而是摘取 */
			if(!((_float_box_id+1)%column))
			{
				for(t_float_id = _float_box_id-1;t_float_id>_float_box_id+(column-1);t_float_id-=1)
				{
					/* 如果可以摘取则返回摘取ID */
					if(_box_plane[t_float_id]>bind_box_type && _box_plane[t_float_id]<fill_box_type)
					{
						_box_plane[t_float_id]-=box_switch_type;
						_box_count++;
						return game_push;
					}
					else
					/* 如果是固定方块返回错误 */
					if(_box_plane[t_float_id]==bind_box_type)
					{
						return game_error;
					}
				}
			}
			return game_error;
		break;
	}
	return 0;
}
int game_process::state()
{
	int i,c;
	for(i=2;i<5;i++)
	{
		if(_box_plane[_level.value(_level.level(),i)] == fill_box_type)return -1;
	}
	return 0;
}
#endif
