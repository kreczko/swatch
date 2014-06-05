#ifndef CACTUSCORE_SWATCH_CORE_LINK_H
#define CACTUSCORE_SWATCH_CORE_LINK_H


// SWATCH HEADERS
// OTHER HEADERS
#include <boost/shared_ptr.hpp>


// C++ HEADERS
#include <string>


// FORWARD DECLARATIONS
namespace swatch
{

namespace core
{

class Device;
class Port;


// CLASS LINK

//!
class Link
{

public:

	/**
	 * Constructor
	 * @param id Name of the Link object
	 * @param src Source Port object of this link, origin of the data flow
	 * @param dst Destination Port object of this link, destination of the data flow
	 */
	Link(const std::string& id, const boost::shared_ptr<Port>& src, const boost::shared_ptr<Port>& dst);

	/**
	 * Destructor
	 */
	virtual ~Link();

	/**
	 * Gets the name of this Link's object
	 * @return The name of this Link's object
	 */
	std::string getId();

	/**
	 * Gets a the Port that is the source of this link's data flow
	 * @return A shared pointer wrapping the Port object that is the source of the data flow
	 */
	boost::shared_ptr<Port> getSrcPort();


	/**
	 * Gets a the Port that is the destination of this link's data flow
	 * @return A shared pointer wrapping the Port object that is the destination of the data flow
	 */
	boost::shared_ptr<Port> getDstPort();

	/**
	 * Sets this link to the "Enabled" state
	 */
	void enable();

	/**
	 * Checks whether this Link is enabled
	 * @return boolean value telling whether this Link object is enabled
	 */
	bool isEnabled();

protected:
private:

	//! type definition for a shared_ptr wrapping up a Port object
	typedef boost::shared_ptr<Port> shared_port;

	//! The name of this Link object
	std::string id_;

	//! Tells whether the Link is enabled or not
	bool enabled_;

	//! Shared pointers to the source and destination Device objects of this Link object
	shared_port srcPort_, dstPort_;
};

} // end ns core
} // end ns swatch
#endif
