/*
 * Serializable.cpp
 *
 *  Created on: 22.05.2013
 *      Author: morpheby
 */

#include "Serializable.h"

namespace serializing {

std::map<std::string, std::shared_ptr<_internal::SerializableConstructorBase>> Serializable::constructors_;

Serializable::Serializable() : registered_{false} {

}

Serializable::~Serializable() {
}

void Serializable::Serialize(std::ostream &output) {
	EnsureFieldsRegistered();
	for(auto o : fields_)
		o->Serialize(output);
}

void Serializable::Deserialize(std::istream &input) {
	for(auto o : fields_)
		o->Deserialize(input);
}

std::shared_ptr<Serializable> Serializable::ConstructType(const std::string &name) {
	if(constructors_.count(name))
		return (*constructors_[name])();
	else
		throw "Can't find derived type for deserialization";
}

void Serializable::EnsureFieldsRegistered() {
	if(!registered_) {
		fields_.clear();
		RegisterFields();
	}
}

_internal::SFieldBase::SFieldBase() {
}

_internal::SFieldBase::~SFieldBase() {
}

_internal::SerializableConstructorBase::~SerializableConstructorBase() {
}

} /* namespace serializing */
