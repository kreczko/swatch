// SWATCH HEADERS
#include "swatch/system/System.hpp"

#include "swatch/core/Port.hpp"
#include "swatch/core/Link.hpp"
#include "swatch/core/exception.hpp"
#include "swatch/board/Board.hpp"
#include "swatch/board/BoardFactory.hpp"


// OTHER HEADERS
#include "uhal/ConnectionManager.hpp"


// C++ HEADERS
#include <iostream>


using namespace swatch::system;
using namespace std;



System::System(const string& id)
:
		Device(id),
		ready_(false)
{
	cout << "System::System --> System CTOR called. Id: " << id_ << endl;
	init();
}




System::~System()
{
	cout << "System::~System --> System DTOR called. Id: " << id_ << endl;
}



// PRIVATE API
void
System::init()
{
	// This method should extract from a database or file the necessary information to build the System object, i.e.,
	// get the boards that are part of it and the links between them and/or other Device objects (other subsystems)
	// For the time being, we are using a uHAL dummy connection file for test only purposes


	// BOARD INITIALIZATION: uHAL
	/*
	uhal::ConnectionManager cm("file://test/xml/dummy_connections.xml");
	vector<string> devices = cm.getDevices();

	vector<string> regBoardTypes =
					swatch::board::BoardFactory::getInstance().getRegisteredBoardTypes();

	for (vector<string>::const_iterator cIt = devices.begin();
			cIt != devices.end();
			++cIt)
	{

		string endPointName = *cIt;
		cout << "System::init --> Found uTCA IP end point " << *cIt << endl;

		for (vector<string>::const_iterator bTypeCIt = regBoardTypes.begin();
				bTypeCIt != regBoardTypes.end();
				++bTypeCIt)
		{

			string boardTypeName = *bTypeCIt;

			if ( endPointName.find(boardTypeName) == string::npos )
			{
				continue;
			}

			// How to cope with the fact that one board can have more than one IP end point? (AMC13)

			swatch::board::Board* b =
					swatch::board::BoardFactory::getInstance().getBoard(boardTypeName);

			uhal::HwInterface* hi = new uhal::HwInterface(cm.getDevice(endPointName));
			b->addUhalInterface(hi);


			boards_.push_back(shared_board(static_cast<swatch::core::Device*>(b)));
			cout << "System::init --> Board of type " << boardTypeName << " added to subsystem" << endl;
		}
	}
	 */

	// BOARD INITIALIZATION: Testing
	swatch::board::Board* b0 = swatch::board::BoardFactory::getInstance().getBoard("MP7-1.udp2");
	boards_.push_back(shared_board(static_cast<swatch::core::Device*>(b0)));

	swatch::board::Board* b1 = swatch::board::BoardFactory::getInstance().getBoard("MP7-2.udp2");
	boards_.push_back(shared_board(static_cast<swatch::core::Device*>(b1)));

	swatch::board::Board* b2 = swatch::board::BoardFactory::getInstance().getBoard("CTP7-1.udp2bigendian");
	boards_.push_back(shared_board(static_cast<swatch::core::Device*>(b2)));


	// PORT INITIALIZATION:  Testing
	boost::shared_ptr<swatch::core::Port> p0B0(new swatch::core::Port("RX0_B0", boards_[0]));
	boost::shared_ptr<swatch::core::Port> p1B0(new swatch::core::Port("TX0_B0", boards_[0]));

	boost::shared_ptr<swatch::core::Port> p0B1(new swatch::core::Port("RX0_B1", boards_[1]));
	boost::shared_ptr<swatch::core::Port> p1B1(new swatch::core::Port("TX0_B1", boards_[1]));

	boost::shared_ptr<swatch::core::Port> p0B2(new swatch::core::Port("RX0_B2", boards_[2]));
	boost::shared_ptr<swatch::core::Port> p1B2(new swatch::core::Port("TX0_B2", boards_[2]));


	boards_[0]->addRx(p0B0); boards_[0]->addTx(p1B0);
	boards_[1]->addRx(p0B1); boards_[1]->addTx(p1B1);
	boards_[2]->addRx(p0B2); boards_[2]->addTx(p1B2);


	ready_ = true;
	setState(Halted);
	cout << "System::init --> Subsystem " << id_ << " is ready" << endl;

}



bool
System::isSystemReady()
{
	return ready_;
}



// PUBLIC API
void
System::configure()
{
	if (!isSystemReady())
	{
		// just to test the exception class
		cout << "System::configure --> EXCEPTION: in configuration; system not ready" << endl;
		throw swatch::core::ConfigureError("Error during configuration");
	}

	cout << "System::configure --> Doing system " << id_ << " configure" << endl;


	// Extract record pointed by each FK and pass it to the different methods
	configureBoards();

	setState(Configured);
}



void
System::enable()
{
	if (!isSystemReady())
	{
		cout << "System::enable --> EXCEPTION; in enable transition: system not ready" << endl;
		return;
	}

	cout << "System::enable --> Doing system " << id_ << " enable" << endl;
	setState(Enabled);
}


void
System::suspend()
{
	cout << "System::suspend --> Doing system " << id_ << " suspend" << endl;
	setState(Suspended);
}



void
System::stop()
{
	cout << "System::stop --> Doing system " << id_ << " stop" << endl;
	setState(Configured);
}



void
System::resume()
{
	if (!isSystemReady())
	{
		cout << "System::resume --> EXCEPTION in configuration: system not ready" << endl;
		return;
	}

	cout << "System::resume --> Doing system " << id_ << " resume" << endl;
	setState(Enabled);
}



void
System::test()
{
	cout << "System::test --> Doing system " << id_ << " test" << endl;
	setState(Enabled);
}



// PRIVATE API

void
System::configureBoards()
{

	System::Boards::const_iterator bCIt;
	for (bCIt = boards_.begin(); bCIt != boards_.end(); ++bCIt)
	{
		(*bCIt)->configure();
	}


	// Something to do with the ports here?
}



void
System::configureMasks()
{

}



void
System::configureSW()
{

}



string
System::displayStatus()
{
	ostringstream status;

	status << "---------- Status for subsystem: " << id_ << "----------\n" << endl;
	status << "State after last transition: " << getState() << "\n" << endl;

	status << "---------- Boards: ----------" << "\n" << endl;

	System::Boards::const_iterator bCIt;
	for (bCIt = boards_.begin(); bCIt != boards_.end(); ++bCIt)
	{
		status << "Board: " << (*bCIt)->getId() << "; Status: " << (*bCIt)->getState() << "\n" << endl;
	}


	status << "---------- Ports:  ----------" << "\n" << endl;



	return status.str();
}
