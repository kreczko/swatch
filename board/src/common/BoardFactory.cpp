// SWATCH HEADERS
#include "swatch/board/BoardFactory.hpp"

#include "swatch/board/Board.hpp"
#include "swatch/board/MP7Board.hpp"
#include "swatch/board/MTF7Board.hpp"
#include "swatch/board/CTP7Board.hpp"


// OTHER HEADERS


// C++ HEADERS
#include <iostream>


using namespace swatch::board;
using namespace std;



// STATIC MEMBERS
BoardFactory*
	BoardFactory::bf_ = 0;

map<string, BoardFactory::CreatorInterface*>
	BoardFactory::mapBoardTypeToBoardCreator_;




// PUBLIC API
BoardFactory&
BoardFactory::getInstance()
{
	// Do we need multi-threaded protection here?
	if (!bf_)
	{
		bf_ = new BoardFactory();

		bf_->add<MP7Board>  ("MP7");
		bf_->add<CTP7Board> ("CTP7");
		bf_->add<MTF7Board> ("MTF7");

		cout << "BoardFactory::getInstance --> Board factory instance created" << endl;
	}

	return *bf_;
}



Board*
BoardFactory::getBoard(const string& boardName)
{

	string boardType = getBoardTypeFromName(boardName);

	map<string, CreatorInterface*>::iterator it = mapBoardTypeToBoardCreator_.find(boardType);

	if (it == mapBoardTypeToBoardCreator_.end())
	{
		cout << "BoardFactory::getBoard --> EXCEPTION: Tried to create a board from a non-registered type!" << endl;
	}

	Board* b = it->second->create(boardName);
	return b;
}



vector<string>
BoardFactory::getRegisteredBoardTypes()
{
	vector<string> result;

	for (map<string, CreatorInterface*>::const_iterator cIt = mapBoardTypeToBoardCreator_.begin();
			cIt != mapBoardTypeToBoardCreator_.end();
			++cIt)
	{
		result.push_back(cIt->first);
	}

	return result;
}



// PRIVATE API
BoardFactory::BoardFactory()
{
}



BoardFactory::~BoardFactory()
{
	for (map<string, CreatorInterface*>::iterator it = mapBoardTypeToBoardCreator_.begin();
			it != mapBoardTypeToBoardCreator_.end();
			++it)
	{
		delete it->second;
	}
}


string
BoardFactory::getBoardTypeFromName(const string& boardName)
{
	if (boardName.find("MP7") != string::npos)
	{
		return "MP7";
	}

	if (boardName.find("CTP7") != string::npos)
	{
		return "CTP7";
	}

	if (boardName.find("MTF7") != string::npos)
	{
		return "MTF7";
	}

	return "";
}
