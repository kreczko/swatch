
namespace swatch {
namespace core {


//---
template<typename T>
XPointer<T>::XPointer() : ptr_(new T()) {
}


//---
template<typename T>
XPointer<T>::XPointer(T* ptr) : ptr_(ptr) {
}


//---
template<typename T>
XPointer<T>::XPointer(const XPointer& orig) : ptr_(orig.ptr_) {
}


//---
template<typename T>
XPointer<T>::~XPointer() {
}

//---
template<typename T>
void XPointer<T>::operator=(const XPointer<T>& b) {
  ptr_ = b.ptr_;
}


//---
template<typename T>
bool XPointer<T>::operator==(const XPointer<T>& b) const {
  return ptr_ == b.ptr_;
}


//---
template<typename T>
T& XPointer<T>::operator*() {
  return *ptr_;
}


//---
template<typename T>
const T& XPointer<T>::operator*() const {
  return *ptr_;
}


//---
template<typename T>
T* XPointer<T>::operator->() {
  return ptr_.operator->();
}


//---
template<typename T>
const T* XPointer<T>::operator->() const {
  return ptr_.operator->();
}


//---
template<typename T>
long XPointer<T>::count() const {
  return ptr_.use_count();
}


//---
template<typename T>
std::string XPointer<T>::type() const {
  return ptr_->type() + "*";
}


//---
template<typename T>
int XPointer<T>::equals(const xdata::Serializable& s) {
  // equals = 1 if the shared pointer is the same
  return (*this == dynamic_cast<const XPointer<T>&> (s));
}


//---
template<typename T>
void XPointer<T>::setValue(const xdata::Serializable& s) {
  // Cast the pointer 
  *this = dynamic_cast<const XPointer<T>&> (s);
}


//---
template<typename T>
void XPointer<T>::fromString(const std::string& s) throw (xdata::exception::Exception) {
  ptr_->fromString(s);
}


//---
template<typename T>
std::string XPointer<T>::toString() throw (xdata::exception::Exception) {
  return ptr_->toString();
}



} // namespace core
} // namespace swatch