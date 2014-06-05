// SWATCH HEADERS
#include "swatch/system/System.hpp"

// OTHER HEADERS


// C++ HEADERS
#include <boost/thread.hpp>
#include <iostream>
#include <queue>
#include <cstdlib>



using namespace std;


namespace swatch
{


namespace test
{

class SwatchTest
{

private:

	const size_t iterations_;
	const size_t channelSize_;
	queue<unsigned int> channel_;


public:

	SwatchTest():
		iterations_(20),
		channelSize_(10)
	{

	}

	~SwatchTest()
	{

	}



	int run()
	{
		cout << "swatch::test::SwatchTest::run --> Running tests..." << endl;
		// initialize systems: ID, load boards etc
		swatch::system::System gt("GT");
		cout << gt.displayStatus();

		cout << "swatch::test::SwatchTest::run --> systems ready; will go through the FSM... " << endl;
		cout << "************************************************************************" << endl;


		// configure systems
		gt.configure();
		cout << gt.displayStatus();

		/*
		// play with the FSM
		gt.enable();
		gt.stop();
		gt.enable();
		gt.suspend();
		gt.stop();
		gt.enable();
		gt.suspend();
		gt.enable();
		gt.stop();

		// finish with enable state
		gt.enable();
		 */
		cout << "swatch::test::SwatchTest::run --> systems enabled; will start the tests now... " << endl;
		cout << "************************************************************************" << endl;

		// start producing and consuming data
//		gt.test();


		return 0;
	}



};

} // end ns test
} // end ns swatch


int main(int argc, char* argv[])
{
	swatch::test::SwatchTest st;
	return st.run();
}
