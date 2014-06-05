// SWATCH HEADERS
#include "swatch/board/CTP7Board.hpp"


// OTHER HEADERS


// C++ HEADERS
#include <iostream>


using namespace swatch::board;
using namespace std;



CTP7Board::CTP7Board(const string& id)
:
		Board(id)
{
	cout << "swatch::board::CTP7Board::CTP7Board --> Board CTOR called. Id: " << id_ << endl;
}



CTP7Board::~CTP7Board()
{
	cout << "swatch::board::CTP7Board::~CTP7Board --> Board DTOR called. Id: " << id_ << endl;
}
