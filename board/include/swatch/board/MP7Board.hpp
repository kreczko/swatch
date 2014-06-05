#ifndef CACTUSCORE_SWATCH_BOARD_MP7BOARD_H
#define CACTUSCORE_SWATCH_BOARD_MP7BOARD_H


// SWATCH HEADERS
#include "swatch/board/Board.hpp"


// OTHER HEADERS


// C++ HEADERS


// FORWARD DECLARATIONS



namespace swatch
{

namespace board
{

// MP7BOARD CLASS

//! Concrete implementation of the class Board for the particular case of the CTP7 board
class MP7Board : public Board
{

public:

	/**
	 * Constructor
	 * @param id The name for this board object
	 */
	MP7Board(const std::string& id);

	/**
	 * Destructor
	 */
	~MP7Board();

	void configure();

protected:
private:


};

} // end ns board
} // end ns swatch
#endif
