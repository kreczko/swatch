#ifndef CACTUSCORE_SWATCH_BOARD_MTF7BOARD_H
#define CACTUSCORE_SWATCH_BOARD_MTF7BOARD_H

// SWATCH HEADERS
#include "swatch/board/Board.hpp"


// OTHER HEADERS


// C++ HEADERS


// FORWARD DECLARATIONS

namespace swatch
{

namespace board
{

// CLASS MTF7BOARD

//! Concrete implementation of the class Board for the particular case of the MTF7 board
class MTF7Board : public Board
{

public:

	/**
	 * Constructor
	 * @param id The name for this board object
	 */
	MTF7Board(const std::string& id);

	/**
	 * Destructor
	 */
	~MTF7Board();

protected:
private:


};

} // end ns board
} // end ns swatch
#endif
