/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{

	for (int i = 0; i < bHEIGHT; i++) {
		for (int j = 0; j < bWIDTH; j++) {
			if ((i == 0) | (j == 0) | (j == 11) | (i == 25)) {
				board[i][j] = boarder;
			}
			else {
				board[i][j] = empty;
			}
		}
	}

	place_piece(board, &current);
}
void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}
void Game::UpdateModel()
{

	const float dt = ft.Mark();
	movementdelaytotal += dt;
	msdelay += dt;
	totalTime += dt;

	if (current.set == 1)
	{
		place_piece(board, &current);
	}
	if (msdelay >= 1+delay)//forcing a movement downward
	{
		msdelay = 0;
		current.command = 0;
		levelcounter++;
	}
	else//players decision
	{

		command(&current);
		movementdelaytotal = 0;

	}
	master_move(&current, board);
	score_board(board, &current, &score, line);


}
void Game::ComposeFrame()
{



	for (int i = 0; i < bHEIGHT; i++)
	{
		for (int j = 0; j < bWIDTH; j++)
		{
			if (board[i][j] == boarder)
			{
				gfx.DrawRectDem(j*celldem+ xpos, i*celldem+ ypos, celldem, celldem, Colors::Blue);

			}
			if (board[i][j] == block)
			{
				gfx.DrawRectDem(j*celldem+ xpos, i*celldem+ypos, celldem, celldem, Colors::MakeRGB(47,79,79));//dimgray

			}
			/////////////////////////////INDIVIDUAL PIECES HERE
			if ((current.position[i][j] == block)&&(current.type=='I'))
			{
				gfx.DrawRectDem(j*celldem + xpos, i*celldem + ypos, celldem, celldem, Colors::MakeRGB(255,140,0));//dark orange
			}
			if ((current.position[i][j] == block) && (current.type == 'Q'))
			{
				gfx.DrawRectDem(j*celldem + xpos, i*celldem + ypos, celldem, celldem, Colors::Yellow);
			}
			if ((current.position[i][j] == block) && (current.type == 'T'))
			{
				gfx.DrawRectDem(j*celldem + xpos, i*celldem + ypos, celldem, celldem, Colors::Cyan);
			}
			if ((current.position[i][j] == block) && (current.type == 'L'))
			{
				gfx.DrawRectDem(j*celldem + xpos, i*celldem + ypos, celldem, celldem, Colors::MakeRGB(255, 165, 0));//orange
			}
			if ((current.position[i][j] == block) && (current.type == 'J'))
			{
				gfx.DrawRectDem(j*celldem + xpos, i*celldem + ypos, celldem, celldem, Colors::Blue);
			}
			if ((current.position[i][j] == block) && (current.type == 'S'))
			{
				gfx.DrawRectDem(j*celldem + xpos, i*celldem + ypos, celldem, celldem, Colors::MakeRGB(255,105,180));//hotpink
			}
			if ((current.position[i][j] == block) && (current.type == 'Z'))
			{
				gfx.DrawRectDem(j*celldem + xpos, i*celldem + ypos, celldem, celldem, Colors::Green);//hotpink
			}

		}

	}
	//gfx.DrawRect(celldem * 2, celldem * 3, celldem * 12, celldem * 27, Colors::LightGray);//inside color
	for (int k = 3; k < 28; k++) {
		for (int i = celldem * 2; i < celldem * 12; i++)
		{
			for (int j = celldem * 3; j < celldem * 27; j++)
			{

				if (celldem * k == j)
				{
					gfx.PutPixel(i, j, Colors::LightGray);
				}

				if (celldem * k == i)
				{
					gfx.PutPixel(i, j, Colors::LightGray);
				}
			}
		}

	}




	SevenSegment totaltimer(200, 0, 1, Colors::Red, gfx);
	totaltimer.print(int(totalTime));



	//totalTime++;
}


//testing git comment

void Game::command(piece *x) {

	//This function converts a keypress into a decimal value to be used to move,drop, and rotate pieces through x->command typedef


	//skey 0x53
	//wkey 0x58
	//akey 0x41
	//dkey 0x44

	//jkey 0x4A
	//kkey 0x4B


	const Keyboard::Event e = wnd.kbd.ReadKey();	// get an event from the queue

	if (e.IsRelease() || e.GetCode() == 0x53)	// check if it is a release event
	{

		if (e.GetCode() == 0x53)// check the event
			x->command = 0;		// respond to  key release event
		if (e.GetCode() == 0x41)// check the event
			x->command = 1;		// respond to  key release event
		if (e.GetCode() == 0x44)// check the event
			x->command = 2;		// respond to  key release event
		if (e.GetCode() == 0x57)// check the event
			x->command = 5;		// respond to  key release event
		if (e.GetCode() == 0x4A)// check the event
			x->command = 3;		// respond to  key release event
		if (e.GetCode() == 0x4B)// check the event
			x->command = 4;		// respond to  key release event
	}
	else
	{
		x->command = 999;
	}

	if (wnd.kbd.KeyIsPressed(0x53))
	{
		//this
		x->command = 0;

	}



}

void Game::shift_stack(char board[][bWIDTH], int line[bHEIGHT]) {
	int i = 0;
	int j = 0;
	int k = 0;
	int zero = 0;
	int one = 1;

	for (i = 0; i < 25; i++) {
		if (line[i] == 10) {
			k = i;
			k = k - 4;
			while (k != 0) {
				for (j = 1; j < 11; j++) {
					if ((i - zero) < 4) {
						break;
					}
					board[i - zero][j] = board[i - one][j];
					board[i - one][j] = empty;
				}
				zero++;
				one++;
				k--;

			}
		}
	}

}
void Game::score_board(char board[bHEIGHT][bWIDTH], piece *x, int *score, int line[bHEIGHT]) {

	int i = 0;
	int j = 0;
	int c = 0;

	while (x->set == 1) {// copy x->position into board
		for (i = 0; i < bHEIGHT; i++) {
			for (j = 0; j < bWIDTH; j++) {
				if (x->position[i][j] == block) {
					board[i][j] = block;
					x->position[i][j] = empty;
				}
			}
		}
		break;
	}

	for (i = 24; i > 4; i--) {
		for (j = 1; j < 11; j++) {
			if (board[i][j] == block) {		//clear line array 10- = things are in there
				c++;
			}
		}
		line[i] = c;
		c = 0;
	}

	shift_stack(board, line);

}
void Game::place_piece(char board[][bWIDTH], piece *x) {

	int type = 99;

	x->command = 999;
	type = (rand() % 7);

	if (type == 0) {
		//drawing an I piece in vertical poistion 0
		x->set = 0;
		x->S = 0;
		x->E = 0;
		x->W = 0;
		x->rl = 0;
		x->rr = 0;
		x->rotation = 0;
		x->type = 'I';
		x->from_left = 6;
		x->from_top = 4;
		x->position[4][4] = block;
		x->position[4][5] = block;
		x->position[4][6] = block;
		x->position[4][7] = block;
	}
	if (type == 1) {
		//drawing a Q square piece
		x->set = 0;
		x->S = 0;
		x->E = 0;
		x->W = 0;
		x->rotation = 0;
		x->type = 'Q';
		x->from_left = 5;
		x->from_top = 4;
		x->position[3][5] = block;
		x->position[3][6] = block;
		x->position[4][5] = block;
		x->position[4][6] = block;
	}
	if (type == 2) {
		//drawing a S piece
		x->set = 0;
		x->S = 0;
		x->E = 0;
		x->W = 0;
		x->rotation = 0;
		x->type = 'S';
		x->from_left = 5;
		x->from_top = 4;
		x->position[4][4] = block;
		x->position[4][5] = block;
		x->position[3][5] = block;
		x->position[3][6] = block;
	}
	if (type == 3) {
		//drawing a T piece
		x->set = 0;
		x->S = 0;
		x->E = 0;
		x->W = 0;
		x->rotation = 0;
		x->type = 'T';
		x->from_left = 5;
		x->from_top = 3;
		x->position[3][4] = block;
		x->position[3][5] = block;
		x->position[3][6] = block;
		x->position[4][5] = block;
	}
	if (type == 4) {
		//drawing a L piece
		x->set = 0;
		x->S = 0;
		x->E = 0;
		x->W = 0;
		x->rotation = 0;
		x->type = 'L';
		x->from_left = 4;
		x->from_top = 4;
		x->position[4][4] = block;
		x->position[3][4] = block;
		x->position[3][5] = block;
		x->position[3][6] = block;
	}
	if (type == 5) {
		//drawing a J piece
		x->set = 0;
		x->S = 0;
		x->E = 0;
		x->W = 0;
		x->rotation = 0;
		x->type = 'J';
		x->from_left = 6;
		x->from_top = 4;
		x->position[3][5] = block;
		x->position[3][4] = block;
		x->position[3][6] = block;
		x->position[4][6] = block;

	}
	if (type == 6) {
		//drawing a Z piece
		x->set = 0;
		x->S = 0;
		x->E = 0;
		x->W = 0;
		x->rotation = 0;
		x->type = 'Z';
		x->from_left = 5;
		x->from_top = 4;
		x->position[4][5] = block;
		x->position[3][5] = block;
		x->position[3][4] = block;
		x->position[4][6] = block;

	}

	//GLOBAL
	x->basedelay = 8;
	x->setdelay = x->basedelay;

}

void Game::music() {
	Beep(658, 125);
	Beep(1320, 500);
	Beep(990, 250);
	Beep(1056, 250);
	Beep(1188, 250);
	Beep(1320, 125);
	Beep(1188, 125);
	Beep(1056, 250);
	Beep(990, 250);
	Beep(880, 500);
	Beep(880, 250);
	Beep(1056, 250);
	Beep(1320, 500);
	Beep(1188, 250);
	Beep(1056, 250);
	Beep(990, 750);
	Beep(1056, 250);
	Beep(1188, 500);
	Beep(1320, 500);
	Beep(1056, 500);
	Beep(880, 500);
	Beep(880, 500);
	Sleep(250);
	Beep(1188, 500);
	Beep(1408, 250);
	Beep(1760, 500);
	Beep(1584, 250);
	Beep(1408, 250);
	Beep(1320, 750);
	Beep(1056, 250);
	Beep(1320, 500);
	Beep(1188, 250);
	Beep(1056, 250);
	Beep(990, 500);
	Beep(990, 250);
	Beep(1056, 250);
	Beep(1188, 500);
	Beep(1320, 500);
	Beep(1056, 500);
	Beep(880, 500);
	Beep(880, 500);
	Sleep(500);
	Beep(1320, 500);
	Beep(990, 250);
	Beep(1056, 250);
	Beep(1188, 250);
	Beep(1320, 125);
	Beep(1188, 125);
	Beep(1056, 250);
	Beep(990, 250);
	Beep(880, 500);
	Beep(880, 250);
	Beep(1056, 250);
	Beep(1320, 500);
	Beep(1188, 250);
	Beep(1056, 250);
	Beep(990, 750);
	Beep(1056, 250);
	Beep(1188, 500);
	Beep(1320, 500);
	Beep(1056, 500);
	Beep(880, 500);
	Beep(880, 500);
	Sleep(250);
	Beep(1188, 500);
	Beep(1408, 250);
	Beep(1760, 500);
	Beep(1584, 250);
	Beep(1408, 250);
	Beep(1320, 750);
	Beep(1056, 250);
	Beep(1320, 500);
	Beep(1188, 250);
	Beep(1056, 250);
	Beep(990, 500);
	Beep(990, 250);
	Beep(1056, 250);
	Beep(1188, 500);
	Beep(1320, 500);
	Beep(1056, 500);
	Beep(880, 500);
	Beep(880, 500);
	Sleep(500);
	Beep(660, 1000);
	Beep(528, 1000);
	Beep(594, 1000);
	Beep(495, 1000);
	Beep(528, 1000);
	Beep(440, 1000);
	Beep(419, 1000);
	Beep(495, 1000);
	Beep(660, 1000);
	Beep(528, 1000);
	Beep(594, 1000);
	Beep(495, 1000);
	Beep(528, 500);
	Beep(660, 500);
	Beep(880, 1000);
	Beep(838, 2000);
	Beep(660, 1000);
	Beep(528, 1000);
	Beep(594, 1000);
	Beep(495, 1000);
	Beep(528, 1000);
	Beep(440, 1000);
	Beep(419, 1000);
	Beep(495, 1000);
	Beep(660, 1000);
	Beep(528, 1000);
	Beep(594, 1000);
	Beep(495, 1000);
	Beep(528, 500);
	Beep(660, 500);
	Beep(880, 1000);
	Beep(838, 2000);
}
void Game::test() {

	//This function is for automating gameplay, for testing, or to just have fun.  Perhaps this will become AI at some point.

	//Move down = 0
	//Move left = 1
	//Move right = 2
	//rotate right = 3

	//board of commands
	//,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	//Test I block position 0
	int test1[10000] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//Test I block position 1
	int test2[10000] = { 3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,3, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,3, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,3, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,3, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//TEST Q block position 0
	int test3[10000] = { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//TEST S block position 0

	//TEST S block position 1

	//TEST Z block position 0

	//TEST Z block position 1

	//TEST T block position 0

	//TEST T block position 1

	//TEST T block position 2

	//TEST T block position 3

	//TEST J block position 0

	//TEST J block position 1

	//TEST J block position 2

	//TEST J block position 3

	//TEST L block position 0

	//TEST L block position 1

	//TEST L block position 2

	//TEST L block position 3

}

//Move functions first checks collision off the pieces, before they are moved, and then set into the board[][] array.


void Game::master_collision(piece *x, char board[][bWIDTH]) {

	if (x->S == 0) {//reset delay 
		x->setdelay = x->basedelay;
	}

	if (x->type == 'I') {
		collision_I(x, board);
	}
	if (x->type == 'Q') {
		collision_Q(x, board);
	}
	if (x->type == 'L') {
		collision_L(x, board);
	}
	if (x->type == 'Z') {
		collision_Z(x, board);
	}
	if (x->type == 'S') {
		collision_S(x, board);
	}
	if (x->type == 'J') {
		collision_J(x, board);
	}
	if (x->type == 'T') {
		collision_T(x, board);
	}

	///////for all pieces



}
void Game::master_move(piece *x, char board[][bWIDTH]) {


	//999 prevents further movement
	if (x->command == 999) {
		return;
	}

	if (x->type == 'I') {
		if (x->command == 5) {
			x->command = 0;
			while (x->set != 1) {
				master_collision(x, board);
				move_I(x);
				master_collision(x, board);


			}
		}
		else {
			if (x->S == 0) {//reset delay 
				x->setdelay = x->basedelay;
			}
			master_collision(x, board);
			move_I(x);
			master_collision(x, board);
		}



	}
	if (x->type == 'Q') {
		if (x->command == 5) {
			x->command = 0;
			while (x->set != 1) {
				master_collision(x, board);
				move_Q(x);
				master_collision(x, board);


			}
		}
		else {
			if (x->S == 0) {//reset delay 
				x->setdelay = x->basedelay;
			}
			master_collision(x, board);
			move_Q(x);
			master_collision(x, board);
		}

	}
	if (x->type == 'S') {
		if (x->command == 5) {
			x->command = 0;
			while (x->set != 1) {
				master_collision(x, board);
				move_S(x);
				master_collision(x, board);


			}
		}
		else {
			if (x->S == 0) {//reset delay 
				x->setdelay = x->basedelay;
			}
			master_collision(x, board);
			move_S(x);
			master_collision(x, board);
		}
	}
	if (x->type == 'L') {
		if (x->command == 5) {
			x->command = 0;
			while (x->set != 1) {
				master_collision(x, board);
				move_L(x);
				master_collision(x, board);


			}
		}
		else {
			if (x->S == 0) {//reset delay 
				x->setdelay = x->basedelay;
			}
			master_collision(x, board);
			move_L(x);
			master_collision(x, board);
		}
	}
	if (x->type == 'J') {
		if (x->command == 5) {
			x->command = 0;
			while (x->set != 1) {
				master_collision(x, board);
				move_J(x);
				master_collision(x, board);


			}
		}
		else {
			if (x->S == 0) {//reset delay 
				x->setdelay = x->basedelay;
			}
			master_collision(x, board);
			move_J(x);
			master_collision(x, board);
		}
	}
	if (x->type == 'Z') {
		if (x->command == 5) {
			x->command = 0;
			while (x->set != 1) {
				master_collision(x, board);
				move_Z(x);
				master_collision(x, board);


			}
		}
		else {
			if (x->S == 0) {//reset delay 
				x->setdelay = x->basedelay;
			}
			master_collision(x, board);
			move_Z(x);
			master_collision(x, board);
		}
	}
	if (x->type == 'T') {
		if (x->command == 5) {
			x->command = 0;
			while (x->set != 1) {
				master_collision(x, board);
				move_T(x);
				master_collision(x, board);


			}
		}
		else {
			if (x->S == 0) {//reset delay 
				x->setdelay = x->basedelay;
			}
			master_collision(x, board);
			move_T(x);
			master_collision(x, board);
		}
	}






}

//I Piece type = 0
void Game::collision_I_rotation(piece *x, char board[][bWIDTH]) {
	x->rr = 0;
	x->rl = 0;


	//I only has two rotation positions, and so we only need to check the top in position one.
	if (((x->command == 3) || (x->command == 4)) && (x->rotation == 0)) {

		if ((board[x->from_top - 1][x->from_left] != empty) || (board[x->from_top - 2][x->from_left] != empty) || (board[x->from_top + 1][x->from_left] != empty)) {// check top
			x->rr = 1;
			x->rl = 1;
		}
		return;
	}
	if ((x->command == 3 | 4) && (x->rotation == 1)) {

		if ((board[x->from_top][x->from_left + 1] != empty)) {// check right
			x->rr = 1;
			x->rl = 1;
		}
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top][x->from_left - 2] != empty)) {// check left
			x->rr = 1;
			x->rl = 1;
		}
		return;
	}

}
void Game::collision_I(piece *x, char board[][bWIDTH]) {

	if (x->command == 3) {
		collision_I_rotation(x, board);
		return;
	}
	if (x->command == 4) {
		collision_I_rotation(x, board);
		return;
	}



	////Rotation = 0

	if (((x->command == 0) || (x->command == 1) || (x->command == 2)) && (x->rotation == 0)) {

		if (board[x->from_top][x->from_left - 3] != empty) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if (board[x->from_top][x->from_left + 2] != empty) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left - 2] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}
	}

	//Rotation = 1 

	if (((x->command == 0) || (x->command == 1) || (x->command == 2)) && (x->rotation == 1)) {
		//check left
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top - 2][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty)) {
			x->W = 1;

		}
		else {
			x->W = 0;
		}
		//check right
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top - 2][x->from_left + 1] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {
			x->E = 1;

		}
		else {
			x->E = 0;
		}
		//check south
		if (board[x->from_top + 2][x->from_left] != empty) {
			x->S = 1;

		}
		else {
			x->S = 0;
		}

	}

	////Type = I Rotation from 0 to 1

	if ((x->command == 3) && (x->rotation == 0)) {

		if ((board[x->from_top - 1][x->from_left] != empty) && (board[x->from_top - 2][x->from_left] != empty)) {// check north
			x->N = 1;
		}
		else {
			x->N = 0;
		}
		if (board[x->from_top + 1][x->from_left] != empty) {// check south
			x->S = 1;
			//prevent set returning now, set should only activate on downward motion;
			return;
		}
		else {
			x->S = 0;
		}
		x->W = 0;
		x->E = 0;



	}

	////Type = I Rotation from 1 to 0

	if ((x->command == 3) && (x->rotation == 1)) {
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top][x->from_left - 2] != empty)) {// check west
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if (board[x->from_top][x->from_left + 1] != empty) {// check east
			x->E = 1;

		}
		else {
			x->E = 0;
		}


	}

	//This checks if the piece should be set or not, so that it can become apart of the board

	if (x->S == 0) {
		x->setdelay = x->basedelay;
	}

	if (x->S == 1) {
		x->setdelay--;
	}
	if ((x->setdelay == 0) && (x->S == 1)) {
		x->set = 1;
	}

}
void Game::move_I_rotate_right(piece *x) {


	if ((x->rotation == 0) && ((x->rr == 0) && (x->rl == 0))) {//clear if rotation is 0
															   //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left - 2] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;

		//set new rotation
		x->rotation = 1;
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		return;
	}

	if ((x->rotation == 1) && ((x->rr == 0) && (x->rl == 0))) {
		//erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		//set new rotation
		x->rotation = 0;
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left - 2] = block;
		x->position[x->from_top][x->from_left + 1] = block;
		return;
	}

}
void Game::move_I(piece *x) {

	if ((x->command == 4) || (x->command == 3)) {
		move_I_rotate_right(x);
		return;
	}


	// Rotation = 0 Movement =  down
	if ((x->command == 0) && (x->rotation == 0) && (x->S == 0) && (x->S == 0)) {
		x->from_top++;

		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 2] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;

		x->position[x->from_top][x->from_left] = block; // middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left - 2] = block;
		x->position[x->from_top][x->from_left + 1] = block;



	}
	// Rotation = 0 Movement =  left
	if ((x->command == 1) && (x->rotation == 0) && (x->W == 0)) {
		x->from_left--;


		x->position[x->from_top][x->from_left] = empty; // middle block
		x->position[x->from_top][x->from_left - 1] = empty; // middle block
		x->position[x->from_top][x->from_left + 1] = empty; // middle block
		x->position[x->from_top][x->from_left + 2] = empty; // middle block

		x->position[x->from_top][x->from_left] = block; // middle block
		x->position[x->from_top][x->from_left - 1] = block; // middle block
		x->position[x->from_top][x->from_left - 2] = block; // middle block
		x->position[x->from_top][x->from_left + 1] = block; // middle block

	}
	//Rotation = 0 Movement =  right
	if ((x->command == 2) && (x->rotation == 0) && (x->E == 0)) {
		x->from_left++;

		x->position[x->from_top][x->from_left] = empty; // middle block
		x->position[x->from_top][x->from_left - 1] = empty; // middle block
		x->position[x->from_top][x->from_left - 2] = empty; // middle block
		x->position[x->from_top][x->from_left - 3] = empty; // middle block

		x->position[x->from_top][x->from_left] = block; // middle block
		x->position[x->from_top][x->from_left - 1] = block; // middle block
		x->position[x->from_top][x->from_left - 2] = block; // middle block
		x->position[x->from_top][x->from_left + 1] = block; // middle block

	}
	//Rotation = 1 Movement = down
	if ((x->command == 0) && (x->rotation == 1) && (x->S == 0)) {
		x->from_top++;

		x->position[x->from_top][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		x->position[x->from_top + 1][x->from_left] = block;

		x->position[x->from_top - 4][x->from_left] = empty;
		x->position[x->from_top - 3][x->from_left] = empty;





	}
	//Rotation = 1 Movement =  left
	if ((x->command == 1) && (x->rotation == 1) && (x->W == 0)) {
		x->from_left--;
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;

		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top + 1][x->from_left + 1] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;

	}
	//Rotation = 1 Movement =  right
	if ((x->command == 2) && (x->rotation == 1) && (x->E == 0)) {
		x->from_left++;
		x->position[x->from_top][x->from_left] = block;
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;

		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top - 2][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left - 1] = empty;

	}
	//Rotation = 0 Movement = rotate
	//
	//
	//
	//if ((x->command == 3) && (x->rotation == 0) && (x->W == 0) && (x->E == 0)) {

	//	x->position[x->from_top][x->from_left] = empty;
	//	x->position[x->from_top][x->from_left - 1] = empty;
	//	x->position[x->from_top][x->from_left - 2] = empty;
	//	x->position[x->from_top][x->from_left + 1] = empty;

	//	x->position[x->from_top][x->from_left] = block;//middle block
	//	x->position[x->from_top + 1][x->from_left] = block;
	//	x->position[x->from_top - 1][x->from_left] = block;
	//	x->position[x->from_top - 2][x->from_left] = block;

	//	// set new rotation position
	//	x->rotation = 1;
	//	return;
	//	//void Game::command so it doesn't interact anymore
	//	x->command = NULL;
	//}
	//Rotation = 1 Movement = rotate

	//
	//
	//
	//if ((x->command == 3) && (x->rotation == 1) && (x->W == 0) && (x->E == 0)) {

	//	x->position[x->from_top][x->from_left] = empty;
	//	x->position[x->from_top - 1][x->from_left] = empty;
	//	x->position[x->from_top - 2][x->from_left] = empty;
	//	x->position[x->from_top + 1][x->from_left] = empty;

	//	x->position[x->from_top][x->from_left] = block;
	//	x->position[x->from_top][x->from_left - 1] = block;
	//	x->position[x->from_top][x->from_left - 2] = block;
	//	x->position[x->from_top][x->from_left + 1] = block;

	//	// set new rotation position
	//	x->rotation = 0;
	//	//void Game::command so it doesn't interact anymore
	//	x->command = NULL;

	//}



}

//Q Piece type = 1
void Game::collision_Q(piece *x, char board[][bWIDTH]) {

	//Type = Q doesn't rotate


	if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty)) {// check left 
		x->W = 1;
	}
	else {
		x->W = 0;
	}


	if ((board[x->from_top][x->from_left + 2] != empty) || (board[x->from_top - 1][x->from_left + 2] != empty)) {// check right
		x->E = 1;
	}
	else {
		x->E = 0;
	}
	if (((board[x->from_top + 1][x->from_left] != empty)) || ((board[x->from_top + 1][x->from_left + 1] != empty))) {// check south
		x->S = 1;
	}
	else {
		x->S = 0;
	}



	//This checks if the piece should be set or not, so that it can become apart of the board
	if (x->S == 0) {
		x->setdelay = x->basedelay;
	}
	if (x->S == 1) {
		x->setdelay--;
	}
	if ((x->setdelay == 0) && (x->S == 1)) {
		x->set = 1;
	}

}
void Game::move_Q(piece *x) {



	//prevent Q from rotating because Q doesn't rotate
	//if ((x->type == 'Q') && ((x->command == 3) || (x->command == 4))) {
	//	x->command = 0;
	//}
	// Movement =  down
	if ((x->type == 'Q') && (x->command == 0) && (x->S == 0)) {
		x->from_top++;

		x->position[x->from_top][x->from_left] = block;
		x->position[x->from_top][x->from_left + 1] = block;

		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;


	}

	// Movement =  left
	if ((x->type == 'Q') && (x->W == 0) && (x->command == 1)) {
		x->from_left--;

		x->position[x->from_top][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;


		x->position[x->from_top - 1][x->from_left + 2] = empty;
		x->position[x->from_top][x->from_left + 2] = empty;



	}

	// Movement =  right
	if ((x->type == 'Q') && (x->command == 2) && (x->E == 0)) {
		x->from_left++;


		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;

		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left - 1] = empty;


	}
}

//S Piece type = 2
void Game::collision_S(piece *x, char board[][bWIDTH]) {


	if (x->command == 3) {
		collision_S_rotation(x, board);
		return;
	}
	if (x->command == 4) {
		collision_S_rotation(x, board);
		return;
	}






	////Rotation = 0

	if (((x->command == 0) || (x->command == 1) || (x->command == 2) || (x->command == 3)) && (x->rotation == 0)) {

		if ((board[x->from_top][x->from_left - 2] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top - 1][x->from_left + 2] != empty) || (board[x->from_top][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty) || (board[x->from_top][x->from_left + 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}
	}


	if (((x->command == 0) || (x->command == 1) || (x->command == 2) || (x->command == 3)) && (x->rotation == 1)) {

		if ((board[x->from_top][x->from_left - 2] != empty) || (board[x->from_top - 1][x->from_left - 2] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 2][x->from_left] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}
	}





	//This checks if the piece should be set or not, so that it can become apart of the board
	if (x->S == 0) {
		x->setdelay = x->basedelay;
	}
	if (x->S == 1) {
		x->setdelay--;
	}
	if ((x->setdelay == 0) && (x->S == 1)) {
		x->set = 1;
	}

}
void Game::collision_S_rotation(piece *x, char board[][bWIDTH]) {
	x->rr = 0;
	x->rl = 0;


	//S only has two rotation positions, and so we only need to check the top in position one.
	if ((x->rotation == 0)) {

		if ((board[x->from_top - 2][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left] != empty)) {// check top
			x->rr = 1;
			x->rl = 1;
		}
		return;
	}
	if ((x->rotation == 1)) {

		if ((board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left] != empty)) {// check right
			x->rr = 1;
			x->rl = 1;
		}
		if ((board[x->from_top + 1][x->from_left - 1] != empty)) {// check below
			x->rr = 1;
			x->rl = 1;
		}
		return;
	}


}
void Game::move_S_rotate_right(piece *x) {

	if ((x->rotation == 0) && ((x->rr == 0) && (x->rl == 0))) {//clear if rotation is 0
															   //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;

		//set new rotation
		x->from_top--;
		x->rotation = 1;
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		return;
	}

	if ((x->rotation == 1) && ((x->rr == 0) && (x->rl == 0))) {
		//erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		//set new rotation
		x->from_top++;
		x->rotation = 0;
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		return;
	}




}
void Game::move_S(piece *x) {

	if ((x->command == 3) || (x->command == 4)) {
		move_S_rotate_right(x);
	}




	// Rotation = 0 Movement =  down
	if ((x->command == 0) && (x->rotation == 0) && (x->S == 0) && (x->S == 0)) {
		x->from_top++;

		x->position[x->from_top][x->from_left] = block;
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;


		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;




	}

	// Rotation = 0 Movement =  right
	if ((x->E == 0) && (x->command == 2) && (x->rotation == 0) && (x->E == 0)) {
		x->from_left++;

		x->position[x->from_top][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;

		x->position[x->from_top][x->from_left - 2] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;


	}

	// Rotation = 0 Movement =  left
	if ((x->W == 0) && (x->command == 1) && (x->rotation == 0) && (x->W == 0)) {
		x->from_left--;

		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;

		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 2] = empty;


	}

	// Rotation = 1 Movement =  down
	if ((x->command == 0) && (x->rotation == 1) && (x->S == 0) && (x->S == 0)) {
		x->from_top++;

		x->position[x->from_top - 1][x->from_left] = empty;//middle
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top - 2][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left] = empty;

		x->position[x->from_top][x->from_left] = block;//middle
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;



	}

	// Rotation = 1 Movement =  left
	if ((x->command == 1) && (x->rotation == 1) && (x->W == 0)) {
		x->from_left--;

		x->position[x->from_top][x->from_left - 1] = block;//middle
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;

		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top + 1][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;

	}

	// Rotation = 1 Movement =  right
	if ((x->command == 2) && (x->rotation == 1) && (x->E == 0)) {
		x->from_left++;

		x->position[x->from_top][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;

		x->position[x->from_top][x->from_left - 2] = empty;
		x->position[x->from_top - 1][x->from_left - 2] = empty;
		x->position[x->from_top + 1][x->from_left - 1] = empty;




	}



}

//T Piece type = 3
void Game::collision_T_rotation(piece * x, char board[][bWIDTH]) {
	x->rr = 0;
	x->rl = 0;

	if (x->rotation == 0) {

		if ((board[x->from_top - 1][x->from_left] != empty)) {
			x->rl = 1;
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 1) {

		if ((board[x->from_top][x->from_left + 1] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top + 1][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 2) {

		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top][x->from_left - 1] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top + 1][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 3) {

		if ((board[x->from_top + 1][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {
			x->rl = 1;
		}
		if (board[x->from_top][x->from_left - 1] != empty) {
			x->rr = 1;
		}
		return;
	}

}
void Game::collision_T(piece *x, char board[][bWIDTH]) {


	if (x->command == 3) {
		collision_T_rotation(x, board);
		return;
	}
	if (x->command == 4) {
		collision_T_rotation(x, board);
		return;
	}

	////Rotation = 0
	if (x->rotation == 0) {

		if ((board[x->from_top][x->from_left - 2] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 2] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 2][x->from_left] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}

	}

	////Rotation = 1
	if (x->rotation == 1) {

		if ((board[x->from_top][x->from_left - 2] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 2][x->from_left] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}

	}

	////Rotation = 2
	if (((x->command == 0) || (x->command == 1) || (x->command == 2) || (x->command == 3)) && (x->rotation == 2)) {

		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left - 2] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top + 1][x->from_left + 2] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 2][x->from_left] != empty) || (board[x->from_top + 2][x->from_left + 1] != empty) || (board[x->from_top + 2][x->from_left - 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}

	}

	////Rotation = 3
	if (((x->command == 0) || (x->command == 1) || (x->command == 2) || (x->command == 3)) && (x->rotation == 3)) {

		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 2] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 2][x->from_left] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}

	}



	//This checks if the piece should be set or not, so that it can become apart of the board
	if (x->S == 0) {
		x->setdelay = x->basedelay;
	}
	if (x->S == 1) {
		x->setdelay--;
	}
	if ((x->setdelay == 0) && (x->S == 1)) {
		x->set = 1;
	}

}
void Game::move_T(piece *x) {

	if ((x->command == 3) || (x->command == 4)) {
		move_T_rotate(x);
		return;
	}

	// Rotation = 0 Movement =  down
	if ((x->command == 0) && (x->rotation == 0) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top + 1][x->from_left] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;

	}
	// Rotation = 0 Movement =  right
	if ((x->command == 2) && (x->rotation == 0) && (x->E == 0)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top + 1][x->from_left] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;

	}
	// Rotation = 0 Movement =  left
	if ((x->command == 1) && (x->rotation == 0) && (x->W == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top + 1][x->from_left] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;

	}


	// Rotation = 1 Movement =  down
	if ((x->command == 0) && (x->rotation == 1) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;

	}
	// Rotation = 1 Movement =  right
	if ((x->command == 2) && (x->rotation == 1) && (x->E == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;

	}
	// Rotation = 1 Movement =  left
	if ((x->command == 1) && (x->rotation == 1) && (x->W == 0)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
	}

	// Rotation = 2 Movement =  down
	if ((x->command == 0) && (x->rotation == 2) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top + 1][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left + 1] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top + 1][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left + 1] = block;

	}
	// Rotation = 2 Movement =  right
	if ((x->command == 2) && (x->rotation == 2) && (x->E == 0)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top + 1][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left + 1] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top + 1][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left + 1] = block;

	}
	// Rotation = 2 Movement =  left
	if ((x->command == 1) && (x->rotation == 2) && (x->W == 0)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top + 1][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left + 1] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top + 1][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left + 1] = block;

	}

	// Rotation = 3 Movement =  down
	if ((x->command == 0) && (x->rotation == 3) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top][x->from_left + 1] = block;

	}
	// Rotation = 3 Movement =  right
	if ((x->command == 2) && (x->rotation == 3) && (x->E == 0)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top][x->from_left + 1] = block;


	}
	// Rotation = 3 Movement =  left
	if ((x->command == 1) && (x->rotation == 3) && (x->W == 0)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top][x->from_left + 1] = block;

	}








	//// Rotation = 0 Movement =  rotateright
	//if ((x->command == 3) && (x->rotation == 0)) {

	//	x->position[x->from_top - 1][x->from_left] = block;

	//	x->position[x->from_top][x->from_left - 1] = empty;

	//	x->rotation = 1;
	//	return;

	//}
	//// Rotation = 1 Movement =  rotateright
	//if ((x->command == 3) && (x->rotation == 1)) {

	//	x->position[x->from_top][x->from_left - 1] = block;

	//	x->position[x->from_top + 1][x->from_left] = empty;

	//	x->rotation = 2;
	//	return;

	//}
	//// Rotation = 2 Movement =  rotateright
	//if ((x->command == 3) && (x->rotation == 2)) {

	//	x->position[x->from_top + 1][x->from_left] = block;

	//	x->position[x->from_top][x->from_left + 1] = empty;

	//	x->rotation = 3;
	//	return;

	//}
	//// Rotation = 3 Movement =  rotateright
	//if ((x->command == 3) && (x->rotation == 3)) {

	//	x->position[x->from_top][x->from_left + 1] = block;

	//	x->position[x->from_top - 1][x->from_left] = empty;

	//	x->rotation = 0;
	//	return;

	//}



}
void Game::move_T_rotate(piece * x) {

	int i = x->rotation;

	if ((x->command == 3) && (x->rl == 1)) {
		return;
	}
	if ((x->command == 4) && (x->rr == 1)) {
		return;
	}

	if (x->command == 3) {
		x->rotation--;
		if (x->rotation == -1) {
			x->rotation = 3;
		}
	}
	if (x->command == 4) {
		x->rotation++;
		if (x->rotation == 4) {
			x->rotation = 0;
		}
	}


	if (i == 0) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top + 1][x->from_left] = empty;
	}
	if (i == 1) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
	}
	if (i == 2) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top + 1][x->from_left - 1] = empty;
		x->position[x->from_top + 1][x->from_left + 1] = empty;
	}
	if (i == 3) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top + 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;
	}


	if (x->rotation == 3) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top][x->from_left + 1] = block;
	}
	if (x->rotation == 2) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top + 1][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left + 1] = block;
	}
	if (x->rotation == 1) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left] = block;
	}
	if (x->rotation == 0) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top + 1][x->from_left] = block;
	}



}

//L Piece type = 4
void Game::collision_L_rotation(piece *x, char board[][bWIDTH]) {
	x->rr = 0;
	x->rl = 0;

	if (x->rotation == 0) {

		if ((board[x->from_top - 2][x->from_left] != empty) || (board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top][x->from_left + 2] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top - 2][x->from_left + 1] != empty) || (board[x->from_top - 2][x->from_left] != empty)) {
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 1) {

		if ((board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top][x->from_left + 1] != empty)) {
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 2) {

		if ((board[x->from_top - 1][x->from_left] != empty) || (board[x->from_top - 2][x->from_left - 1] != empty) || (board[x->from_top - 2][x->from_left] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top - 1][x->from_left] != empty) || (board[x->from_top - 2][x->from_left] != empty)) {
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 3) {

		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty)) {
			x->rr = 1;
		}
		return;
	}


}
void Game::collision_L(piece *x, char board[][bWIDTH]) {

	if (x->command == 3) {
		collision_L_rotation(x, board);
		return;
	}
	if (x->command == 4) {
		collision_L_rotation(x, board);
		return;
	}


	////Rotation = 0

	if (x->rotation == 0) {

		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left + 3] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top][x->from_left + 2] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}
	}

	if (x->rotation == 1) {

		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top - 2][x->from_left - 2] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top - 2][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}
	}

	if (x->rotation == 2) {

		if ((board[x->from_top][x->from_left - 2] != empty) || (board[x->from_top - 1][x->from_left] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 2] != empty) || (board[x->from_top - 1][x->from_left + 2] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}
	}

	if (x->rotation == 3) {

		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top - 2][x->from_left - 1] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 2] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top - 2][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty)) {// check south
			x->S = 1;
		}
		else {
			x->S = 0;
		}
	}








	//This checks if the piece should be set or not, so that it can become apart of the board
	if (x->S == 0) {
		x->setdelay = x->basedelay;
	}
	if (x->S == 1) {
		x->setdelay--;
	}
	if ((x->setdelay == 0) && (x->S == 1)) {
		x->set = 1;
	}

}
void Game::move_L(piece *x) {

	if ((x->command == 3) || (x->command == 4)) {
		move_L_rotate(x);
		return;
	}

	// Rotation = 0 Movement =  down
	if ((x->command == 0) && (x->rotation == 0) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 2] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left + 2] = block;
	}
	// Rotation = 0 Movement =  right
	if ((x->command == 2) && (x->rotation == 0) && (x->E != 1)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 2] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left + 2] = block;
	}
	// Rotation = 0 Movement =  left
	if ((x->command == 1) && (x->rotation == 0) && (x->W != 1)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 2] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left + 2] = block;
	}
	// Rotation = 1 Movement =  left
	if ((x->command == 1) && (x->rotation == 1) && (x->W != 1)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left - 1] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left - 1] = block;
	}
	// Rotation = 1 Movement =  down
	if ((x->command == 0) && (x->rotation == 1) && (x->S == 0)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left - 1] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left - 1] = block;
	}
	// Rotation = 1 Movement =  right
	if ((x->command == 2) && (x->rotation == 1) && (x->E != 1)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left - 1] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left - 1] = block;
	}
	// Rotation = 2 Movement =  left
	if ((x->command == 1) && (x->rotation == 2) && (x->W != 1)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top][x->from_left - 1] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top][x->from_left - 1] = block;
	}
	// Rotation = 2 Movement =  right
	if ((x->command == 2) && (x->rotation == 2) && (x->E != 1)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top][x->from_left - 1] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top][x->from_left - 1] = block;
	}
	// Rotation = 2 Movement =  down
	if ((x->command == 0) && (x->rotation == 2) && (x->S == 0)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top][x->from_left - 1] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top][x->from_left - 1] = block;
	}
	// Rotation = 3 Movement =  down
	if ((x->command == 0) && (x->rotation == 3) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->from_top++;
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
	}
	// Rotation = 3 Movement =  right
	if ((x->command == 2) && (x->rotation == 3) && (x->E != 1)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->from_left++;
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
	}
	// Rotation = 3 Movement =  left
	if ((x->command == 1) && (x->rotation == 3) && (x->W != 1)) {
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->from_left--;
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
	}









}
void Game::move_L_rotate(piece *x) {
	int i = x->rotation;

	if ((x->command == 3) && (x->rl == 1)) {
		return;
	}
	if ((x->command == 4) && (x->rr == 1)) {
		return;
	}

	if (x->command == 3) {
		x->rotation--;
		if (x->rotation == -1) {
			x->rotation = 3;
		}
	}
	if (x->command == 4) {
		x->rotation++;
		if (x->rotation == 4) {
			x->rotation = 0;
		}
	}


	if (i == 0) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 2] = empty;
	}
	if (i == 1) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left - 1] = empty;
	}
	if (i == 2) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top][x->from_left - 1] = empty;
	}
	if (i == 3) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
	}

	//Sets proper rotation so the pieces don't run away!
	if ((i == 0) && (x->rotation == 1)) {
		x->from_left++;
	}
	if ((i == 3) && (x->rotation == 0)) {
		x->from_left--;
	}
	if ((i == 0) && (x->rotation == 3)) {
		x->from_left++;
	}
	if ((i == 1) && (x->rotation == 0)) {
		x->from_left--;
	}
	//

	if (x->rotation == 3) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
	}
	if (x->rotation == 2) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top][x->from_left - 1] = block;
	}
	if (x->rotation == 1) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left - 1] = block;
	}
	if (x->rotation == 0) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top - 1][x->from_left + 2] = block;
	}



}

//J Piece  type = 5
void Game::collision_J_rotation(piece *x, char board[][bWIDTH]) {
	x->rr = 0;
	x->rl = 0;

	if (x->rotation == 0) {

		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 2][x->from_left - 1] != empty) || (board[x->from_top - 2][x->from_left] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top][x->from_left - 2] != empty)) {
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 1) {

		if ((board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top][x->from_left + 1] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top][x->from_left + 1] != empty)) {
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 2) {

		if ((board[x->from_top - 1][x->from_left] != empty) || (board[x->from_top - 2][x->from_left] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top - 1][x->from_left] != empty) || (board[x->from_top - 2][x->from_left] != empty) || (board[x->from_top - 2][x->from_left + 1] != empty)) {
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 3) {

		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top][x->from_left + 1] != empty)) {
			x->rl = 1;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty)) {
			x->rr = 1;
		}
		return;
	}
}
void Game::collision_J(piece *x, char board[][bWIDTH]) {

	if (x->command == 3) {
		collision_J_rotation(x, board);
		return;
	}
	if (x->command == 4) {
		collision_J_rotation(x, board);
		return;
	}

	////////////////
	////Rotation = 0
	////////////////

	if (x->rotation == 0) {
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top][x->from_left - 2] != empty)) {// check down
			x->S = 1;
		}
		else {
			x->S = 0;
		}
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 3] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}

	}

	////////////////
	////Rotation = 1
	////////////////

	if (x->rotation == 1) {
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty)) {// check down
			x->S = 1;
		}
		else {
			x->S = 0;
		}
		if ((board[x->from_top][x->from_left - 2] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top - 2][x->from_left - 1] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top - 2][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}

	}

	////////////////
	////Rotation = 2
	////////////////

	if (x->rotation == 2) {//DOWN
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty) || (board[x->from_top + 1][x->from_left - 1] != empty)) {// check down
			x->S = 1;
		}
		else {
			x->S = 0;
		}
		if ((board[x->from_top][x->from_left - 2] != empty) || (board[x->from_top - 1][x->from_left - 2] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 2] != empty) || (board[x->from_top - 1][x->from_left] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}

	}
	////////////////
	////Rotation = 3
	////////////////

	if (x->rotation == 3) {//DOWN
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty)) {// check down
			x->S = 1;
		}
		else {
			x->S = 0;
		}
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top - 2][x->from_left - 1] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top - 2][x->from_left + 2] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}

	}



}
void Game::move_J(piece *x) {

	if ((x->command == 3) || (x->command == 4)) {
		move_J_rotate(x);
		return;
	}

	// Rotation = 0 Movement =  down
	if ((x->command == 0) && (x->rotation == 0) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 2] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left - 2] = block;

	}
	// Rotation = 0 Movement =  right
	if ((x->command == 2) && (x->rotation == 0) && (x->E == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 2] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left - 2] = block;

	}
	// Rotation = 0 Movement =  left
	if ((x->command == 1) && (x->rotation == 0) && (x->W == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 2] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left - 2] = block;

	}

	////////////////////////

	// Rotation = 1 Movement =  down
	if ((x->command == 0) && (x->rotation == 1) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->from_top++;
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
	}
	// Rotation = 1 Movement =  right
	if ((x->command == 2) && (x->rotation == 1) && (x->E == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->from_left++;
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;

	}
	// Rotation = 1 Movement =  left
	if ((x->command == 1) && (x->rotation == 1) && (x->W == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->from_left--;
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;

	}

	////////////////////////

	// Rotation = 2 Movement =  down
	if ((x->command == 0) && (x->rotation == 2) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;


	}
	// Rotation = 2 Movement =  right
	if ((x->command == 2) && (x->rotation == 2) && (x->E == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;


	}
	// Rotation = 2 Movement =  left
	if ((x->command == 1) && (x->rotation == 2) && (x->W == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;


	}


	////////////////////////

	// Rotation = 3 Movement =  down
	if ((x->command == 0) && (x->rotation == 3) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left + 1] = block;


	}
	// Rotation = 3 Movement =  left
	if ((x->command == 1) && (x->rotation == 3) && (x->W == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left + 1] = block;


	}
	// Rotation = 3 Movement =  right
	if ((x->command == 2) && (x->rotation == 3) && (x->E == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left + 1] = block;


	}

	////////////////////////Loops rotation back to 0


	//This checks if the piece should be set or not, so that it can become apart of the board

	if (x->S == 0) {
		x->setdelay = x->basedelay;
	}

	if (x->S == 1) {
		x->setdelay--;
	}
	if ((x->setdelay == 0) && (x->S == 1)) {
		x->set = 1;
	}


}
void Game::move_J_rotate(piece *x) {
	int i = x->rotation;

	if ((x->command == 3) && (x->rl == 1)) {
		return;
	}
	if ((x->command == 4) && (x->rr == 1)) {
		return;
	}

	if (x->command == 3) {
		x->rotation--;
		if (x->rotation == -1) {
			x->rotation = 3;
		}
	}
	if (x->command == 4) {
		x->rotation++;
		if (x->rotation == 4) {
			x->rotation = 0;
		}
	}

	if (i == 0) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 2] = empty;
	}
	if (i == 1) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
	}
	if (i == 2) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top][x->from_left + 1] = empty;
		x->position[x->from_top][x->from_left - 1] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
	}
	if (i == 3) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;
	}

	//Sets proper rotation so the pieces don't run away!
	if ((i == 0) && (x->rotation == 1)) {
		x->from_left--;
	}
	if ((i == 3) && (x->rotation == 0)) {
		x->from_left++;
	}
	if ((i == 0) && (x->rotation == 3)) {
		x->from_left--;
	}
	if ((i == 1) && (x->rotation == 0)) {
		x->from_left++;
	}


	if (x->rotation == 3) {
		//draw new rotation

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left + 1] = block;
	}
	if (x->rotation == 2) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left + 1] = block;
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
	}
	if (x->rotation == 1) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 2][x->from_left] = block;
	}
	if (x->rotation == 0) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top - 1][x->from_left - 2] = block;
	}
}

//Z Piece type = 6
void Game::collision_Z_rotation(piece *x, char board[][bWIDTH]) {
	x->rr = 0;
	x->rl = 0;

	if (x->rotation == 0) {

		if ((board[x->from_top - 1][x->from_left + 1] != empty) || (board[x->from_top - 2][x->from_left + 1] != empty)) {
			x->rl = 1;
			x->rr = 1;
		}
		return;
	}
	if (x->rotation == 1) {

		if ((board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top][x->from_left + 1] != empty)) {
			x->rl = 1;
			x->rr = 1;
		}
		return;
	}
}
void Game::collision_Z(piece *x, char board[][bWIDTH]) {

	if (x->command == 3) {
		collision_Z_rotation(x, board);
		return;
	}
	if (x->command == 4) {
		collision_Z_rotation(x, board);
		return;
	}

	////////////////
	////Rotation = 0
	////////////////

	if (x->rotation == 0) {//DOWN
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top + 1][x->from_left + 1] != empty) || (board[x->from_top][x->from_left - 1] != empty)) {// check down
			x->S = 1;
		}
		else {
			x->S = 0;
		}
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 2] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 2] != empty) || (board[x->from_top - 1][x->from_left + 1] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}

	}

	////////////////
	////Rotation = 1
	////////////////

	if (x->rotation == 1) {//DOWN
		if ((board[x->from_top + 1][x->from_left] != empty) || (board[x->from_top][x->from_left + 1] != empty)) {// check down
			x->S = 1;
		}
		else {
			x->S = 0;
		}
		if ((board[x->from_top][x->from_left - 1] != empty) || (board[x->from_top - 1][x->from_left - 1] != empty) || (board[x->from_top - 2][x->from_left] != empty)) {// check left
			x->W = 1;
		}
		else {
			x->W = 0;
		}
		if ((board[x->from_top][x->from_left + 1] != empty) || (board[x->from_top - 1][x->from_left + 2] != empty) || (board[x->from_top - 2][x->from_left + 2] != empty)) {// check right
			x->E = 1;
		}
		else {
			x->E = 0;
		}

	}


	//This checks if the piece should be set or not, so that it can become apart of the board

	if (x->S == 0) {
		x->setdelay = x->basedelay;
	}

	if (x->S == 1) {
		x->setdelay--;
	}
	if ((x->setdelay == 0) && (x->S == 1)) {
		x->set = 1;
	}




}
void Game::move_Z(piece *x) {

	if ((x->command == 3) || (x->command == 4)) {
		move_Z_rotate(x);
		return;
	}


	// Rotation = 0 Movement =  down
	if ((x->command == 0) && (x->rotation == 0) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left + 1] = block;

	}
	// Rotation = 0 Movement =  right
	if ((x->command == 2) && (x->rotation == 0) && (x->E == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left + 1] = block;

	}
	// Rotation = 0 Movement =  left
	if ((x->command == 1) && (x->rotation == 0) && (x->W == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left + 1] = block;
	}

	////////////////////////

	// Rotation = 1 Movement =  down
	if ((x->command == 0) && (x->rotation == 1) && (x->S == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;

		x->from_top++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top - 2][x->from_left + 1] = block;


	}
	// Rotation = 1 Movement =  left
	if ((x->command == 1) && (x->rotation == 1) && (x->W == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;

		x->from_left--;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top - 2][x->from_left + 1] = block;


	}
	// Rotation = 1 Movement =  right
	if ((x->command == 2) && (x->rotation == 1) && (x->E == 0)) {

		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;

		x->from_left++;

		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top - 2][x->from_left + 1] = block;

	}



	//This checks if the piece should be set or not, so that it can become apart of the board

	if (x->S == 0) {
		x->setdelay = x->basedelay;
	}

	if (x->S == 1) {
		x->setdelay--;
	}
	if ((x->setdelay == 0) && (x->S == 1)) {
		x->set = 1;
	}



}
void Game::move_Z_rotate(piece *x) {
	int i = x->rotation;

	if ((x->command == 3) && (x->rl == 1)) {
		return;
	}
	if ((x->command == 4) && (x->rr == 1)) {
		return;
	}

	if (x->command == 3) {
		x->rotation--;
		if (x->rotation == -1) {
			x->rotation = 1;
		}
	}
	if (x->command == 4) {
		x->rotation++;
		if (x->rotation == 2) {
			x->rotation = 0;
		}
	}


	if (i == 0) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left - 1] = empty;
		x->position[x->from_top][x->from_left + 1] = empty;
	}
	if (i == 1) { //clear if rotation is 0
				  //erase old rotation
		x->position[x->from_top][x->from_left] = empty;//middle block
		x->position[x->from_top - 1][x->from_left] = empty;
		x->position[x->from_top - 1][x->from_left + 1] = empty;
		x->position[x->from_top - 2][x->from_left + 1] = empty;
	}
	if (x->rotation == 1) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left + 1] = block;
		x->position[x->from_top - 2][x->from_left + 1] = block;
	}
	if (x->rotation == 0) {
		//draw new rotation
		x->position[x->from_top][x->from_left] = block;//middle block
		x->position[x->from_top - 1][x->from_left] = block;
		x->position[x->from_top - 1][x->from_left - 1] = block;
		x->position[x->from_top][x->from_left + 1] = block;
	}






}

