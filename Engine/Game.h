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
#include <chrono>
#include "FrameTimer.h"
#include "SevenSegment.h"

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
	
	//What I am going to model
	//http://tetris.wikia.com/wiki/Tetris_The_Absolute_The_Grand_Master_2
	//NEWTIMERSSYEM
	float _ARE = 25.0f;//measurement is in frames
	float _DAS = 16.0f; //delayed auto shift
	float _LOCK = 30.0f; //lock delay
	float _LINECLEAR = 40.0f;
	float _GRAVITY = 4.0 /256.0f;//rows per frame
	float _GRAVITYEFFECT = 0.0f;

	float _dt = 0.0f;
	float _TOTFRAMECOUNTER = 0.0f;
	float _TOTTIME = 0.0f;

	float _FPSCOUNTER = 0.0f;
	float _FPSFRAMECOUNT = 0.0f;
	float _FPSFRAMETIME = 0.0f;

	unsigned int _SCORE = 0;

	float totalTime = 0;
	unsigned int levelcounter = 0;
	FrameTimer ft;

	float delay = 0;
	float msdelay = 0;
	bool duringmovement = false;

	float movementdelaytotal = 0;
	static constexpr float movementdelay = 0.15f;


	float lockdelay = 0;

	float Piecevx = 0;
	float Piecevy = 0;



	bool isInputBlocked = false;

	static constexpr int bHEIGHT = 26;
	static constexpr int bWIDTH = 12;
	static constexpr char boarder = '*';
	static constexpr char empty = ' ';
	static constexpr char block = 'X';

	static constexpr int celldem = 20;
	static constexpr int cellpadding = 0;
	static constexpr int xpos = celldem;
	static constexpr int ypos = celldem*2;



	int line[bHEIGHT];

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

private:
	/********************************/
	/*  User Functions              */
	/********************************/

	void shiftStack();
	void commitPiecetoBoard();
	void scoreBoard();
	void place_piece();
	void test();
	void command();
	void music();

	//custom
	void calculateScore();


	void master_collision();
	void collision_I_rotation();
	void collision_S_rotation();
	void collision_Z_rotation();
	void collision_L_rotation();
	void collision_J_rotation();
	void collision_T_rotation();
	void collision_I();
	void collision_Q();
	void collision_S();
	void collision_Z();
	void collision_L();
	void collision_J();
	void collision_T();

	void move_I_rotate_right();
	void move_S_rotate_right();
	void move_Z_rotate();
	void move_T_rotate();
	void move_J_rotate();
	void move_L_rotate();

	void master_move();
	void move_I();
	void move_S();
	void move_Z();
	void move_Q();
	void move_L();
	void move_J();
	void move_T();
};