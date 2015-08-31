
#include <iostream>
#include "swatch/core/StandardOperation.hpp"

#include "toolbox/fsm/exception/Exception.h"

// using namespace swatch::logger;

namespace swatch {
namespace core {
StandardOperation::StandardOperation( const std::string& aId ) :
        Operation( aId )
{

  fsm_->addState(Halted);
  fsm_->addState(Synchronised);
  fsm_->addState(Connected);
  fsm_->addState(Configured);
  fsm_->addState(Enabled);
  fsm_->addState(Suspended);
  fsm_->addState(Failed);

  fsm_->setInitialState(Halted);
  fsm_->reset();

/*
  addTransition( Halted,       Halted,       ColdReset, mDummyCommandSequence );
  addTransition( Halted,       Synchronised, Setup,     mDummyCommandSequence );
  addTransition( Synchronised, Connected,    Connect,   mDummyCommandSequence );
  addTransition( Connected,    Configured,   Configure, mDummyCommandSequence );
  addTransition( Configured,   Enabled,      Enable,    mDummyCommandSequence );
  addTransition( Enabled,      Suspended,    Suspend,   mDummyCommandSequence );
  addTransition( Enabled,      Configured,   Stop,      mDummyCommandSequence );
  addTransition( Suspended,    Enabled,      Enable,    mDummyCommandSequence );
  addTransition( Suspended,    Configured,   Stop,      mDummyCommandSequence );
 */
}


StandardOperation::~StandardOperation() {
}


// Static members
// static const std::string mHalted, mSynchronised, mConnected, mConfigured, mEnabled, mSuspended, mFailed;
const std::string StandardOperation::Halted("HALTED");
const std::string StandardOperation::Synchronised("SYNCHRONISED");
const std::string StandardOperation::Connected("CONNECTED");
const std::string StandardOperation::Configured("CONFIGURED");
const std::string StandardOperation::Enabled("ENABLED");
const std::string StandardOperation::Suspended("SUSPENDED");
const std::string StandardOperation::Failed("FAILED");

// static const std::string mColdReset, mSetup, mConnect, mConfigure, mEnable, mSuspend, mStop;
const std::string StandardOperation::ColdReset("COLDRESET");
const std::string StandardOperation::Setup("SETUP");
const std::string StandardOperation::Connect("CONNECT");
const std::string StandardOperation::Configure("CONFIGURE");
const std::string StandardOperation::Enable("ENABLE");
const std::string StandardOperation::Suspend("SUSPEND");
const std::string StandardOperation::Stop("STOP");



//StandardOperation::DummyCommandSequence::DummyCommandSequence() :
//CommandSequence( "DummyCommandSequence" )
//{}
//
//void StandardOperation::DummyCommandSequence::exec()
//{
//  std::cout << "Dummy exec" << std::endl;
//}
//
//std::vector<std::string>* StandardOperation::DummyCommandSequence::setTables()
//{
//  return NULL;
//}



} /* namespace core */
} /* namespace swatch */
