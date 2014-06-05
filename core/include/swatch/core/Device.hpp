#ifndef CACTUSCORE_SWATCH_CORE_DEVICE_H
#define CACTUSCORE_SWATCH_CORE_DEVICE_H

// SWATCH HEADERS


// OTHER HEADERS


// C++ HEADERS
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>


// FORWARD DECLARATIONS


namespace swatch
{

namespace core
{

class Port;

// CLASS DEVICE

	//! Base class for the Board and System classes. It also defines the methods
	//! that provide with the interface to run control
	class Device
	{

	protected:
		//! Enum type containing all possible states from the TS/XDAQ FSM
		typedef enum AllowedStates {Halted, Configured, Enabled, Suspended} FSMStates;

		//! Type definitions for a boost shared pointer to Port (shared_port), and a vector of the previous type (Ports)
		typedef boost::shared_ptr<Port> shared_port;
		typedef std::vector<shared_port> Ports;


	public:

		/**
		 * Constructor
		 * @param id The name for the Device object
		 */
		Device(const std::string& id);

		/**
		 * Destructor
		 */
		virtual ~Device();

		/**
		 * Gets the ID of the Device
		 * @return string identifying the device uniquely
		 */
		std::string getId();


		void addTx(const shared_port& tx);
		void addRx(const shared_port& rx);

		/**
		 * Gets the state in which the Device object is at any time
		 * @return The state in which the Device object is
		 */
		FSMStates getState();

		/**
		 * Configure transition. Final state: Configured. Implementation delegated to subclasses
		 * @
		 */
		virtual void configure() = 0;

		/**
		 * Enable transition. Final state: Enabled. Implementation delegated to subclasses
		 */
		virtual void enable() = 0;

		/**
		 * Suspend transition. Final state: Suspended. Implementation delegated to subclasses
		 */
		virtual void suspend() = 0;

		/**
		 * Stop transition. Final state: Configured. Implementation delegated to subclasses
		 */
		virtual void stop() = 0;

		/**
		 * Resume transition. Final state: Enabled. Implementation delegated to subclasses
		 */
		virtual void resume() = 0;

		/**
		 * Don't know its purpose yet, or how to handle the testing procedures
		 */
		virtual void test();


	protected:

		/**
		 * Sets a state to the Device object after each run control operation
		 * @param state The state to be set to the Device object
		 */
		void setState(const FSMStates& state);

		//! Name of the Device object
		std::string id_;

		//! Current state of the Device object
		FSMStates state_;

		//! Vector containing the transmission Port objects this Device object contains
		Ports txPorts_;

		//! Vector containing the reception Port objects this Device object contains
		Ports rxPorts_;




	private:


	};

} // end ns core
} // end ns swatch

#endif
