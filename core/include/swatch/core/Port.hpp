#ifndef CACTUSCORE_SWATCH_CORE_PORT_H
#define CACTUSCORE_SWATCH_CORE_PORT_H


// SWATCH HEADERS

// OTHER HEADERS
// C++ HEADERS
#include <string>
#include <boost/shared_ptr.hpp>

namespace swatch
{

namespace core
{

class Device;
class Link;

class Port
{

protected:
	//! Enum type defining data direction for a Port object
	typedef enum Direction {IN, OUT} PortDirection;

	//! Enum types for a boost shared pointer wrapping Device and Link objets, respectively
	typedef boost::shared_ptr<Device> shared_device;
	typedef boost::shared_ptr<Link>   shared_link;


public:

	/**
	 *
	 * @param id Port unique identifier
	 * @param device Device object the Port object belongs to
	 * @param enabled Whether the Port is enabled (ready for transmission or reception) or not
	 */
	Port(const std::string& id, const shared_device& device, bool enabled=false);

	/**
	 * Destructor
	 *
	 */
	~Port();

	/**
	 * Sets this port object direction to "OUT", meaning data will flow out of the port
	 */
	void setTxDirection();

	/**
	 * Sets this port object direction to "IN", meaning data will flow IN the port
	 */
	void setRxDirection();

	/**
	 * Attaches a Link object to this Port object
	 * @param link The link this Port object will be part of
	 */
	void setLink(const shared_link& link);

	/**
	 * Gets the data direction for this Port object
	 * @return IN (Port receives data), OUT (Port transmits data)
	 */
	PortDirection getDirection();

protected:

	//! Port unique identifier
	std::string id_;

	//! boost::shared_ptr<Device> this Port object belongs to
	shared_device device_;

	//! Tells whehter the port is enabled (ready for tx or rx) or not
	bool enabled_;

	//! Port data direction
	Direction direction_;

	//! boost::shared_ptr<Link> this Port makes part of
	shared_link link_;

private:
};
} // end ns core
} // end ns swatch


#endif
