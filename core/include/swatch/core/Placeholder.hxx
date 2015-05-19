#include "swatch/core/Utilities.hpp"

namespace swatch {
namespace core {


template<typename T>
Placeholder<T>::Placeholder() {}


template<typename T>
Placeholder<T>::~Placeholder() {}


template<typename T>
std::string Placeholder<T>::type() const {
  T lT;
  return "Placeholder<" + lT.type() + ">";
}


template<typename T>
int Placeholder<T>::equals(const xdata::Serializable& s) {
  // equals = 1 if both are placeholders for the same type
  return ( dynamic_cast< const Placeholder<T>* > (&s) != NULL);
}


template<typename T>
void Placeholder<T>::setValue(const xdata::Serializable& s) {}


template<typename T>
void Placeholder<T>::fromString(const std::string& s) throw (xdata::exception::Exception) {}


template<typename T>
std::string Placeholder<T>::toString() throw (xdata::exception::Exception) {
  return type();
}



} // namespace core
} // namespace swatch