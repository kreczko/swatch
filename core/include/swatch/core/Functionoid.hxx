
namespace swatch {
namespace core {
template<typename T>
T* Functionoid::getParent() {
  return dynamic_cast<T*>( Object::getAncestor(1) );
}
} // namespace core
} // namespace swatch
