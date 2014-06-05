#ifndef CACTUSCORE_SWATCH_BOARD_CTP7BOARD_H
#define CACTUSCORE_SWATCH_BOARD_CTP7BOARD_H


// SWATCH HEADERS
#include "swatch/board/Board.hpp"


// OTHER HEADERS


// C++ HEADERS


// FORWARD DECLARATIONS





namespace swatch
{

namespace board
{

// CTP7BOARD CLASS

//! Concrete implementation of the class Board for the particular case of the CTP7 board
class CTP7Board : public Board
{

public:

	/**
	 * Constructor
	 * @param id The name for this board object
	 */
	CTP7Board(const std::string& id);

	/**
	 * Destructor
	 */
	~CTP7Board();

protected:
private:


};

} // end ns board
} // end ns swatch
#endif
