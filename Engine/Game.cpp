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

	for (int i = 0; i < bHEIGHT; i++)
	{
		line[i] = 0;
	}

	place_piece();
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
	_TOTTIME += dt;
	_FPSFRAMETIME += dt;


	_FPSCOUNTER = _FPSFRAMECOUNT / _FPSFRAMETIME;
	_FPSFRAMETIME = 0;
	_FPSFRAMECOUNT = 0;
	_dt = dt;
	_GRAVITYEFFECT += _GRAVITY;

	if (current.set == 1)
	{
		place_piece();
	}
	else//players decision
	{
		command();
		movementdelaytotal = 0;
	}

	master_move();
	scoreBoard();
	shiftStack();
	calculateScore();
	
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
				gfx.DrawRectDem(j*celldem + xpos, i*celldem + ypos, celldem, celldem, Colors::MakeRGB(153,0,0));//dark orange
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
					gfx.PutPixel(i, j, Colors::Black);
				}

				if (celldem * k == i)
				{
					gfx.PutPixel(i, j, Colors::Black);
				}
			}
		}

	}


	
	gfx.DrawRectDem(300, 50, 200, 200, Colors::LightGray);

	SevenSegment framecounter(300, 50, 1, Colors::Red, gfx);
	framecounter.print(int(_TOTFRAMECOUNTER));

	SevenSegment totaltimer(300, 100, 1, Colors::Red, gfx);
	totaltimer.print(int(_TOTTIME));

	SevenSegment FPS(300, 150, 1, Colors::Blue, gfx);
	FPS.print(int(_FPSCOUNTER));

	SevenSegment SCORE(300, 200, 1, Colors::Blue, gfx);
	SCORE.print(int(_SCORE));



	_FPSFRAMECOUNT++;
	_TOTFRAMECOUNTER++;
}

void Game::command() {

	//This function converts a keypress into a decimal value to be used to move,drop, and rotate pieces through current.command typedef


	//skey 0x53
	//wkey 0x58
	//akey 0x41
	//dkey 0x44

	//jkey 0x4A
	//kkey 0x4B


	const Keyboard::Event e = wnd.kbd.ReadKey();	// get an event from the queue

	if (e.IsRelease() || e.GetCode() == 0x53)	// check if it is a release event
	{
		//
		if (e.GetCode() == 0x53)// check the event
			current.command = 0;		// respond to  key release event
		if (e.GetCode() == 0x41)// check the event
			current.command = 1;		// respond to  key release event
		if (e.GetCode() == 0x44)// check the event
			current.command = 2;		// respond to  key release event
		if (e.GetCode() == 0x57)// check the event
			current.command = 5;		// respond to  key release event
		if (e.GetCode() == 0x4A)// check the event
			current.command = 3;		// respond to  key release event
		if (e.GetCode() == 0x4B)// check the event
			current.command = 4;		// respond to  key release event
	}
	else
	{
		current.command = 999;
	}
	
	if (_GRAVITYEFFECT >= 1)
	{
		current.command = 0;
		_GRAVITYEFFECT = 0.0f;
	}
	if (wnd.kbd.KeyIsPressed(0x53))
	{
		//this
		current.command = 0;

	}



}
void Game::calculateScore()
{
	//calculate how much score you get if line[n] == 10 then we have a point
	{
		int score = 0;
		for (int i = 0; i < bHEIGHT; i++)
		{
			if (line[i] == 10)
			{
				score++;

			}
		}
		this->_SCORE += score;

	}
}
void Game::shiftStack() {




	//shift stack part
	
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
			line[i] = 0;
		}
	}
	
}
void Game::commitPiecetoBoard()
{
	if (current.set == 1)
	{


	}
}
void Game::scoreBoard() {

	int i = 0;
	int j = 0;
	int c = 0;

	while (current.set == 1) {// copy current.position into board
		for (i = 0; i < bHEIGHT; i++) {
			for (j = 0; j < bWIDTH; j++) {
				if (current.position[i][j] == block) {
					board[i][j] = block;
					current.position[i][j] = empty;
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



}
void Game::place_piece() {

	int type = 99;

	current.command = 999;
	type = (rand() % 7);

	if (type == 0) {
		//drawing an I piece in vertical poistion 0
		current.set = 0;
		current.S = 0;
		current.E = 0;
		current.W = 0;
		current.rl = 0;
		current.rr = 0;
		current.rotation = 0;
		current.type = 'I';
		current.from_left = 6;
		current.from_top = 4;
		current.position[4][4] = block;
		current.position[4][5] = block;
		current.position[4][6] = block;
		current.position[4][7] = block;
	}
	if (type == 1) {
		//drawing a Q square piece
		current.set = 0;
		current.S = 0;
		current.E = 0;
		current.W = 0;
		current.rotation = 0;
		current.type = 'Q';
		current.from_left = 5;
		current.from_top = 4;
		current.position[3][5] = block;
		current.position[3][6] = block;
		current.position[4][5] = block;
		current.position[4][6] = block;
	}
	if (type == 2) {
		//drawing a S piece
		current.set = 0;
		current.S = 0;
		current.E = 0;
		current.W = 0;
		current.rotation = 0;
		current.type = 'S';
		current.from_left = 5;
		current.from_top = 4;
		current.position[4][4] = block;
		current.position[4][5] = block;
		current.position[3][5] = block;
		current.position[3][6] = block;
	}
	if (type == 3) {
		//drawing a T piece
		current.set = 0;
		current.S = 0;
		current.E = 0;
		current.W = 0;
		current.rotation = 0;
		current.type = 'T';
		current.from_left = 5;
		current.from_top = 3;
		current.position[3][4] = block;
		current.position[3][5] = block;
		current.position[3][6] = block;
		current.position[4][5] = block;
	}
	if (type == 4) {
		//drawing a L piece
		current.set = 0;
		current.S = 0;
		current.E = 0;
		current.W = 0;
		current.rotation = 0;
		current.type = 'L';
		current.from_left = 4;
		current.from_top = 4;
		current.position[4][4] = block;
		current.position[3][4] = block;
		current.position[3][5] = block;
		current.position[3][6] = block;
	}
	if (type == 5) {
		//drawing a J piece
		current.set = 0;
		current.S = 0;
		current.E = 0;
		current.W = 0;
		current.rotation = 0;
		current.type = 'J';
		current.from_left = 6;
		current.from_top = 4;
		current.position[3][5] = block;
		current.position[3][4] = block;
		current.position[3][6] = block;
		current.position[4][6] = block;

	}
	if (type == 6) {
		//drawing a Z piece
		current.set = 0;
		current.S = 0;
		current.E = 0;
		current.W = 0;
		current.rotation = 0;
		current.type = 'Z';
		current.from_left = 5;
		current.from_top = 4;
		current.position[4][5] = block;
		current.position[3][5] = block;
		current.position[3][4] = block;
		current.position[4][6] = block;

	}

	//GLOBAL
	current.basedelay = 8;
	current.setdelay = _LOCK;//current.basedelay;

}



//Move functions first checks collision off the pieces, before they are moved, and then set into the board[][] array.


void Game::master_collision() {

	if (current.S == 0) {//reset delay 
		current.setdelay = _LOCK;//current.basedelay;
	}

	if (current.type == 'I') {
		collision_I();
	}
	if (current.type == 'Q') {
		collision_Q();
	}
	if (current.type == 'L') {
		collision_L();
	}
	if (current.type == 'Z') {
		collision_Z();
	}
	if (current.type == 'S') {
		collision_S();
	}
	if (current.type == 'J') {
		collision_J();
	}
	if (current.type == 'T') {
		collision_T();
	}

	///////for all pieces



}
void Game::master_move() {


	//999 prevents further movement
	if (current.command == 999) {
		return;
	}

	if (current.type == 'I') {
		if (current.command == 5) {
			current.command = 0;
			while (current.set != 1) {
				master_collision();
				move_I();
				master_collision();


			}
		}
		else {
			if (current.S == 0) {//reset delay 
				current.setdelay = _LOCK;//current.basedelay;
			}
			master_collision();
			move_I();
			master_collision();
		}



	}
	if (current.type == 'Q') {
		if (current.command == 5) {
			current.command = 0;
			while (current.set != 1) {
				master_collision();
				move_Q();
				master_collision();


			}
		}
		else {
			if (current.S == 0) {//reset delay 
				current.setdelay = _LOCK;//current.basedelay;
			}
			master_collision();
			move_Q();
			master_collision();
		}

	}
	if (current.type == 'S') {
		if (current.command == 5) {
			current.command = 0;
			while (current.set != 1) {
				master_collision();
				move_S();
				master_collision();


			}
		}
		else {
			if (current.S == 0) {//reset delay 
				current.setdelay = _LOCK;//current.basedelay;
			}
			master_collision();
			move_S();
			master_collision();
		}
	}
	if (current.type == 'L') {
		if (current.command == 5) {
			current.command = 0;
			while (current.set != 1) {
				master_collision();
				move_L();
				master_collision();


			}
		}
		else {
			if (current.S == 0) {//reset delay 
				current.setdelay = _LOCK;//current.basedelay;
			}
			master_collision();
			move_L();
			master_collision();
		}
	}
	if (current.type == 'J') {
		if (current.command == 5) {
			current.command = 0;
			while (current.set != 1) {
				master_collision();
				move_J();
				master_collision();


			}
		}
		else {
			if (current.S == 0) {//reset delay 
				current.setdelay = _LOCK;//current.basedelay;
			}
			master_collision();
			move_J();
			master_collision();
		}
	}
	if (current.type == 'Z') {
		if (current.command == 5) {
			current.command = 0;
			while (current.set != 1) {
				master_collision();
				move_Z();
				master_collision();


			}
		}
		else {
			if (current.S == 0) {//reset delay 
				current.setdelay = _LOCK;//current.basedelay;
			}
			master_collision();
			move_Z();
			master_collision();
		}
	}
	if (current.type == 'T') {
		if (current.command == 5) {
			current.command = 0;
			while (current.set != 1) {
				master_collision();
				move_T();
				master_collision();


			}
		}
		else {
			if (current.S == 0) {//reset delay 
				current.setdelay = _LOCK;//current.basedelay;
			}
			master_collision();
			move_T();
			master_collision();
		}
	}






}

//I Piece type = 0
void Game::collision_I_rotation() {
	current.rr = 0;
	current.rl = 0;


	//I only has two rotation positions, and so we only need to check the top in position one.
	if (((current.command == 3) || (current.command == 4)) && (current.rotation == 0)) {

		if ((board[current.from_top - 1][current.from_left] != empty) || (board[current.from_top - 2][current.from_left] != empty) || (board[current.from_top + 1][current.from_left] != empty)) {// check top
			current.rr = 1;
			current.rl = 1;
		}
		return;
	}
	if ((current.command == (3 | 4)) && (current.rotation == 1)) {

		if ((board[current.from_top][current.from_left + 1] != empty)) {// check right
			current.rr = 1;
			current.rl = 1;
		}
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top][current.from_left - 2] != empty)) {// check left
			current.rr = 1;
			current.rl = 1;
		}
		return;
	}

}
void Game::collision_I() {

	if (current.command == 3) {
		collision_I_rotation();
		return;
	}
	if (current.command == 4) {
		collision_I_rotation();
		return;
	}



	////Rotation = 0

	if (((current.command == 0) || (current.command == 1) || (current.command == 2)) && (current.rotation == 0)) {

		if (board[current.from_top][current.from_left - 3] != empty) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if (board[current.from_top][current.from_left + 2] != empty) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left - 2] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}
	}

	//Rotation = 1 

	if (((current.command == 0) || (current.command == 1) || (current.command == 2)) && (current.rotation == 1)) {
		//check left
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top - 2][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty)) {
			current.W = 1;

		}
		else {
			current.W = 0;
		}
		//check right
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top - 2][current.from_left + 1] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {
			current.E = 1;

		}
		else {
			current.E = 0;
		}
		//check south
		if (board[current.from_top + 2][current.from_left] != empty) {
			current.S = 1;

		}
		else {
			current.S = 0;
		}

	}

	////Type = I Rotation from 0 to 1

	if ((current.command == 3) && (current.rotation == 0)) {

		if ((board[current.from_top - 1][current.from_left] != empty) && (board[current.from_top - 2][current.from_left] != empty)) {// check north
			current.N = 1;
		}
		else {
			current.N = 0;
		}
		if (board[current.from_top + 1][current.from_left] != empty) {// check south
			current.S = 1;
			//prevent set returning now, set should only activate on downward motion;
			return;
		}
		else {
			current.S = 0;
		}
		current.W = 0;
		current.E = 0;



	}

	////Type = I Rotation from 1 to 0

	if ((current.command == 3) && (current.rotation == 1)) {
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top][current.from_left - 2] != empty)) {// check west
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if (board[current.from_top][current.from_left + 1] != empty) {// check east
			current.E = 1;

		}
		else {
			current.E = 0;
		}


	}

	//This checks if the piece should be set or not, so that it can become apart of the board

	if (current.S == 0) {
		current.setdelay = _LOCK;//current.basedelay;
	}

	if (current.S == 1) {
		current.setdelay -= _dt;
	}
	if ((current.setdelay <= 0) && (current.S == 1)) {
		current.set = 1;
	}

}
void Game::move_I_rotate_right() {


	if ((current.rotation == 0) && ((current.rr == 0) && (current.rl == 0))) {//clear if rotation is 0
															   //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left - 2] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;

		//set new rotation
		current.rotation = 1;
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		return;
	}

	if ((current.rotation == 1) && ((current.rr == 0) && (current.rl == 0))) {
		//erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		//set new rotation
		current.rotation = 0;
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left - 2] = block;
		current.position[current.from_top][current.from_left + 1] = block;
		return;
	}

}
void Game::move_I() {

	if ((current.command == 4) || (current.command == 3)) {
		move_I_rotate_right();
		return;
	}


	// Rotation = 0 Movement =  down
	if ((current.command == 0) && (current.rotation == 0) && (current.S == 0) && (current.S == 0)) {
		current.from_top++;

		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 2] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;

		current.position[current.from_top][current.from_left] = block; // middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left - 2] = block;
		current.position[current.from_top][current.from_left + 1] = block;



	}
	// Rotation = 0 Movement =  left
	if ((current.command == 1) && (current.rotation == 0) && (current.W == 0)) {
		current.from_left--;


		current.position[current.from_top][current.from_left] = empty; // middle block
		current.position[current.from_top][current.from_left - 1] = empty; // middle block
		current.position[current.from_top][current.from_left + 1] = empty; // middle block
		current.position[current.from_top][current.from_left + 2] = empty; // middle block

		current.position[current.from_top][current.from_left] = block; // middle block
		current.position[current.from_top][current.from_left - 1] = block; // middle block
		current.position[current.from_top][current.from_left - 2] = block; // middle block
		current.position[current.from_top][current.from_left + 1] = block; // middle block

	}
	//Rotation = 0 Movement =  right
	if ((current.command == 2) && (current.rotation == 0) && (current.E == 0)) {
		current.from_left++;

		current.position[current.from_top][current.from_left] = empty; // middle block
		current.position[current.from_top][current.from_left - 1] = empty; // middle block
		current.position[current.from_top][current.from_left - 2] = empty; // middle block
		current.position[current.from_top][current.from_left - 3] = empty; // middle block

		current.position[current.from_top][current.from_left] = block; // middle block
		current.position[current.from_top][current.from_left - 1] = block; // middle block
		current.position[current.from_top][current.from_left - 2] = block; // middle block
		current.position[current.from_top][current.from_left + 1] = block; // middle block

	}
	//Rotation = 1 Movement = down
	if ((current.command == 0) && (current.rotation == 1) && (current.S == 0)) {
		current.from_top++;

		current.position[current.from_top][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		current.position[current.from_top + 1][current.from_left] = block;

		current.position[current.from_top - 4][current.from_left] = empty;
		current.position[current.from_top - 3][current.from_left] = empty;
	}
	//Rotation = 1 Movement =  left
	if ((current.command == 1) && (current.rotation == 1) && (current.W == 0)) {
		current.from_left--;
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;

		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top + 1][current.from_left + 1] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;

	}
	//Rotation = 1 Movement =  right
	if ((current.command == 2) && (current.rotation == 1) && (current.E == 0)) {
		current.from_left++;
		current.position[current.from_top][current.from_left] = block;
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;

		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top - 2][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left - 1] = empty;

	}
}

//Q Piece type = 1
void Game::collision_Q() {

	//Type = Q doesn't rotate


	if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty)) {// check left 
		current.W = 1;
	}
	else {
		current.W = 0;
	}


	if ((board[current.from_top][current.from_left + 2] != empty) || (board[current.from_top - 1][current.from_left + 2] != empty)) {// check right
		current.E = 1;
	}
	else {
		current.E = 0;
	}
	if (((board[current.from_top + 1][current.from_left] != empty)) || ((board[current.from_top + 1][current.from_left + 1] != empty))) {// check south
		current.S = 1;
	}
	else {
		current.S = 0;
	}


	//This checks if the piece should be set or not, so that it can become apart of the board

	if (current.S == 0) {
		current.setdelay = _LOCK;//current.basedelay;
	}

	if (current.S == 1) {
		current.setdelay -= _dt;
	}
	if ((current.setdelay <= 0) && (current.S == 1)) {
		current.set = 1;
	}

}
void Game::move_Q() {



	//prevent Q from rotating because Q doesn't rotate
	//if ((current.type == 'Q') && ((current.command == 3) || (current.command == 4))) {
	//	current.command = 0;
	//}
	// Movement =  down
	if ((current.type == 'Q') && (current.command == 0) && (current.S == 0)) {
		current.from_top++;

		current.position[current.from_top][current.from_left] = block;
		current.position[current.from_top][current.from_left + 1] = block;

		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;


	}

	// Movement =  left
	if ((current.type == 'Q') && (current.W == 0) && (current.command == 1)) {
		current.from_left--;

		current.position[current.from_top][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;


		current.position[current.from_top - 1][current.from_left + 2] = empty;
		current.position[current.from_top][current.from_left + 2] = empty;



	}

	// Movement =  right
	if ((current.type == 'Q') && (current.command == 2) && (current.E == 0)) {
		current.from_left++;


		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;

		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left - 1] = empty;


	}
}

//S Piece type = 2
void Game::collision_S() {


	if (current.command == 3) {
		collision_S_rotation();
		return;
	}
	if (current.command == 4) {
		collision_S_rotation();
		return;
	}






	////Rotation = 0

	if (((current.command == 0) || (current.command == 1) || (current.command == 2) || (current.command == 3)) && (current.rotation == 0)) {

		if ((board[current.from_top][current.from_left - 2] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top - 1][current.from_left + 2] != empty) || (board[current.from_top][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty) || (board[current.from_top][current.from_left + 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}
	}


	if (((current.command == 0) || (current.command == 1) || (current.command == 2) || (current.command == 3)) && (current.rotation == 1)) {

		if ((board[current.from_top][current.from_left - 2] != empty) || (board[current.from_top - 1][current.from_left - 2] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 2][current.from_left] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}
	}



	//This checks if the piece should be set or not, so that it can become apart of the board

	if (current.S == 0) {
		current.setdelay = _LOCK;//current.basedelay;
	}

	if (current.S == 1) {
		current.setdelay -= _dt;
	}
	if ((current.setdelay <= 0) && (current.S == 1)) {
		current.set = 1;
	}

}
void Game::collision_S_rotation() {
	current.rr = 0;
	current.rl = 0;


	//S only has two rotation positions, and so we only need to check the top in position one.
	if ((current.rotation == 0)) {

		if ((board[current.from_top - 2][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left] != empty)) {// check top
			current.rr = 1;
			current.rl = 1;
		}
		return;
	}
	if ((current.rotation == 1)) {

		if ((board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left] != empty)) {// check right
			current.rr = 1;
			current.rl = 1;
		}
		if ((board[current.from_top + 1][current.from_left - 1] != empty)) {// check below
			current.rr = 1;
			current.rl = 1;
		}
		return;
	}


}
void Game::move_S_rotate_right() {

	if ((current.rotation == 0) && ((current.rr == 0) && (current.rl == 0))) {//clear if rotation is 0
															   //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;

		//set new rotation
		current.from_top--;
		current.rotation = 1;
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		return;
	}

	if ((current.rotation == 1) && ((current.rr == 0) && (current.rl == 0))) {
		//erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		//set new rotation
		current.from_top++;
		current.rotation = 0;
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		return;
	}




}
void Game::move_S() {

	if ((current.command == 3) || (current.command == 4)) {
		move_S_rotate_right();
	}




	// Rotation = 0 Movement =  down
	if ((current.command == 0) && (current.rotation == 0) && (current.S == 0) && (current.S == 0)) {
		current.from_top++;

		current.position[current.from_top][current.from_left] = block;
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;


		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;




	}

	// Rotation = 0 Movement =  right
	if ((current.E == 0) && (current.command == 2) && (current.rotation == 0) && (current.E == 0)) {
		current.from_left++;

		current.position[current.from_top][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;

		current.position[current.from_top][current.from_left - 2] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;


	}

	// Rotation = 0 Movement =  left
	if ((current.W == 0) && (current.command == 1) && (current.rotation == 0) && (current.W == 0)) {
		current.from_left--;

		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;

		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 2] = empty;


	}

	// Rotation = 1 Movement =  down
	if ((current.command == 0) && (current.rotation == 1) && (current.S == 0) && (current.S == 0)) {
		current.from_top++;

		current.position[current.from_top - 1][current.from_left] = empty;//middle
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top - 2][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left] = empty;

		current.position[current.from_top][current.from_left] = block;//middle
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;



	}

	// Rotation = 1 Movement =  left
	if ((current.command == 1) && (current.rotation == 1) && (current.W == 0)) {
		current.from_left--;

		current.position[current.from_top][current.from_left - 1] = block;//middle
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;

		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top + 1][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;

	}

	// Rotation = 1 Movement =  right
	if ((current.command == 2) && (current.rotation == 1) && (current.E == 0)) {
		current.from_left++;

		current.position[current.from_top][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;

		current.position[current.from_top][current.from_left - 2] = empty;
		current.position[current.from_top - 1][current.from_left - 2] = empty;
		current.position[current.from_top + 1][current.from_left - 1] = empty;




	}



}

//T Piece type = 3
void Game::collision_T_rotation() {
	current.rr = 0;
	current.rl = 0;

	if (current.rotation == 0) {

		if ((board[current.from_top - 1][current.from_left] != empty)) {
			current.rl = 1;
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 1) {

		if ((board[current.from_top][current.from_left + 1] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top + 1][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 2) {

		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top][current.from_left - 1] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top + 1][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 3) {

		if ((board[current.from_top + 1][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {
			current.rl = 1;
		}
		if (board[current.from_top][current.from_left - 1] != empty) {
			current.rr = 1;
		}
		return;
	}

}
void Game::collision_T() {


	if (current.command == 3) {
		collision_T_rotation();
		return;
	}
	if (current.command == 4) {
		collision_T_rotation();
		return;
	}

	////Rotation = 0
	if (current.rotation == 0) {

		if ((board[current.from_top][current.from_left - 2] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 2] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 2][current.from_left] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}

	}

	////Rotation = 1
	if (current.rotation == 1) {

		if ((board[current.from_top][current.from_left - 2] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 2][current.from_left] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}

	}

	////Rotation = 2
	if (((current.command == 0) || (current.command == 1) || (current.command == 2) || (current.command == 3)) && (current.rotation == 2)) {

		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left - 2] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top + 1][current.from_left + 2] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 2][current.from_left] != empty) || (board[current.from_top + 2][current.from_left + 1] != empty) || (board[current.from_top + 2][current.from_left - 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}

	}

	////Rotation = 3
	if (((current.command == 0) || (current.command == 1) || (current.command == 2) || (current.command == 3)) && (current.rotation == 3)) {

		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 2] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 2][current.from_left] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}

	}



	//This checks if the piece should be set or not, so that it can become apart of the board

	if (current.S == 0) {
		current.setdelay = _LOCK;//current.basedelay;
	}

	if (current.S == 1) {
		current.setdelay -= _dt;
	}
	if ((current.setdelay <= 0) && (current.S == 1)) {
		current.set = 1;
	}
}
void Game::move_T() {

	if ((current.command == 3) || (current.command == 4)) {
		move_T_rotate();
		return;
	}

	// Rotation = 0 Movement =  down
	if ((current.command == 0) && (current.rotation == 0) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top + 1][current.from_left] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;

	}
	// Rotation = 0 Movement =  right
	if ((current.command == 2) && (current.rotation == 0) && (current.E == 0)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top + 1][current.from_left] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;

	}
	// Rotation = 0 Movement =  left
	if ((current.command == 1) && (current.rotation == 0) && (current.W == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top + 1][current.from_left] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;

	}


	// Rotation = 1 Movement =  down
	if ((current.command == 0) && (current.rotation == 1) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;

	}
	// Rotation = 1 Movement =  right
	if ((current.command == 2) && (current.rotation == 1) && (current.E == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;

	}
	// Rotation = 1 Movement =  left
	if ((current.command == 1) && (current.rotation == 1) && (current.W == 0)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
	}

	// Rotation = 2 Movement =  down
	if ((current.command == 0) && (current.rotation == 2) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top + 1][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left + 1] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top + 1][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left + 1] = block;

	}
	// Rotation = 2 Movement =  right
	if ((current.command == 2) && (current.rotation == 2) && (current.E == 0)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top + 1][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left + 1] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top + 1][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left + 1] = block;

	}
	// Rotation = 2 Movement =  left
	if ((current.command == 1) && (current.rotation == 2) && (current.W == 0)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top + 1][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left + 1] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top + 1][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left + 1] = block;

	}

	// Rotation = 3 Movement =  down
	if ((current.command == 0) && (current.rotation == 3) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top][current.from_left + 1] = block;

	}
	// Rotation = 3 Movement =  right
	if ((current.command == 2) && (current.rotation == 3) && (current.E == 0)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top][current.from_left + 1] = block;


	}
	// Rotation = 3 Movement =  left
	if ((current.command == 1) && (current.rotation == 3) && (current.W == 0)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top][current.from_left + 1] = block;

	}


}
void Game::move_T_rotate() {

	int i = current.rotation;

	if ((current.command == 3) && (current.rl == 1)) {
		return;
	}
	if ((current.command == 4) && (current.rr == 1)) {
		return;
	}

	if (current.command == 3) {
		current.rotation--;
		if (current.rotation == -1) {
			current.rotation = 3;
		}
	}
	if (current.command == 4) {
		current.rotation++;
		if (current.rotation == 4) {
			current.rotation = 0;
		}
	}


	if (i == 0) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top + 1][current.from_left] = empty;
	}
	if (i == 1) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
	}
	if (i == 2) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top + 1][current.from_left - 1] = empty;
		current.position[current.from_top + 1][current.from_left + 1] = empty;
	}
	if (i == 3) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top + 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;
	}


	if (current.rotation == 3) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top][current.from_left + 1] = block;
	}
	if (current.rotation == 2) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top + 1][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left + 1] = block;
	}
	if (current.rotation == 1) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left] = block;
	}
	if (current.rotation == 0) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top + 1][current.from_left] = block;
	}



}

//L Piece type = 4
void Game::collision_L_rotation() {
	current.rr = 0;
	current.rl = 0;

	if (current.rotation == 0) {

		if ((board[current.from_top - 2][current.from_left] != empty) || (board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top][current.from_left + 2] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top - 2][current.from_left + 1] != empty) || (board[current.from_top - 2][current.from_left] != empty)) {
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 1) {

		if ((board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top][current.from_left + 1] != empty)) {
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 2) {

		if ((board[current.from_top - 1][current.from_left] != empty) || (board[current.from_top - 2][current.from_left - 1] != empty) || (board[current.from_top - 2][current.from_left] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top - 1][current.from_left] != empty) || (board[current.from_top - 2][current.from_left] != empty)) {
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 3) {

		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty)) {
			current.rr = 1;
		}
		return;
	}


}
void Game::collision_L() {

	if (current.command == 3) {
		collision_L_rotation();
		return;
	}
	if (current.command == 4) {
		collision_L_rotation();
		return;
	}


	////Rotation = 0

	if (current.rotation == 0) {

		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left + 3] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top][current.from_left + 2] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}
	}

	if (current.rotation == 1) {

		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top - 2][current.from_left - 2] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top - 2][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}
	}

	if (current.rotation == 2) {

		if ((board[current.from_top][current.from_left - 2] != empty) || (board[current.from_top - 1][current.from_left] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 2] != empty) || (board[current.from_top - 1][current.from_left + 2] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}
	}

	if (current.rotation == 3) {

		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top - 2][current.from_left - 1] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 2] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top - 2][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty)) {// check south
			current.S = 1;
		}
		else {
			current.S = 0;
		}
	}







	//This checks if the piece should be set or not, so that it can become apart of the board

	if (current.S == 0) {
		current.setdelay = _LOCK;//current.basedelay;
	}

	if (current.S == 1) {
		current.setdelay -= _dt;
	}
	if ((current.setdelay <= 0) && (current.S == 1)) {
		current.set = 1;
	}

}
void Game::move_L() {

	if ((current.command == 3) || (current.command == 4)) {
		move_L_rotate();
		return;
	}

	// Rotation = 0 Movement =  down
	if ((current.command == 0) && (current.rotation == 0) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 2] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left + 2] = block;
	}
	// Rotation = 0 Movement =  right
	if ((current.command == 2) && (current.rotation == 0) && (current.E != 1)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 2] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left + 2] = block;
	}
	// Rotation = 0 Movement =  left
	if ((current.command == 1) && (current.rotation == 0) && (current.W != 1)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 2] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left + 2] = block;
	}
	// Rotation = 1 Movement =  left
	if ((current.command == 1) && (current.rotation == 1) && (current.W != 1)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left - 1] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left - 1] = block;
	}
	// Rotation = 1 Movement =  down
	if ((current.command == 0) && (current.rotation == 1) && (current.S == 0)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left - 1] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left - 1] = block;
	}
	// Rotation = 1 Movement =  right
	if ((current.command == 2) && (current.rotation == 1) && (current.E != 1)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left - 1] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left - 1] = block;
	}
	// Rotation = 2 Movement =  left
	if ((current.command == 1) && (current.rotation == 2) && (current.W != 1)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top][current.from_left - 1] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top][current.from_left - 1] = block;
	}
	// Rotation = 2 Movement =  right
	if ((current.command == 2) && (current.rotation == 2) && (current.E != 1)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top][current.from_left - 1] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top][current.from_left - 1] = block;
	}
	// Rotation = 2 Movement =  down
	if ((current.command == 0) && (current.rotation == 2) && (current.S == 0)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top][current.from_left - 1] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top][current.from_left - 1] = block;
	}
	// Rotation = 3 Movement =  down
	if ((current.command == 0) && (current.rotation == 3) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.from_top++;
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
	}
	// Rotation = 3 Movement =  right
	if ((current.command == 2) && (current.rotation == 3) && (current.E != 1)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.from_left++;
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
	}
	// Rotation = 3 Movement =  left
	if ((current.command == 1) && (current.rotation == 3) && (current.W != 1)) {
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.from_left--;
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
	}









}
void Game::move_L_rotate() {
	int i = current.rotation;

	if ((current.command == 3) && (current.rl == 1)) {
		return;
	}
	if ((current.command == 4) && (current.rr == 1)) {
		return;
	}

	if (current.command == 3) {
		current.rotation--;
		if (current.rotation == -1) {
			current.rotation = 3;
		}
	}
	if (current.command == 4) {
		current.rotation++;
		if (current.rotation == 4) {
			current.rotation = 0;
		}
	}


	if (i == 0) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 2] = empty;
	}
	if (i == 1) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left - 1] = empty;
	}
	if (i == 2) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top][current.from_left - 1] = empty;
	}
	if (i == 3) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
	}

	//Sets proper rotation so the pieces don't run away!
	if ((i == 0) && (current.rotation == 1)) {
		current.from_left++;
	}
	if ((i == 3) && (current.rotation == 0)) {
		current.from_left--;
	}
	if ((i == 0) && (current.rotation == 3)) {
		current.from_left++;
	}
	if ((i == 1) && (current.rotation == 0)) {
		current.from_left--;
	}
	//

	if (current.rotation == 3) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
	}
	if (current.rotation == 2) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top][current.from_left - 1] = block;
	}
	if (current.rotation == 1) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left - 1] = block;
	}
	if (current.rotation == 0) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top - 1][current.from_left + 2] = block;
	}



}

//J Piece  type = 5
void Game::collision_J_rotation() {
	current.rr = 0;
	current.rl = 0;

	if (current.rotation == 0) {

		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 2][current.from_left - 1] != empty) || (board[current.from_top - 2][current.from_left] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top][current.from_left - 2] != empty)) {
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 1) {

		if ((board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top][current.from_left + 1] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top][current.from_left + 1] != empty)) {
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 2) {

		if ((board[current.from_top - 1][current.from_left] != empty) || (board[current.from_top - 2][current.from_left] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top - 1][current.from_left] != empty) || (board[current.from_top - 2][current.from_left] != empty) || (board[current.from_top - 2][current.from_left + 1] != empty)) {
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 3) {

		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top][current.from_left + 1] != empty)) {
			current.rl = 1;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty)) {
			current.rr = 1;
		}
		return;
	}
}
void Game::collision_J() {

	if (current.command == 3) {
		collision_J_rotation();
		return;
	}
	if (current.command == 4) {
		collision_J_rotation();
		return;
	}

	////////////////
	////Rotation = 0
	////////////////

	if (current.rotation == 0) {
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top][current.from_left - 2] != empty)) {// check down
			current.S = 1;
		}
		else {
			current.S = 0;
		}
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 3] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}

	}

	////////////////
	////Rotation = 1
	////////////////

	if (current.rotation == 1) {
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty)) {// check down
			current.S = 1;
		}
		else {
			current.S = 0;
		}
		if ((board[current.from_top][current.from_left - 2] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top - 2][current.from_left - 1] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top - 2][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}

	}

	////////////////
	////Rotation = 2
	////////////////

	if (current.rotation == 2) {//DOWN
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty) || (board[current.from_top + 1][current.from_left - 1] != empty)) {// check down
			current.S = 1;
		}
		else {
			current.S = 0;
		}
		if ((board[current.from_top][current.from_left - 2] != empty) || (board[current.from_top - 1][current.from_left - 2] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 2] != empty) || (board[current.from_top - 1][current.from_left] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}

	}
	////////////////
	////Rotation = 3
	////////////////

	if (current.rotation == 3) {//DOWN
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty)) {// check down
			current.S = 1;
		}
		else {
			current.S = 0;
		}
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top - 2][current.from_left - 1] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top - 2][current.from_left + 2] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}

	}



}
void Game::move_J() {

	if ((current.command == 3) || (current.command == 4)) {
		move_J_rotate();
		return;
	}

	// Rotation = 0 Movement =  down
	if ((current.command == 0) && (current.rotation == 0) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 2] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left - 2] = block;

	}
	// Rotation = 0 Movement =  right
	if ((current.command == 2) && (current.rotation == 0) && (current.E == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 2] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left - 2] = block;

	}
	// Rotation = 0 Movement =  left
	if ((current.command == 1) && (current.rotation == 0) && (current.W == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 2] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left - 2] = block;

	}

	////////////////////////

	// Rotation = 1 Movement =  down
	if ((current.command == 0) && (current.rotation == 1) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.from_top++;
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
	}
	// Rotation = 1 Movement =  right
	if ((current.command == 2) && (current.rotation == 1) && (current.E == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.from_left++;
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;

	}
	// Rotation = 1 Movement =  left
	if ((current.command == 1) && (current.rotation == 1) && (current.W == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.from_left--;
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;

	}

	////////////////////////

	// Rotation = 2 Movement =  down
	if ((current.command == 0) && (current.rotation == 2) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;


	}
	// Rotation = 2 Movement =  right
	if ((current.command == 2) && (current.rotation == 2) && (current.E == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;


	}
	// Rotation = 2 Movement =  left
	if ((current.command == 1) && (current.rotation == 2) && (current.W == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;


	}


	////////////////////////

	// Rotation = 3 Movement =  down
	if ((current.command == 0) && (current.rotation == 3) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left + 1] = block;


	}
	// Rotation = 3 Movement =  left
	if ((current.command == 1) && (current.rotation == 3) && (current.W == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left + 1] = block;


	}
	// Rotation = 3 Movement =  right
	if ((current.command == 2) && (current.rotation == 3) && (current.E == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left + 1] = block;


	}

	////////////////////////Loops rotation back to 0


	//This checks if the piece should be set or not, so that it can become apart of the board

	if (current.S == 0) {
		current.setdelay = _LOCK;//current.basedelay;
	}

	if (current.S == 1) {
		current.setdelay -= _dt;
	}
	if ((current.setdelay <= 0) && (current.S == 1)) {
		current.set = 1;
	}


}
void Game::move_J_rotate() {
	int i = current.rotation;

	if ((current.command == 3) && (current.rl == 1)) {
		return;
	}
	if ((current.command == 4) && (current.rr == 1)) {
		return;
	}

	if (current.command == 3) {
		current.rotation--;
		if (current.rotation == -1) {
			current.rotation = 3;
		}
	}
	if (current.command == 4) {
		current.rotation++;
		if (current.rotation == 4) {
			current.rotation = 0;
		}
	}

	if (i == 0) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 2] = empty;
	}
	if (i == 1) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
	}
	if (i == 2) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top][current.from_left + 1] = empty;
		current.position[current.from_top][current.from_left - 1] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
	}
	if (i == 3) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;
	}

	//Sets proper rotation so the pieces don't run away!
	if ((i == 0) && (current.rotation == 1)) {
		current.from_left--;
	}
	if ((i == 3) && (current.rotation == 0)) {
		current.from_left++;
	}
	if ((i == 0) && (current.rotation == 3)) {
		current.from_left--;
	}
	if ((i == 1) && (current.rotation == 0)) {
		current.from_left++;
	}


	if (current.rotation == 3) {
		//draw new rotation

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left + 1] = block;
	}
	if (current.rotation == 2) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left + 1] = block;
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
	}
	if (current.rotation == 1) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 2][current.from_left] = block;
	}
	if (current.rotation == 0) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top - 1][current.from_left - 2] = block;
	}
}

//Z Piece type = 6
void Game::collision_Z_rotation() {
	current.rr = 0;
	current.rl = 0;

	if (current.rotation == 0) {

		if ((board[current.from_top - 1][current.from_left + 1] != empty) || (board[current.from_top - 2][current.from_left + 1] != empty)) {
			current.rl = 1;
			current.rr = 1;
		}
		return;
	}
	if (current.rotation == 1) {

		if ((board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top][current.from_left + 1] != empty)) {
			current.rl = 1;
			current.rr = 1;
		}
		return;
	}
}
void Game::collision_Z() {

	if (current.command == 3) {
		collision_Z_rotation();
		return;
	}
	if (current.command == 4) {
		collision_Z_rotation();
		return;
	}

	////////////////
	////Rotation = 0
	////////////////

	if (current.rotation == 0) {//DOWN
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top + 1][current.from_left + 1] != empty) || (board[current.from_top][current.from_left - 1] != empty)) {// check down
			current.S = 1;
		}
		else {
			current.S = 0;
		}
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 2] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 2] != empty) || (board[current.from_top - 1][current.from_left + 1] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}

	}

	////////////////
	////Rotation = 1
	////////////////

	if (current.rotation == 1) {//DOWN
		if ((board[current.from_top + 1][current.from_left] != empty) || (board[current.from_top][current.from_left + 1] != empty)) {// check down
			current.S = 1;
		}
		else {
			current.S = 0;
		}
		if ((board[current.from_top][current.from_left - 1] != empty) || (board[current.from_top - 1][current.from_left - 1] != empty) || (board[current.from_top - 2][current.from_left] != empty)) {// check left
			current.W = 1;
		}
		else {
			current.W = 0;
		}
		if ((board[current.from_top][current.from_left + 1] != empty) || (board[current.from_top - 1][current.from_left + 2] != empty) || (board[current.from_top - 2][current.from_left + 2] != empty)) {// check right
			current.E = 1;
		}
		else {
			current.E = 0;
		}

	}

	//This checks if the piece should be set or not, so that it can become apart of the board

	if (current.S == 0) {
		current.setdelay = _LOCK;//current.basedelay;
	}

	if (current.S == 1) {
		current.setdelay -= _dt;
	}
	if ((current.setdelay <= 0) && (current.S == 1)) {
		current.set = 1;
	}



}
void Game::move_Z() {

	if ((current.command == 3) || (current.command == 4)) {
		move_Z_rotate();
		return;
	}


	// Rotation = 0 Movement =  down
	if ((current.command == 0) && (current.rotation == 0) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left + 1] = block;

	}
	// Rotation = 0 Movement =  right
	if ((current.command == 2) && (current.rotation == 0) && (current.E == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left + 1] = block;

	}
	// Rotation = 0 Movement =  left
	if ((current.command == 1) && (current.rotation == 0) && (current.W == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left + 1] = block;
	}

	////////////////////////

	// Rotation = 1 Movement =  down
	if ((current.command == 0) && (current.rotation == 1) && (current.S == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;

		current.from_top++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top - 2][current.from_left + 1] = block;


	}
	// Rotation = 1 Movement =  left
	if ((current.command == 1) && (current.rotation == 1) && (current.W == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;

		current.from_left--;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top - 2][current.from_left + 1] = block;


	}
	// Rotation = 1 Movement =  right
	if ((current.command == 2) && (current.rotation == 1) && (current.E == 0)) {

		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;

		current.from_left++;

		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top - 2][current.from_left + 1] = block;

	}


	//This checks if the piece should be set or not, so that it can become apart of the board

	if (current.S == 0) {
		current.setdelay = _LOCK;//current.basedelay;
	}

	if (current.S == 1) {
		current.setdelay -= _dt;
	}
	if ((current.setdelay <= 0) && (current.S == 1)) {
		current.set = 1;
	}



}
void Game::move_Z_rotate() {
	int i = current.rotation;

	if ((current.command == 3) && (current.rl == 1)) {
		return;
	}
	if ((current.command == 4) && (current.rr == 1)) {
		return;
	}

	if (current.command == 3) {
		current.rotation--;
		if (current.rotation == -1) {
			current.rotation = 1;
		}
	}
	if (current.command == 4) {
		current.rotation++;
		if (current.rotation == 2) {
			current.rotation = 0;
		}
	}


	if (i == 0) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left - 1] = empty;
		current.position[current.from_top][current.from_left + 1] = empty;
	}
	if (i == 1) { //clear if rotation is 0
				  //erase old rotation
		current.position[current.from_top][current.from_left] = empty;//middle block
		current.position[current.from_top - 1][current.from_left] = empty;
		current.position[current.from_top - 1][current.from_left + 1] = empty;
		current.position[current.from_top - 2][current.from_left + 1] = empty;
	}
	if (current.rotation == 1) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left + 1] = block;
		current.position[current.from_top - 2][current.from_left + 1] = block;
	}
	if (current.rotation == 0) {
		//draw new rotation
		current.position[current.from_top][current.from_left] = block;//middle block
		current.position[current.from_top - 1][current.from_left] = block;
		current.position[current.from_top - 1][current.from_left - 1] = block;
		current.position[current.from_top][current.from_left + 1] = block;
	}






}

