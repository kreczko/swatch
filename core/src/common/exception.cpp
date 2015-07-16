#include "swatch/core/exception.hpp"


#include <time.h>
#include "boost/lexical_cast.hpp"


using namespace std;


swatch::core::exception::exception() throw() :
  std::exception(),
  exThreadId_(boost::this_thread::get_id())
{
  gettimeofday(&exTime_, NULL);
}



swatch::core::exception::exception(const string& what) :
  std::exception(),
  what_(what)
{
  gettimeofday(&exTime_, NULL);
}



swatch::core::exception::exception(const swatch::core::exception& e) throw() :
  std::exception(),
  exThreadId_(e.exThreadId_),
  exTime_(e.exTime_),
  what_(e.what_)
{
}



swatch::core::exception::~exception() throw()
{
}



swatch::core::exception&
swatch::core::exception::operator=(const swatch::core::exception& e) throw()
{
  exThreadId_ = e.exThreadId_;
  exTime_ = e.exTime_;
  what_ = e.what_;

  return *this;
}



const char*
swatch::core::exception::what() const throw()
{
  char timeBuf[64];
  strftime (timeBuf, sizeof timeBuf, "%Y-%m-%d %H:%M:%S", localtime (&exTime_.tv_sec));

  ostringstream msg;
  msg << "TIME: " << timeBuf << " DESCRIPTION: " << what_;
  return msg.str().c_str();
}
