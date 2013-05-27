/*
 * Serializable.hpp
 * wincpp
 *
 * Created by Илья Михальцов on 2013-05-26.
 * Copyright 2013 Илья Михальцов. All rights reserved.
 */

#include "serialized_io.h"

namespace serializing {

template<class T>
bool Serializable::RegisterSerializableClass() {
	constructors_[typeid(T).name()] = std::make_shared<_internal::SerializableConstructor<T>>();
}

template<typename T>
void Serializable::RegisterField(T &field) {
	fields_.push_back(_internal::make_sfield(field));
}

template<typename T>
void Serializable::RegisterField(T &field, EventBase<_internal::SFieldNotifying<T>> *onDeserializeEvent) {
	fields_.push_back(_internal::make_sfield(field, onDeserializeEvent));
}

namespace _internal {

template<typename T>
SField<T>::SField(T &data) : SField_helper<T, SField_type<T>::value>(data) {
}

template<typename T>
SFieldNotifying<T>::SFieldNotifying(T &data, EventBase<SFieldNotifying<T>> *onDeserializeEvent) : SField_helper<T, SField_type<T>::value>(data) {
	event_ = onDeserializeEvent;
}

template<typename T>
void SFieldNotifying<T>::Deserialize(std::istream &input) {
	SField_helper<T, SField_type<T>::value>::Deserialize(input);
	event_(*this);
}

template<class T>
SerializableConstructor<T>::SerializableConstructor() {
}

template<class T>
std::shared_ptr<Serializable> SerializableConstructor<T>::operator() () {
	return std::make_shared<T>();
}

template<typename T>
SField_helper<T, _POD>::SField_helper(T& data) : data_(data) {
}

template<typename T>
void SField_helper<T, _POD>::Serialize(std::ostream &output) {
	_io::write_data(&data_, sizeof(T), output);
}

template<typename T>
void SField_helper<T, _POD>::Deserialize(std::istream &input) {
	_io::read_data(&data_, sizeof(T), input);
}

template<typename T>
SField_helper<T, _SSharedPtr>::SField_helper(T& value) : value_(value) {
}

template<typename T>
void SField_helper<T, _SSharedPtr>::Serialize(std::ostream &output) {
	if(!value_) {
		_io::write_name("null", output);
		return;
	}
	_io::write_name(typeid(*value_).name(), output);
	value_->Serialize(output);
}

template<typename T>
void SField_helper<T, _SSharedPtr>::Deserialize(std::istream &input) {
	std::string name = _io::read_name(input);
	if(name == "null") {
		value_ = nullptr;
		return;
	}

	value_ = std::dynamic_pointer_cast<typename T::element_type>(Serializable::ConstructType(name));
	value_->Deserialize(input);
}

/*** DEPRECATED ***/
// template<typename T>
// SField_helper<T, _PODContainer>::SField_helper(T& container) : container_(container) {
// }
//
// template<typename T>
// void SField_helper<T, _PODContainer>::Serialize(std::ostream &output) {
// 	_io::write_container(container_, output);
// }
//
// template<typename T>
// void SField_helper<T, _PODContainer>::Deserialize(std::istream &input) {
// 	_io::read_container(container_, input);
// }

template<typename T>
SField_helper<T, _Container>::SField_helper(T& container) : container_(container) {
}

template<typename T>
void SField_helper<T, _Container>::Serialize(std::ostream &output) {
	_io::write_container_size(container_, output);
	for(auto x : container_) {
		make_sfield(x)->Serialize(output);
	}
}

template<typename T>
void SField_helper<T, _Container>::Deserialize(std::istream &input) {
	size_t sz = _io::read_container_size(input);
	for(size_t i = 0; i < sz; ++i) {
		typename T::value_type x;
		auto f = make_sfield(x);
		f->Deserialize(input);
		__add_to_container(container_, x);
	}
}

template<typename T>
SField_helper<T, _Pair>::SField_helper(T& pair) : pair_(pair) {
}

template<typename T>
void SField_helper<T, _Pair>::Serialize(std::ostream &output) {
	make_sfield(pair_.first)->Serialize(output);
	make_sfield(pair_.second)->Serialize(output);
}

template<typename T>
void SField_helper<T, _Pair>::Deserialize(std::istream &input) {
	make_sfield(pair_.first)->Deserialize(input);
	make_sfield(pair_.second)->Deserialize(input);
}

template<typename T>
std::shared_ptr<SFieldBase> make_sfield(T &field) {
	return std::make_shared<SField<T>>(field);
}

template<typename T>
std::shared_ptr<SFieldBase> make_sfield(T &field, EventBase<SFieldNotifying<T>> *onDeserializeEvent) {
	return std::make_shared<SFieldNotifying<T>>(field, onDeserializeEvent);
}

template<typename _Tp, typename _Up>
typename __void_type<decltype(static_cast<std::pair<typename _Tp::iterator,bool> (_Tp::*)(const typename _Tp::const_reference)>(&_Tp::insert))>::type
__add_to_container(_Tp &container, _Up value) {
	container.insert(value);
}

template<typename _Tp, typename _Up>
typename __void_type<decltype(static_cast<void (_Tp::*)(const typename _Tp::const_reference)>(&_Tp::push_back))>::type
__add_to_container(_Tp &container, _Up value) {
	container.push_back(value);
}

template<typename _Tp, typename _Up>
typename __void_type<decltype(static_cast<void (_Tp::*)(typename _Tp::value_type)>(&_Tp::push_back))>::type
__add_to_container(_Tp &container, _Up value) {
	container.push_back(value);
}

} /* namespace serializing::_internal */

} /* namespace serializing */
