
namespace swatch {
namespace core {
template<typename T>
T* Functionoid::getParent( const uint32_t& aDepth ) {
  return dynamic_cast<T*>( getAncestor(aDepth) );
}
} // namespace core
} // namespace swatch
