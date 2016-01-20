#include "swatch/core/exception.hpp"


#include <time.h>
#include "boost/lexical_cast.hpp"


using namespace std;


swatch::core::exception::exception() throw() :
  std::exception(),
  mThreadId(boost::this_thread::get_id())
{
}



swatch::core::exception::exception(const string& aWhat) :
  std::exception(),
  mWhat(aWhat)
{
}



swatch::core::exception::exception(const swatch::core::exception& aOther) throw() :
  std::exception(),
  mThreadId(aOther.mThreadId),
  mWhat(aOther.mWhat)
{
}



swatch::core::exception::~exception() throw()
{
}



swatch::core::exception&
swatch::core::exception::operator=(const swatch::core::exception& aOther) throw()
{
  mThreadId = aOther.mThreadId;
  mWhat = aOther.mWhat;

  return *this;
}



const char*
swatch::core::exception::what() const throw()
{ 
  return mWhat.c_str();
}
