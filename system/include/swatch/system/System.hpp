#ifndef CACTUSCORE_SWATCH_SYSTEM_SYSTEM_H
#define CACTUSCORE_SWATCH_SYSTEM_SYSTEM_H

// SWATCH HEADERS
#include "swatch/core/Device.hpp"


// OTHER HEADERS


// C++ HEADERS
#include <string>
#include <vector>


// FORWARD DECLARATIONS

namespace swatch
{


namespace board
{
class Board;
class BoardFactory;
}


namespace system
{

// CLASS SYSTEM

	//! Class extending the Device class for the case of a System (collection of boards and links)
	class System : public swatch::core::Device
	{
	public:

		/**
		 * Constructor
		 * @param id Name of this system object
		 */
		System(const std::string& id);

		/**
		 * Destructor
		 */
		~System();

		/**
		 * System implementation of the configure transition. Final state: Configured.
		 */
		void configure();

		/**
		 * System implementation of the enable transition. Final state: Enabled.
		 */
		void enable();

		/**
		 * System implementation of the suspend transition. Final state: Suspended.
		 */
		void suspend();

		/**
		 * System implementation of the stop transition. Final state: Configured.
		 */
		void stop();

		/**
		 * System implementation of the resume transition. Final state: Enabled.
		 */
		void resume();

		/**
		 * We'll see
		 */
		void test();


		/**
		 * For testing purposes
		 */
		std::string displayStatus();


	protected:
	private:

		typedef boost::shared_ptr<swatch::core::Device> shared_board;
		typedef std::vector<shared_board> Boards;
		/**
		 * Performs initialization operaton for this System object:
		 * 1. Reads from database ...
		 * 2. Creates Board objects...
		 * 3. Creates Link objects...
		 */
		void init();

		/**
		 * Checks whether the system is ready. The system is considered to be ready for operations after the init() method has
		 * successfully finished, leaving the System in a first known state
		 * @return boolean value telling whether the System has acquired a first known state ready for operations
		 */
		bool isSystemReady();

		/**
		 * System configuration is subdivided in these different tasks
		 */
		void configureBoards();
		void configureMasks();
		void configureSW();


		//! Vector containing all the Board objects part of this System object
		Boards boards_;

		//! Boolean telling whether this System object is in a known state ready for operations
		bool ready_;

	};
}
}
#endif
