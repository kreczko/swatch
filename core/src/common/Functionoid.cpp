
#include "swatch/core/Functionoid.hpp"


namespace swatch {
namespace core {

Functionoid::Functionoid( const std::string& aId ) :
  Object( aId )
{
}


Functionoid::~Functionoid()
{
}


std::ostream& operator<<(std::ostream& aStream, swatch::core::ActionSnapshot::State aState)
{
  switch (aState) {
    case ActionSnapshot::kInitial   : aStream << "Initial"; break;
    case ActionSnapshot::kScheduled : aStream << "Scheduled"; break;
    case ActionSnapshot::kRunning   : aStream << "Running"; break;
    case ActionSnapshot::kWarning   : aStream << "Warning"; break;
    case ActionSnapshot::kError     : aStream << "Error"; break;
    case ActionSnapshot::kDone      : aStream << "Done"; break;
    default : aStream << "Unknown value of swatch::core::ActionStatus::Status enum"; 
  }
  return aStream;
}


ActionSnapshot::~ActionSnapshot() 
{
}


const std::string& ActionSnapshot::getActionPath() const
{
  return mPath;
}


ActionSnapshot::State ActionSnapshot::getState() const
{
  return mState;
}


float ActionSnapshot::getRunningTime() const
{
  return mRunningTime;
}
  
ActionSnapshot::ActionSnapshot(const std::string& aPath, State aState, float aRunningTime) : 
  mPath(aPath),
  mState(aState),
  mRunningTime(aRunningTime)
{}


} /* namespace core */
} /* namespace swatch */
