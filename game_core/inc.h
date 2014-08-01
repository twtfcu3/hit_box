#ifndef __INC__HEAD__
#define __INC__HEAD__
const int info_height = 30;
const int row = 8;
const int column = 8;
const int clip_width = 320/row;
const int clip_height= (350-info_height)/column;
const char* bg_path = "img/ball_backgroup.jpg";
const char* mouth_path = "img/mouth.bmp";
//-----------------------------------------
const int float_box_alpha = 150;
const int empty_box_alpha = 255;
const int fill_box_alpha = 200;
const int full_box_alpha = 200;
const int bind_box_alpha = 50;
//-----------------------------------------
const int float_box_type = 1;
const int empty_box_type = 3;
const int fill_box_type = 4;
const int full_box_type = 6;
const int bind_box_type = 7;
const int box_switch_type = 2;
//----------------------------------------
const int move_up = 1;
const int move_down = 2;
const int move_left = 3;
const int move_right = 4;
//----------------------------------------
const int game_error = -1;
const int game_win = 0;
const int game_push = 1000;
const int game_pull = 100;
const int game_float = 10000;
#endif
