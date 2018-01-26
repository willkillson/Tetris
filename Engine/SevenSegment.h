#pragma once
#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H
#include "Colors.h"
#include "Graphics.h"
#include <string>
#include <sstream>
#include <string>



class SevenSegment
{

public:

	SevenSegment(int x, int y, int scale, Color c, Graphics &gfx)
	{
		this->_xpos = x;
		this->_ypos = y;
		this->_scale = scale;
		this->_c = c;
		this->_gfx = &gfx;
	}
	void print(int x);

private:
	void _DrawNumber(int x);//Prints an int from 0 - 9 
	void _CalculatePlaces(); //calculates how many places an int has
	void _DrawSegment(char segment);// prints segments A,B,C,D,E,F,G of a 7 segment display
	
	int _number;
	int _xpos;
	int _ypos;
	int _scale;
	int _places;
	Graphics *_gfx;
	Color _c;

};

#endif