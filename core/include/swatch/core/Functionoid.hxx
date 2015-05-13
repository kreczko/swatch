
namespace swatch {
namespace core {
template<typename T>
T* Functionoid::getParent() {
  return dynamic_cast<T*>( Object::getParent() );
}
} // namespace core
} // namespace swatch
