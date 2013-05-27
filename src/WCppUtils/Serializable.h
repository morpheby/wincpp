/*
 * Serializable.h
 *
 *  Created on: 22.05.2013
 *      Author: morpheby
 */

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <iostream>
#include <sharedPtr.h>
#include <vector>
#include <map>
#include <type_traits>
#include <typeinfo>
#include <Events.h>

namespace serializing {

class Serializable;

namespace _internal {

template<typename>
struct __void_type {
	typedef void type;
};

template<typename T, typename _Enable = void>
struct __container_type {};

template<typename T>
struct __container_type<T, typename __void_type<typename T::value_type>::type> {
	typedef typename T::value_type type;
};

template<typename T, typename _Enable = void>
struct __shared_ptr_type {};

template<typename T>
struct __shared_ptr_type<T, typename __void_type<typename T::element_type>::type> {
	typedef typename T::element_type type;
};

template<typename T, typename _Enable = void>
struct __pair_types {};

template<typename T>
struct __pair_types<T, typename __void_type<typename T::first_type>::type> {
	typedef typename T::first_type first_type;
	typedef typename T::second_type second_type;
};


class SFieldBase {
public:
	SFieldBase();
	virtual ~SFieldBase();

	virtual void Serialize(std::ostream &output) = 0;
	virtual void Deserialize(std::istream &input) = 0;
};

template<typename T, class _This = void>
struct SField_type;

enum _SField_type_int {
	_POD,
	// _SContainer, // deprecated
	_Container,
	_SSharedPtr,
	// _PODContainer, // deprecated
	_Pair
};

template<typename T>
struct SField_type<T, typename std::enable_if<std::is_pod<T>::value>::type> {
	enum { value = _POD }; // POD
};
template<typename T>
struct SField_type<T, typename std::enable_if<std::is_base_of<Serializable, typename __shared_ptr_type<T>::type>::value>::type> {
	enum { value = _SSharedPtr }; // Shared_ptr serizlizable
};
template<typename T>
struct SField_type<T, typename __void_type<typename __container_type<T>::type>::type> {
	enum { value = _Container }; // Container of anything
};
template<typename T>
struct SField_type<T, typename __void_type<typename __pair_types<T>::first_type>::type> {
	enum { value = _Pair }; // Pair of anything
};
/*** DEPRECATED ***/
// template<typename T>
// struct SField_type<T, typename std::enable_if<std::is_base_of<Serializable, typename __shared_ptr_type<typename __container_type<T>::type>::type>::value>::type> {
// 	enum { value = _SContainer }; // Container of serizlizables
// };
// template<typename T>
// struct SField_type<T, typename std::enable_if<std::is_pod<typename __container_type<T>::type>::value>::type> {
// 	enum { value = _PODContainer }; // Container of PODs
// };

template<typename T, int _TypeSpec>
class SField_helper;

template<typename T>
class SField;
template<typename T>
class SFieldNotifying;

class SerializableConstructorBase {
public:
	virtual ~SerializableConstructorBase();
	virtual std::shared_ptr<Serializable> operator() () = 0;
};

template<class T>
class SerializableConstructor : public SerializableConstructorBase {
public:
	SerializableConstructor();

	std::shared_ptr<Serializable> operator() () override;
};

} /* namespace _internal */

// If you want support of arrays of derived classes, register each class with RegisterClass()
// and use std::shared_ptr for storage in arrays
// Call RegisterSerializableClass for each of your classes in main()
class Serializable {
	static std::map<std::string, std::shared_ptr<_internal::SerializableConstructorBase>> constructors_;
	std::vector<std::shared_ptr<_internal::SFieldBase>> fields_;
	bool registered_;

public:
	Serializable();
	virtual ~Serializable();

	virtual void Serialize(std::ostream &output);
	virtual void Deserialize(std::istream &input);

	template<class T>
	static bool RegisterSerializableClass();

protected:
	virtual void RegisterFields() = 0;

	template<typename T>
	void RegisterField(T &field);
	template<typename T>
	void RegisterField(T &field, EventBase<_internal::SFieldNotifying<T>> *onDeserializeEvent);

private:
	static std::shared_ptr<Serializable> ConstructType(const std::string &name);
	void EnsureFieldsRegistered();

	template<typename T, int _TypeSpec>
	friend class _internal::SField_helper;
};

namespace _internal {

template<typename T>
class SField_helper<T, _POD> : public SFieldBase {
	T &data_;
public:
	SField_helper(T &data);

	void Serialize(std::ostream &output) override;
	void Deserialize(std::istream &input) override;
};

template<class T>
class SField_helper<T, _SSharedPtr> : public SFieldBase {
	T &value_;
public:
	SField_helper(T &value);

	void Serialize(std::ostream& output) override;
	void Deserialize(std::istream& input) override;
};

template<class T>
class SField_helper<T, _Container> : public SFieldBase {
	T &container_;
public:
	SField_helper(T &container);

	void Serialize(std::ostream& output) override;
	void Deserialize(std::istream& input) override;
};
template<class T>
class SField_helper<T, _Pair> : public SFieldBase {
	T &pair_;
public:
	SField_helper(T &pair);

	void Serialize(std::ostream& output) override;
	void Deserialize(std::istream& input) override;
};

/*** DEPRECATED ***/
// template<class T>
// class SField_helper<T, _PODContainer> : public SFieldBase {
// 	T &container_;
// public:
// 	SField_helper(T &container);
//
// 	void Serialize(std::ostream& output) override;
// 	void Deserialize(std::istream& input) override;
// };
//
// template<class T>
// class SField_helper<T, _SContainer> : public SFieldBase {
// 	T &container_;
// public:
// 	SField_helper(T &container);
//
// 	void Serialize(std::ostream& output) override;
// 	void Deserialize(std::istream& input) override;
// };

template<typename T>
class SField : public SField_helper<T, SField_type<T>::value> {
public:
	SField(T &data);
};

template<typename T>
class SFieldNotifying : public SField_helper<T, SField_type<T>::value> {
	EventCaller<SFieldNotifying<T>> event_;
public:
	SFieldNotifying(T &data, EventBase<SFieldNotifying<T>> *onDeserializeEvent);

	void Deserialize(std::istream &input) override;
};

template<typename T>
std::shared_ptr<SFieldBase> make_sfield(T &field);

template<typename T>
std::shared_ptr<SFieldBase> make_sfield(T &field, EventBase<SFieldNotifying<T>> *onDeserializeEvent);


template<typename _Tp, typename _Up>
typename __void_type<decltype(static_cast<std::pair<typename _Tp::iterator,bool> (_Tp::*)(const typename _Tp::const_reference)>(&_Tp::insert))>::type
__add_to_container(_Tp &container, _Up value);

template<typename _Tp, typename _Up>
typename __void_type<decltype(static_cast<void (_Tp::*)(const typename _Tp::const_reference)>(&_Tp::push_back))>::type
__add_to_container(_Tp &container, _Up value);

template<typename _Tp, typename _Up>
typename __void_type<decltype(static_cast<void (_Tp::*)(typename _Tp::value_type)>(&_Tp::push_back))>::type
__add_to_container(_Tp &container, _Up value);

} /* namespace serializing::_internal */

} /* namespace serializing */

#include "Serializable.hpp"

#endif /* SERIALIZABLE_H_ */
