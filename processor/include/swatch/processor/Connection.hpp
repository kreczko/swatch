/* 
 * File:   Connection.hpp
 * Author: ale
 *
 * Created on July 23, 2014, 12:03 PM
 */
#ifndef CONNECTION_HPP
#define	CONNECTION_HPP
// OTHER HEADERS

// Boost Headers
#include "boost/smart_ptr.hpp"
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>

// SWATCH HEADERS

// C++ HEADERS
#include <string>
#include <typeinfo>

namespace swatch {
namespace processor {

class Connection {
public:

    Connection() {
    }

    virtual ~Connection() {
    }

    template <class T> T& get() {
        if (typeid (T) != typeId()) {
            throw std::bad_cast();
        };
        return *(static_cast<T*> (connection_()));
    };

    template<typename T>
    static Connection* make(const T &value);

    template<typename T>
    static Connection* make(T* ptr);

    virtual const std::type_info & typeId() const = 0;
    virtual const std::string & typeName() const = 0;

protected:
    virtual void * connection_() = 0;
    static std::string demangleName(const char* iMangledName);

};

template<class T>
class ConnectionHolder : public Connection {
public:

    /**
     * Default constructor
     */
    ConnectionHolder() : obj_(0x0) {
    }

    /**
     * Reference-based constructor
     * @param data
     */
    ConnectionHolder(const T &data) : obj_(new T(data)) {
    } //{ std::cout << "Reference constructor (" << typeName() << ", " << this->obj_ << ")" << std::endl; } // copy the object

    /**
     * Pointer-based constructor
     * @param data
     */
    ConnectionHolder(T* data) : obj_(data) {
    } //{ std::cout << "Pointer constructor(" << typeName() << ", " << this->obj_ << ")" << std::endl; } // adopt the pointers

    /**
     * Destructor
     */
    virtual ~ConnectionHolder() {
        delete obj_;
    }

    virtual const std::type_info & typeId() const {
        return typeid (T);
    }

    virtual const std::string & typeName() const {
        return typeName_();
    }
protected:

    virtual void * connection_() {
        return obj_;
    }
private:

    T* obj_;
    static const std::string & typeName_();

};

template<typename T>
const std::string & ConnectionHolder<T>::typeName_() {
    static const std::string name(demangleName(typeid (T).name()));
    return name;
}

template<typename T>
Connection* Connection::make(const T &value) {
    return new ConnectionHolder<T>(value);
}

template<typename T>
Connection* Connection::make(T* ptr) {
    return new ConnectionHolder<T>(ptr);
}

}
}

#endif	/* CONNECTION_HPP */

