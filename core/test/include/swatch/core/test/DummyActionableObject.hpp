/**
 * @file    DummyActionableObject.hpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#ifndef SWATCH_CORE_TEST_DUMMYACTIONABLEOBJECT_HPP
#define SWATCH_CORE_TEST_DUMMYACTIONABLEOBJECT_HPP


#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/Command.hpp"


namespace swatch {
namespace core {
namespace test {

class DummyActionableObject: public swatch::core::ActionableObject {
public:
  class MonChild : public MonitorableObject {
  public:
    MonChild(const std::string& aId, const DummyActionableObject& aObj);
    ~MonChild();
    template<class T>
    T& addMonitorable(T* aObj);
    
  protected:
    void retrieveMetricValues();
    
    const DummyActionableObject& mObj;
    swatch::core::Metric<uint32_t>& mDummyMetric;
  };
  
  class WaitCommand : public Command {
  public:
    WaitCommand(const std::string& aId, ActionableObject& aActionable);
    ~WaitCommand();
  protected:
    ActionSnapshot::State code(const XParameterSet& aParams);
  };

  
  DummyActionableObject(const std::string& aId);

  virtual ~DummyActionableObject();

  virtual const std::vector<std::string>& getGateKeeperContexts() const;

  uint32_t getNumber() const;

  void setNumber(uint32_t number);

  const Metric<uint32_t>& getDummyMetric() const;
  
  // Expose registerFunctionoid template method as public for tests
  template< typename T>
  T& registerCommand( const std::string& aId ) { return ActionableObject::registerCommand<T>(aId); }

  // Expose registerCommandSequence method as public for tests
  CommandSequence& registerSequence(const std::string& aId, const std::string& aFirstCmdId);
  
  // Expose registerCommandSequence method as public for tests
  CommandSequence& registerSequence(const std::string& aId, Command& aFirstCmd);

  // Expose registerStateMachine method as public for tests
  StateMachine& registerStateMachine(const std::string& aId, const std::string& aInitialState, const std::string& aErrorState);

  // Expose MonitorableObject::addMonitorable method as public for tests
  template<class T>
  T& addMonitorable(T* aObj); // { addMonitorable(aObj); }
  
  //! Tells object to wait next time the "wait" method is called; e.g. in WaitCommand or retrieveMetrics
  void pleaseWaitNextTime();

  //! Returns id of thread that's currently waiting; if no thread is currently executing the "wait" method 
  boost::thread::id getWaitingThread();
  
  //! Tells other thread in "wait" method to 
  void pleaseContinue();

private:

  void wait() const;
  
  void retrieveMetricValues();
    
  uint32_t mNumber;
  swatch::core::Metric<uint32_t>& mDummyMetric;

  bool mWait;
  mutable bool mContinue;
  mutable boost::thread::id mWaitingThread;
  mutable boost::mutex mMutex;
  mutable boost::condition_variable mConditionVar;

  mutable std::vector<std::string> mGateKeeperTables;
  
  DummyActionableObject( const DummyActionableObject& other ); // non copyable
  DummyActionableObject& operator=( const DummyActionableObject& ); // non copyable
};



template<class T>
T& DummyActionableObject::MonChild::addMonitorable(T* aObj)
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::MonitorableObject,T>::value) );
  MonitorableObject::addMonitorable(aObj);
  return *aObj;
};


template<class T>
T& DummyActionableObject::addMonitorable(T* aObj)
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::MonitorableObject,T>::value) );
  MonitorableObject::addMonitorable(aObj);
  return *aObj;
};
  
  
} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYACTIONABLEOBJECT_HPP */
