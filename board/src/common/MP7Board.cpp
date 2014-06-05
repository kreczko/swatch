// SWATCH HEADERS
#include "swatch/board/MP7Board.hpp"


// OTHER HEADERS


// C++ HEADERS
#include <iostream>


using namespace swatch::board;
using namespace std;



MP7Board::MP7Board(const string& id)
:
		Board(id)
{
	cout << "swatch::board::MP7Board::MP7Board --> Board CTOR called. Id: " << id_ << endl;
}



MP7Board::~MP7Board()
{
	cout << "swatch::board::MP7Board::~MP7Board --> Board DTOR called. Id: " << id_ << endl;
}



void
MP7Board::configure()
{
	cout << "Executing configure in MP7Board class for board " << id_ << endl;
	swatch::board::Board::Board::configure();
}
