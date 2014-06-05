// SWATCH HEADERS
#include "swatch/board/Board.hpp"


// OTHER HEADERS
#include "uhal/HwInterface.hpp"


// C++ HEADERS
#include <iostream>

using namespace swatch::board;
using namespace std;


Board::Board(const std::string& id):
Device(id)
{
	cout << "Board::Board --> Board CTOR called. Id: " << id_ << endl;
	setState(Halted);
}



Board::~Board()
{
	cout << "Board::~Board --> Board DTOR called. Id: " << id_ << endl;

}



void
Board::addUhalInterface(uhal::HwInterface* hi)
{
	hwInterfaces_.push_back(boost::shared_ptr<uhal::HwInterface>(hi));
	cout << "Board::addUhalInterface --> uhal::HwInterface " << hi->id() << " added to board " << id_ << endl;
}



void
Board::configure()
{
	cout << " Executing configure() in Board class for board " << id_ << endl;
	setState(Configured);
}



void
Board::enable()
{
}



void
Board::suspend()
{
}



void
Board::stop()
{
}



void
Board::resume()
{
}
