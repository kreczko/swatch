
#include "swatch/core/Object.hpp"


#include <iostream>

#include "swatch/core/Utilities.hpp"


using namespace std;


namespace swatch {
namespace core {


Object::Object(const std::string& aId) : id_(aId), parent_(0x0) {
}


Object::~Object() {
  objectsChart_.clear();

  for (std::deque< Object* >::iterator it(children_.begin()); it != children_.end(); ++it) {
    //        cout << "Deleting " << ( *it )->typeName() << " addr=" << *it << " uid=" << ( *it )->uid() << endl;
    delete *it;
  }
}

const std::type_info& Object::type() const {
  return typeid ( *this);
}

std::string Object::typeName() const {
  return demangleName(this->type().name());
}

void Object::setParent(Object* aParent) {
  if (parent_) {
    stringstream ss;
    ss << "Cannot set ancestor of " << this->path() << ". Already set to " << parent_->path();
    throw runtime_error(ss.str());
  }

  parent_ = aParent;
}

void Object::addObj(Object* aChild) {
  aChild->setParent(this);

  // Insure the child does not have a twin
  if (objectsChart_.find(aChild->id()) != objectsChart_.end()) {
    throw std::runtime_error(aChild->id() + " already exists in this family");
  }

  //    cout << this->id() << " - Adding " << aChild->id() << endl;
  children_.push_back(aChild);
  objectsChart_.insert(std::make_pair(aChild->id(), aChild));
}

Object* Object::getAncestor(const uint32_t& aDepth) {
  if (!aDepth) {
    return this;
  }

  if (!parent_) {
    return NULL;
  }

  return parent_-> getAncestor(aDepth - 1);
}

const std::string& Object::id() const {
  return id_;
}

const std::string Object::path() const {
  std::deque< const Object* > lGenealogy;
  getAncestors(lGenealogy);
  std::string lRet;

  for (std::deque< const Object* >::iterator lIt(lGenealogy.begin()); lIt != lGenealogy.end(); ++lIt) {
    if ((**lIt).id_.size()) {
      lRet += (**lIt).id_;
      lRet += ".";
    }
  }

  if (lRet.size()) {
    lRet.resize(lRet.size() - 1);
  }

  return lRet;
}

void Object::getAncestors(std::deque<const Object*>& aGenealogy) const {
  aGenealogy.push_front(this);

  if (parent_) {
    parent_->getAncestors(aGenealogy);
  }
}

void Object::print(std::ostream& aStr, const uint32_t& aIndent) const {
  aStr << '\n' << std::string(aIndent, ' ') << "- [" << typeName() << "] " << id_;

  for (std::deque<Object*>::const_iterator lIt = children_.begin(); lIt != children_.end(); ++lIt) {
    (**lIt).print(aStr, aIndent + 1);
  }
}

void Object::getCharts(const std::string& basePath, boost::unordered_map<std::string, Object*>& chart) const {
  std::string prefix(basePath.empty() ? "" : basePath + '.');
  // 1) Add children
  boost::unordered_map<std::string, Object*>::const_iterator itMap;

  for (itMap = objectsChart_.begin(); itMap != objectsChart_.end(); ++itMap) {
    chart.insert(std::make_pair(prefix + itMap->first, itMap->second));
  }

  // 2) Add children's children (ad infinitum) by recursive calls
  std::deque<Object*>::const_iterator it;

  for (it = children_.begin(); it != children_.end(); ++it) {
    (*it)->getCharts(prefix + (*it)->id_, chart);
  }
}

std::vector<std::string> Object::getChildren() const {
  std::vector<std::string> names;
  names.reserve(objectsChart_.size());

  for (boost::unordered_map<std::string, Object*>::const_iterator it = objectsChart_.begin(); it != objectsChart_.end(); it++) {
    names.push_back(it->first);
  }

  return names;
}

std::vector<std::string> Object::getDescendants() const {
  boost::unordered_map<std::string, Object*> chart;
  this->getCharts("", chart);
  std::vector<std::string> names;
  names.reserve(chart.size());
  boost::unordered_map<std::string, Object*>::const_iterator it;

  for (it = chart.begin(); it != chart.end(); ++it) {
    names.push_back(it->first);
  }

  return names;
}

Object& Object::getObj(const std::string& aId) const {
  std::size_t pos;
  string father, child;

  if ((pos = aId.find('.')) != std::string::npos) {
    father = aId.substr(0, pos);
    child = aId.substr(pos + 1);
  } else {
    father = aId;
  }

  //    cout << "father = '" << father << "'   child = '" << child << "'" << endl;
  boost::unordered_map<std::string, Object*>::const_iterator it;

  if ((it = objectsChart_.find(father)) == objectsChart_.end()) {
    stringstream ss;
    ss << "Object " << father << " not found in " << id_;
    throw runtime_error(ss.str());
  }

  if (child.empty()) {
    return *(it->second);
  }

  return it->second->getObj(child);
}


// TODO: include self

std::vector<std::string>
Object::getPaths() const {
  std::vector<std::string> names;
  names.reserve(objectsChart_.size());
  boost::unordered_map<std::string, Object*>::const_iterator it;

  for (it = objectsChart_.begin(); it != objectsChart_.end(); ++it) {
    names.push_back(it->second->path());
  }

  return names;
}

Object::iterator Object::begin() {
  Object::iterator lIt(this);
  return lIt;
}

Object::iterator Object::end() {
  Object::iterator lIt;
  return lIt;
}

// Object View Methods Implementation

ObjectView::ObjectView(const std::string& aId) :
  Object(aId) {
}

/*
ObjectView::ObjectView(const std::string& aId, const XParameterSet& params) :
  Object(aId, params) {
}
*/

ObjectView::~ObjectView() {
  // clear the maps before ~Object, which would destroy them
  //    mChildren.clear();
  //    mObjectsChart.clear();
}

void ObjectView::addObj(Object* aChild, const std::string& aAlias) {
  // Insure the child does not have a twin
  if (objectsChart_.find(aChild->id()) != objectsChart_.end()) {
    throw std::runtime_error(aChild->id() + " already exists in this family");
  }

  objectsChart_.insert(std::make_pair(aAlias, aChild));
}

void ObjectView::addObj(Object* aChild) {
  this->addObj(aChild, aChild->id());
}


// Object::iterator Methods Implementation

Object::iterator::iterator() : begin_() {
}

Object::iterator::iterator(Object* aBegin) : begin_(aBegin) {
}

Object::iterator::iterator(const iterator& aOrig) : begin_(aOrig.begin_), itStack_(aOrig.itStack_) {
}

Object::iterator::~iterator() {
}

Object& Object::iterator::operator*() const {
  return value();
}

Object* Object::iterator::operator->() const {
  return & (value());
}

Object& Object::iterator::value() const {
  return ( itStack_.size()) ? (**itStack_[0]) : (*begin_);
}

Object::iterator& Object::iterator::operator++() {
  next();
  return *this;
}

Object::iterator Object::iterator::operator++(int) {
  Object::iterator lTemp(*this);
  next();
  return lTemp;
}

bool Object::iterator::operator!=(const Object::iterator& aIt) const {
  return !(*this == aIt);
}

bool Object::iterator::operator==(const Object::iterator& aIt) const {
  return ( aIt.begin_ == begin_) && (aIt.itStack_ == itStack_);
}

bool Object::iterator::next() {
  // Null iterator can't be incremented...
  if (!begin_) {
    return false;
  }

  if (!itStack_.size()) {
    //We have just started and have no stack...
    if (begin_->children_.size()) {
      //We have children so recurse down to them
      itStack_.push_front(begin_->children_.begin());
      return true;
    }

    //We have no children so we are at the end of the iteration. Make Buffer NULL to stop infinite loop
    begin_ = NULL;
    return false;
  }

  //We are already in the tree...
  if ((**itStack_[0]).children_.size()) {
    // Entry has children, recurse...
    itStack_.push_front((**itStack_[0]).children_.begin());
    return true;
  }

  // No children so go to the next entry on this level
  while (itStack_.size()) {
    if (++(itStack_[0]) != ((itStack_.size() == 1) ? (*begin_) : (**itStack_[1])).children_.end()) {
      // Next entry on this level is valid - return
      return true;
    }

    // No more valid entries in this level, go back up tree
    itStack_.pop_front();
  }

  //We have no more children so we are at the end of the iteration. Make Buffer NULL to stop infinite loop
  begin_ = NULL;
  return false;
}

std::ostream& operator<<(std::ostream& aStr, const swatch::core::Object& aObject) {
  aObject.print(aStr);
  return aStr;
}

}
}

