
#include "swatch/core/Functionoid.hpp"


namespace swatch {
namespace core {

Functionoid::Functionoid( const std::string& aId, Object& aResource ) :
  Object( aId ),
	mResource( aResource)
{
}


Functionoid::~Functionoid() {
}


const Object& Functionoid::getResource() const {
  return mResource;
}

Object& Functionoid::getResource() {
  return mResource;
}

std::ostream& operator<<(std::ostream& out, swatch::core::ActionStatus::State s) {
  switch (s) {
    case ActionStatus::kInitial   : out << "Initial"; break;
    case ActionStatus::kScheduled : out << "Scheduled"; break;
    case ActionStatus::kRunning   : out << "Running"; break;
    case ActionStatus::kWarning   : out << "Warning"; break;
    case ActionStatus::kError     : out << "Error"; break;
    case ActionStatus::kDone      : out << "Done"; break;
    default : out << "Unknown value of swatch::core::ActionStatus::Status enum"; 
  }
  return out;
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
