
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


std::ostream& operator<<(std::ostream& aStream, swatch::core::ActionStatus::State aState)
{
  switch (aState) {
    case ActionStatus::kInitial   : aStream << "Initial"; break;
    case ActionStatus::kScheduled : aStream << "Scheduled"; break;
    case ActionStatus::kRunning   : aStream << "Running"; break;
    case ActionStatus::kWarning   : aStream << "Warning"; break;
    case ActionStatus::kError     : aStream << "Error"; break;
    case ActionStatus::kDone      : aStream << "Done"; break;
    default : aStream << "Unknown value of swatch::core::ActionStatus::Status enum"; 
  }
  return aStream;
}


ActionStatus::~ActionStatus() 
{
}


const std::string& ActionStatus::getActionPath() const
{
  return mPath;
}


ActionStatus::State ActionStatus::getState() const
{
  return mState;
}


float ActionStatus::getRunningTime() const
{
  return mRunningTime;
}
  
ActionStatus::ActionStatus(const std::string& aPath, State aState, float aRunningTime) : 
  mPath(aPath),
  mState(aState),
  mRunningTime(aRunningTime)
{}


} /* namespace core */
} /* namespace swatch */
