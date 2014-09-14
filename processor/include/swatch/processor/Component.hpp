/**
 * @file    Component.hpp
 * @author  Alessandro Thea
 * @brief   Device, class representing the trigger network node.
 * @date    20.08.2014
 *
 */

#ifndef SWATCH_PROCESSOR_COMPONENT_HPP
#define SWATCH_PROCESSOR_COMPONENT_HPP


namespace swatch {
namespace processor {

class Connection;

class Component {
public:
    virtual ~Component() {}

    virtual const Connection* connection() const { return connection_; }
protected:
    Component(Connection* aConnection) : connection_(aConnection) {}

    Connection* connection_;
};

} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_COMPONENT_HPP */
