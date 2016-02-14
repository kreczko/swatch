
#include "swatch/core/Object.hpp"


#include <iostream>

#include "boost/lexical_cast.hpp"

#include "swatch/core/Utilities.hpp"


using namespace std;


namespace swatch {
namespace core {


Object::Object(const std::string& aId) : 
  mId(aId), 
  mParent(0x0)
{
  if (mId.empty())
    throw InvalidObjectId("Cannot create object with empty ID string");
  else if(mId.find('.') != std::string::npos)
    throw InvalidObjectId("Object ID string '" + mId + "' contains a dot at position "+boost::lexical_cast<std::string>(mId.find('.')));
}


Object::~Object() {
  mObjectsChart.clear();

  for (std::deque< std::pair<Object*,Deleter*> >::iterator it(mChildren.begin()); it != mChildren.end(); ++it) {
    if ( it->second != NULL )
    {
      (*it->second)(it->first);
      delete it->second;
    }
    else
      delete it->first;
  }
}


std::string Object::getTypeName() const {
  return demangleName(typeid(*this).name());
}


void Object::setParent(Object* aParent) {
  if (mParent) {
    stringstream ss;
    ss << "Cannot set ancestor of " << this->getPath() << ". Already set to " << mParent->getPath();
    throw runtime_error(ss.str());
  }

  mParent = aParent;
}


void Object::addObj(Object* aChild) {
  aChild->setParent(this);

  // Insure the child does not have a twin
  if (mObjectsChart.find(aChild->getId()) != mObjectsChart.end()) {
    throw std::runtime_error(aChild->getId() + " already exists in this family");
  }

  //    cout << this->id() << " - Adding " << aChild->id() << endl;
  mChildren.push_back(std::make_pair(aChild, (Deleter*)NULL));
  mObjectsChart.insert(std::make_pair(aChild->getId(), aChild));
}


const Object* Object::getAncestor(const uint32_t& aDepth) const {
  if (!aDepth) {
    return this;
  }

  if (!mParent) {
    return NULL;
  }

  return mParent-> getAncestor(aDepth - 1);
}


Object* Object::getAncestor(const uint32_t& aDepth) {
  if (!aDepth) {
    return this;
  }

  if (!mParent) {
    return NULL;
  }

  return mParent-> getAncestor(aDepth - 1);
}


const std::string& Object::getId() const {
  return mId;
}


std::string Object::getPath() const {
  std::deque< const Object* > lGenealogy;
  getAncestors(lGenealogy);
  std::string lRet;

  for (std::deque< const Object* >::iterator lIt(lGenealogy.begin()); lIt != lGenealogy.end(); ++lIt) {
    if ((**lIt).mId.size()) {
      lRet += (**lIt).mId;
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

  if (mParent) {
    mParent->getAncestors(aGenealogy);
  }
}


void Object::print(std::ostream& aStr, const uint32_t& aIndent) const {
  aStr << '\n' << std::string(aIndent, ' ') << "- [" << getTypeName() << "] " << mId;

  for (std::deque< std::pair<Object*,Deleter*> >::const_iterator lIt = mChildren.begin(); lIt != mChildren.end(); ++lIt) {
    (*lIt->first).print(aStr, aIndent + 1);
  }
}


void Object::getCharts(const std::string& aBasePath, boost::unordered_map<std::string, Object*>& aChart) const {
  std::string prefix(aBasePath.empty() ? "" : aBasePath + '.');
  // 1) Add children
  boost::unordered_map<std::string, Object*>::const_iterator itMap;

  for (itMap = mObjectsChart.begin(); itMap != mObjectsChart.end(); ++itMap) {
    aChart.insert(std::make_pair(prefix + itMap->first, itMap->second));
  }

  // 2) Add children's children (ad infinitum) by recursive calls
  std::deque< std::pair<Object*,Deleter*> >::const_iterator it;

  for (it = mChildren.begin(); it != mChildren.end(); ++it) {
    it->first->getCharts(prefix + it->first->mId, aChart);
  }
}


std::vector<std::string> Object::getChildren() const {
  std::vector<std::string> names;
  names.reserve(mObjectsChart.size());

  for (boost::unordered_map<std::string, Object*>::const_iterator it = mObjectsChart.begin(); it != mObjectsChart.end(); it++) {
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


Object& Object::getObj(const std::string& aId) {
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

  if ((it = mObjectsChart.find(father)) == mObjectsChart.end()) {
    stringstream ss;
    ss << "Object " << father << " not found in " << mId;
    throw ObjectDoesNotExist(ss.str());
  }

  if (child.empty()) {
    return *(it->second);
  }

  return it->second->getObj(child);
}


const Object& Object::getObj(const std::string& aId) const {
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

  if ((it = mObjectsChart.find(father)) == mObjectsChart.end()) {
    stringstream ss;
    ss << "Object " << father << " not found in " << mId;
    throw ObjectDoesNotExist(ss.str());
  }

  if (child.empty()) {
    return *(it->second);
  }

  return it->second->getObj(child);
}


// TODO: include self

std::vector<std::string> Object::getPaths() const {
  std::vector<std::string> names;
  names.reserve(mObjectsChart.size());
  boost::unordered_map<std::string, Object*>::const_iterator it;

  for (it = mObjectsChart.begin(); it != mObjectsChart.end(); ++it) {
    names.push_back(it->second->getPath());
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
  if (mObjectsChart.find(aChild->getId()) != mObjectsChart.end()) {
    throw std::runtime_error(aChild->getId() + " already exists in this family");
  }

  mObjectsChart.insert(std::make_pair(aAlias, aChild));
}


void ObjectView::addObj(Object* aChild) {
  this->addObj(aChild, aChild->getId());
}


// Object::iterator Methods Implementation

Object::iterator::iterator() : mBegin() {
}


Object::iterator::iterator(Object* aBegin) : mBegin(aBegin) {
}


Object::iterator::iterator(const iterator& aOrig) : mBegin(aOrig.mBegin), mItStack(aOrig.mItStack) {
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
  return ( mItStack.size()) ? ( * mItStack[0]->first) : (*mBegin);
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
  return ( aIt.mBegin == mBegin) && (aIt.mItStack == mItStack);
}


bool Object::iterator::next() {
  // Null iterator can't be incremented...
  if (!mBegin) {
    return false;
  }

  if (!mItStack.size()) {
    //We have just started and have no stack...
    if (mBegin->mChildren.size()) {
      //We have children so recurse down to them
      mItStack.push_front(mBegin->mChildren.begin());
      return true;
    }

    //We have no children so we are at the end of the iteration. Make Buffer NULL to stop infinite loop
    mBegin = NULL;
    return false;
  }

  //We are already in the tree...
  if (mItStack[0]->first->mChildren.size()) {
    // Entry has children, recurse...
    mItStack.push_front(mItStack[0]->first->mChildren.begin());
    return true;
  }

  // No children so go to the next entry on this level
  while (mItStack.size()) {
    if (++(mItStack[0]) != ((mItStack.size() == 1) ? (*mBegin) : (*mItStack[1]->first)).mChildren.end()) {
      // Next entry on this level is valid - return
      return true;
    }

    // No more valid entries in this level, go back up tree
    mItStack.pop_front();
  }

  //We have no more children so we are at the end of the iteration. Make Buffer NULL to stop infinite loop
  mBegin = NULL;
  return false;
}


std::ostream& operator<<(std::ostream& aStr, const swatch::core::Object& aObject) {
  aObject.print(aStr);
  return aStr;
}

}
}

