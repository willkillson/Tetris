#include "SevenSegment.h"

void SevenSegment::print(int x)
{
	this->_number = x;
	this->_CalculatePlaces();
	std::stringstream ss;
	ss << _number;

	std::string number(ss.str());

	int i = 0;
	int places = _places;
	int space = 0;
	int temp = 0;
	while (places >= 0)
	{
		/*std::cout << number[i] << std::endl;*/
		std::stringstream ss;
		ss << number[i];
		ss >> temp;
		this->_DrawNumber(temp);
		this->_xpos += 30;
		i++;
		places--;

	}




}

void SevenSegment::_DrawNumber(int x)
{
	switch (x)
	{
	case(0):
		_DrawSegment('A');
		_DrawSegment('B');
		_DrawSegment('C');
		_DrawSegment('D');
		_DrawSegment('E');
		_DrawSegment('F');
		break;
	case(1):
		_DrawSegment('B');
		_DrawSegment('C');
		break;
	case(2):
		_DrawSegment('A');
		_DrawSegment('B');
		_DrawSegment('G');
		_DrawSegment('E');
		_DrawSegment('D');
		break;
	case(3):
		_DrawSegment('A');
		_DrawSegment('B');
		_DrawSegment('G');
		_DrawSegment('C');
		_DrawSegment('D');
		break;
	case(4):
		_DrawSegment('F');
		_DrawSegment('G');
		_DrawSegment('B');
		_DrawSegment('C');
		break;
	case(5):
		_DrawSegment('A');
		_DrawSegment('F');
		_DrawSegment('G');
		_DrawSegment('C');
		_DrawSegment('D');
		break;
	case(6):
		_DrawSegment('F');
		_DrawSegment('G');
		_DrawSegment('E');
		_DrawSegment('D');
		_DrawSegment('C');
		break;
	case(7):
		_DrawSegment('A');
		_DrawSegment('B');
		_DrawSegment('C');
		break;
	case(8):
		_DrawSegment('A');
		_DrawSegment('B');
		_DrawSegment('C');
		_DrawSegment('D');
		_DrawSegment('E');
		_DrawSegment('F');
		_DrawSegment('G');
		break;
	case(9):
		_DrawSegment('A');
		_DrawSegment('B');
		_DrawSegment('C');
		_DrawSegment('F');
		_DrawSegment('G');
		break;

	}
}
void SevenSegment::_CalculatePlaces()
{
	int places = 0;

	int number = _number;
	while (number >= 10)
	{
		places++;
		number = number / 10;
	}
	this->_places = places;
}
void SevenSegment::_DrawSegment(char segment)
{
	int adjustX = this->_xpos;
	int adjustY = this->_ypos;
		switch (segment) {
		case('A'):
			adjustX += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			adjustX += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			break;
		case('B'):
			adjustX = this->_xpos;
			adjustY = this->_ypos;
			adjustX += 5 * 3;
			adjustY += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			adjustY += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);

			break;
		case('C'):
			adjustX = this->_xpos;
			adjustY = this->_ypos;		
			adjustX += 5 * 3;
			adjustY += 5 * 4;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			adjustY += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);

			break;
		case('D'):
			adjustX = this->_xpos;
			adjustY = this->_ypos;
			adjustX +=  5;
			adjustY += 5 * 6;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			adjustX += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			break;
		case('E'):
			adjustX = this->_xpos;
			adjustY = this->_ypos;
			adjustY += 5 * 4;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			adjustY += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			break;
		case('F'):
			adjustX = this->_xpos;
			adjustY = this->_ypos;
			adjustY += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			adjustY += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			break;
		case('G'):
			adjustX = this->_xpos;
			adjustY = this->_ypos;
			adjustX +=  5;
			adjustY += 5 * 3;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			adjustX += 5;
			_gfx->DrawRect(adjustX, adjustY, adjustX + 5, adjustY + 5, _c);
			break;
		}



}
