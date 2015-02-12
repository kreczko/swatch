#include "swatch/system/test/DummyAMC13Service.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/system/ServiceFactory.hpp"
#include "swatch/system/AMC13ServiceStub.hpp"
#include <swatch/core/xoperators.hpp>

#include <xdata/Integer.h>

// XDAQ Headers
#include "xdata/Serializable.h"

// Namespace resolution
using namespace std;

namespace swlog = swatch::logger;
namespace swsys = swatch::system;


namespace swatch {
namespace system {
namespace test {

void oStreamTest() {
    xdata::Integer i(0);
    std::ostringstream oss;
    oss << i;
    
    const xdata::Serializable& s = i;
    oss << s;

}

}
}
}