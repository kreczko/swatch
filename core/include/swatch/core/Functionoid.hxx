
namespace swatch {
namespace core {
template<typename T>
T* Functionoid::getParent( const uint32_t& aDepth ) {
  return dynamic_cast<T*>( getAncestor(aDepth) );
}

template<typename T>
void Functionoid::registerParameter(const std::string name, const T& defaultValue){
  if (parameters_.has(name))
    parameters_.set(name, defaultValue);
  else
    parameters_.add(name, defaultValue);
}
} // namespace core
} // namespace swatch
