/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	/********************************/
	static constexpr int bHEIGHT = 26;
	static constexpr int bWIDTH = 12;
	static constexpr char boarder = '*';
	static constexpr char empty = ' ';
	static constexpr char block = 'X';

	static constexpr int celldem = 10;
	static constexpr int cellpadding = 0;
	static constexpr int xpos = 0;
	static constexpr int ypos = 0;

	int line[bHEIGHT];
	int score = 0;
	int time = 0;
	int level = 1;
	char board[bHEIGHT][bWIDTH];

	typedef struct {
		int test1;
		int test2;

		int N;//prevent up movement
		int S;//prevent down movement
		int E;//prevent right movement
		int W;//prevent left movement
		int rr; //prevent rotate right if rotate right is blocked
		int rl; //prevent rotate left if rotate left is blocked

		int setdelay;
		int basedelay;
		int set;// setting a piece will then copy the array position into the array of the board
		int from_left;//x position to calculate bounds of a block
		int from_top;//y position to calculate bounds of a block
		int rotation;//Left and right 
		char type;//I,Q,S,Z,L,J,T
		int command;//0 move down 1 move left 2 move right 3 rotate left 4 rotate right 5 instant drop
		char position[bHEIGHT][bWIDTH];// position in board, but not the board

	}piece;

	piece current;

	void clearScreen();
	void print_board(char board[bHEIGHT][bWIDTH], piece *x, int *score, int time, int line[bHEIGHT]);
	void shift_stack(char board[][bWIDTH], int line[bHEIGHT]);
	void score_board(char board[bHEIGHT][bWIDTH], piece *x, int *score, int line[bHEIGHT]);
	void place_piece(char board[][bWIDTH], piece *x);
	void test();
	void command(piece *x);
	void music();

	void master_collision(piece *x, char board[][bWIDTH]);
	void collision_I_rotation(piece *x, char board[][bWIDTH]);
	void collision_S_rotation(piece *x, char board[][bWIDTH]);
	void collision_Z_rotation(piece *x, char board[][bWIDTH]);
	void collision_L_rotation(piece *x, char board[][bWIDTH]);
	void collision_J_rotation(piece *x, char board[][bWIDTH]);
	void collision_T_rotation(piece *x, char board[][bWIDTH]);
	void collision_I(piece *x, char board[][bWIDTH]);
	void collision_Q(piece *x, char board[][bWIDTH]);
	void collision_S(piece *x, char board[][bWIDTH]);
	void collision_Z(piece *x, char board[][bWIDTH]);
	void collision_L(piece *x, char board[][bWIDTH]);
	void collision_J(piece *x, char board[][bWIDTH]);
	void collision_T(piece *x, char board[][bWIDTH]);

	void move_I_rotate_right(piece *x);
	void move_S_rotate_right(piece *x);
	void move_Z_rotate(piece *x);
	void move_T_rotate(piece *x);
	void move_J_rotate(piece *x);
	void move_L_rotate(piece *x);

	void master_move(piece *x, char board[][bWIDTH]);
	void move_I(piece *x);
	void move_S(piece *x);
	void move_Z(piece *x);
	void move_Q(piece *x);
	void move_L(piece *x);
	void move_J(piece *x);
	void move_T(piece *x);
};