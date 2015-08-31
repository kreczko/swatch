

namespace swatch {
namespace core {

template<typename T>
const T* Functionoid::getParent() const {
  return dynamic_cast<const T*>( getAncestor(1) );
}


template<typename T>
T* Functionoid::getParent() {
  return dynamic_cast<T*>( getAncestor(1) );
}


} // namespace core
} // namespace swatch
