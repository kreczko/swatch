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
class ActionStatus {
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
  
  virtual ~ActionStatus();
  
  //! Returns the path of the action (command, sequence, transition, ...)
  const std::string& getActionPath() const;
  
  //! Returns state of execution (scheduled, running, done/warning/error)
  State getState() const;
  
  //! Returns action's running time in seconds
  float getRunningTime() const;
  
  //! Returns fractional progress of command; range [0,1]
  virtual float getProgress() const = 0;
  
protected:
  ActionStatus(const std::string& aPath, State aState, float aRunningTime);
  
private:
  std::string mPath;
  State mState;
  float mRunningTime;
};

    
std::ostream& operator<<(std::ostream& out, swatch::core::ActionStatus::State s);


class Functionoid : public Object {
public:
  /// Constructor
  Functionoid( const std::string& aId );

  /// Destructor
  virtual ~Functionoid();
  
protected:
  const Object* getParent() const ;

  Object* getParent();

  template<typename T> const T* getParent() const ;

  template<typename T> T* getParent();
  
public:
  typedef ActionStatus::State State;
  static const ActionStatus::State kInitial = ActionStatus::kInitial;
  static const ActionStatus::State kScheduled = ActionStatus::kScheduled;
  static const ActionStatus::State kRunning = ActionStatus::kRunning;
  static const ActionStatus::State kError = ActionStatus::kError;
  static const ActionStatus::State kWarning = ActionStatus::kWarning;
  static const ActionStatus::State kDone = ActionStatus::kDone;
};


DEFINE_SWATCH_EXCEPTION(InvalidResource);

} /* namespace core */
} /* namespace swatch */

#include "swatch/core/Functionoid.hxx"

#endif /* __SWATCH_CORE_Functionoid_HPP__ */
