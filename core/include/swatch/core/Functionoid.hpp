/**
 * @file    Functionoid.hpp
 * @author  arose
 * @brief   
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_FUNCTIONOID_HPP__
#define __SWATCH_CORE_FUNCTIONOID_HPP__


// XDAQ Headers
#include <xdata/Serializable.h>

// Swatch Headers
#include "swatch/core/exception.hpp"
#include "swatch/core/Object.hpp"


namespace swatch {
namespace core {

   
//! Abstract base class for snapshots of the progress/status of an action (e.g. Command, CommandSequence, Transition)
class ActionSnapshot {
public:
  //! Represents state of action's execution
  enum State {
    kInitial,
    kScheduled,
    kRunning,
    kError,
    kWarning,
    kDone
  };
  
  virtual ~ActionSnapshot();
  
  //! Returns the path of the action (command, sequence, transition, ...)
  const std::string& getActionPath() const;
  
  //! Returns state of execution (scheduled, running, done/warning/error)
  State getState() const;
  
  //! Returns action's running time in seconds
  float getRunningTime() const;
  
  //! Returns fractional progress of command; range [0,1]
  virtual float getProgress() const = 0;
  
protected:
  ActionSnapshot(const std::string& aPath, State aState, float aRunningTime);
  
private:
  std::string mPath;
  State mState;
  float mRunningTime;
};


std::ostream& operator<<(std::ostream& out, swatch::core::ActionSnapshot::State s);


class Functionoid : public Object {
public:
  /// Constructor
  explicit Functionoid( const std::string& aId );

  /// Destructor
  virtual ~Functionoid();
  
  typedef ActionSnapshot::State State;
//
};


DEFINE_SWATCH_EXCEPTION(InvalidResource);

} /* namespace core */
} /* namespace swatch */


#endif /* __SWATCH_CORE_FUNCTIONOID_HPP__ */
