#include "swatch/core/ParameterSet.hpp"

namespace swatch {
namespace core {

ParameterSet::ParameterSet() {
}

ParameterSet::~ParameterSet() {
}

std::set<std::string> ParameterSet::names() const {
    std::set<std::string> names;
    for (const_iterator it = begin(); it != end(); ++it)
        names.insert(it->first);

    return names;
}

bool
ParameterSet::has(const std::string& aKey) const {
    return (find(aKey) != end());
}

void ParameterSet::set(const std::string& aKey, const char* aValue) {
    this->parent::operator[](aKey) = std::string(aValue);
}

std::string
ParameterSet::get(const std::string& aKey, const char* aDefault ) const {
    const_iterator it = this->find(aKey);

    if (it == this->end()) return aDefault;
    if ( typeid(std::string) != it->second.type() ) {
        std::cout << "Cannot cast " << demangleName(it->second.type().name()) << " to " << demangleName(typeid(std::string).name()) << std::endl;
    }
    
    return boost::any_cast<std::string>(it->second);
}

const ParameterSet::ParameterBase&
ParameterSet::get(const std::string& aKey) const {
    const_iterator it = this->find(aKey);
    //TODO runtime_error -> dedicated error
    if (it == this->end()) throw std::runtime_error("Parameter not found");

    return it->second;
}

std::string
ParameterSet::asstring(const std::string& aKey) const {
    
    ParameterBase p = get(aKey);
    return anyToString(p);

}

ParameterSet::Inserter::Inserter(ParameterSet* ps) {
    mPS = ps;
}

//template<>

ParameterSet::Inserter& ParameterSet::Inserter::operator()(const std::string& aKey, const char* aValue) {
    mPS->set(aKey, std::string(aValue));
    return *this;
}

//template<>

ParameterSet::Inserter ParameterSet::insert(const std::string& aKey, const char* aValue) {
    this->set(aKey, std::string(aValue));
    return Inserter(this);
}

}
}
