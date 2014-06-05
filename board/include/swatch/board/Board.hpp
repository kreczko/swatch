#ifndef CACTUSCORE_SWATCH_BOARD_BOARD_H
#define CACTUSCORE_SWATCH_BOARD_BOARD_H


// SWATCH HEADERS
#include "swatch/core/Device.hpp"


// OTHER HEADERS


// C++ HEADERS
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>


// FORWARD DECLARATIONS
namespace uhal
{
class HwInterface;
}




namespace swatch
{

// BOARD CLASS
namespace board
{

//! A class derived from swatch::core::Device used as a base class for concrete type of uTCA boards
class Board : public swatch::core::Device
{
public:

	/**
	 * Constructor
	 * @param id The name of the board
	 */
	Board(const std::string& id);

	/**
	 * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
	 */
	virtual ~Board();

	/**
	 * Add a uhal::HwInterface object to a Board
	 * @param hwInterface The pointer to the uhal::HwInterface object. It will be used to perform read and write operations.
	 */
	void addUhalInterface(uhal::HwInterface* hwInterface);

	/**
	 * Board implementation of the configure transition. Final state: Configured.
	 */
	virtual void configure();

	/**
	 * Board implementation of the enable transition. Final state: Enabled.
	 */
	virtual void enable();

	/**
	 * Board implementation of the suspend transition. Final state: Suspended.
	 */
	virtual void suspend();

	/**
	 * Board implementation of the stop transition. Final state: Configured.
	 */
	virtual void stop();

	/**
	 * Board implementation of the resume transition. Final state: Enabled.
	 */
	virtual void resume();


protected:

private:

	//! vector that holds the uhal::HwInterfaces pointers wrapped up in a boost::shared_ptr to use the RAII idiom
	std::vector< boost::shared_ptr<uhal::HwInterface> > hwInterfaces_;
};

} // end ns board
} // end ns swatch
#endif
