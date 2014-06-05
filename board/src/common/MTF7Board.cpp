// SWATCH HEADERS
#include "swatch/board/MTF7Board.hpp"


// OTHER HEADERS


// C++ HEADERS
#include <iostream>


using namespace swatch::board;
using namespace std;



MTF7Board::MTF7Board(const string& id)
:
		Board(id)
{
	cout << "swatch::board::MTF7Board::MTF7Board --> Board CTOR called. Id: " << id_ << endl;
}



MTF7Board::~MTF7Board()
{
	cout << "swatch::board::MTF7Board::~MTF7Board --> Board DTOR called. Id: " << id_ << endl;
}
