#include "swatch/core/exception.hpp"


#include <time.h>
#include "boost/lexical_cast.hpp"


using namespace std;


swatch::core::exception::exception() throw() :
  std::exception(),
  mThreadId(boost::this_thread::get_id())
{
  gettimeofday(&mTime, NULL);
}



swatch::core::exception::exception(const string& aWhat) :
  std::exception(),
  mWhat(aWhat)
{
  gettimeofday(&mTime, NULL);
}



swatch::core::exception::exception(const swatch::core::exception& aOther) throw() :
  std::exception(),
  mThreadId(aOther.mThreadId),
  mTime(aOther.mTime),
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
  mTime = aOther.mTime;
  mWhat = aOther.mWhat;

  return *this;
}



const char*
swatch::core::exception::what() const throw()
{
  char timeBuf[64];
  strftime (timeBuf, sizeof timeBuf, "%Y-%m-%d %H:%M:%S", localtime (&mTime.tv_sec));

  ostringstream msg;
  msg << "TIME: " << timeBuf << " DESCRIPTION: " << mWhat;
  return msg.str().c_str();
}
