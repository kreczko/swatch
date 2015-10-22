/**
 * @file    DummyObject.hpp
 * @author  Alessandro Thea
 * @brief   Dummy Object-derived class to exercise the Object interface
 * @date    20.08.2014
 *
 * Detailed description
 */

#ifndef SWATCH_CORE_TEST_DUMMYOBJECT_HPP
#define	SWATCH_CORE_TEST_DUMMYOBJECT_HPP


#include "swatch/core/AbstractStub.hpp"
#include "swatch/core/Object.hpp"


namespace swatch {
namespace core {
namespace test {

//____________________________________________________________________________//
class DummyObject : public  swatch::core::Object {
public:
    explicit DummyObject( const std::string& aId ) : swatch::core::Object( aId ){}
    DummyObject( const AbstractStub& aStub ) : swatch::core::Object( aStub.id ){}
    virtual ~DummyObject() {}
    
    void add( swatch::core::Object* aChild ) { this->Object::addObj(aChild); }
    void setValue( double v ) { value_ = v; }
    double getValue() { return value_; }
    
private:
    //! simple data member
    double value_;
};

class DummyView : public  swatch::core::ObjectView {
public:
    explicit DummyView( const std::string& aId ) : swatch::core::ObjectView(aId ){}
    DummyView( const AbstractStub& aStub ) : swatch::core::ObjectView(aStub.id) {}
    virtual ~DummyView() {}
    
    void add( swatch::core::Object* aChild, const std::string& aAlias ) { this->ObjectView::addObj(aChild, aAlias); }
};

} // namespace swatch
} // namespace core
} // namespace test




#endif	/* SWATCH_CORE_TEST_SIMPLEOBJECT_HPP */

