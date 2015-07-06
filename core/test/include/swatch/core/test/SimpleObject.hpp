/**
 * @file    SimpleObject.hpp
 * @author  Alessandro Thea
 * @brief   Simple Object-derived class to exercise the Object interface
 * @date    20.08.2014
 *
 * Detailed description
 */

#ifndef SWATCH_CORE_TEST_SIMPLEOBJECT_HPP
#define	SWATCH_CORE_TEST_SIMPLEOBJECT_HPP

#include "swatch/core/AbstractStub.hpp"

namespace swatch {
namespace core {
namespace test {

//____________________________________________________________________________//
class SimpleObject : public  swatch::core::Object {
public:
    explicit SimpleObject( const std::string& aId ) : swatch::core::Object( aId ){}
//    SimpleObject( const std::string& aId, const XParameterSet& params ) : swatch::core::Object( aId, params ){}
    SimpleObject( const AbstractStub& aStub ) : swatch::core::Object( aStub.id ){}
    virtual ~SimpleObject() {}
    
    void add( swatch::core::Object* aChild ) { this->Object::addObj(aChild); }
    void setValue( double v ) { value_ = v; }
    double getValue() { return value_; }
    
private:
    //! simple data member
    double value_;
};

class SimpleView : public  swatch::core::ObjectView {
public:
    explicit SimpleView( const std::string& aId ) : swatch::core::ObjectView(aId ){}
//    SimpleView( const std::string& aId, const swatch::core::XParameterSet& params ) : swatch::core::ObjectView(aId, params) {}
    SimpleView( const AbstractStub& aStub ) : swatch::core::ObjectView(aStub.id) {}
    virtual ~SimpleView() {}
    
    void add( swatch::core::Object* aChild, const std::string& aAlias ) { this->ObjectView::addObj(aChild, aAlias); }
};

} // namespace swatch
} // namespace core
} // namespace test




#endif	/* SWATCH_CORE_TEST_SIMPLEOBJECT_HPP */

