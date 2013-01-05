/*
 * sharedPtr.h
 *
 *  Created on: 21.5.2012
 *      Author: Morphe
 */

#ifndef SHAREDPTR_H_
#define SHAREDPTR_H_

#if __cplusplus >= 201103L
#include <memory>

//! Makes an shared_ptr from a pointer
//! \param ptr Pointer to share
template <typename T>
std::shared_ptr<T> SharePtr(T* ptr) {
	return std::shared_ptr<T> (ptr);
}

template <typename T, typename _Del>
std::shared_ptr<T> SharePtr(T* ptr, _Del deleter) {
	return std::shared_ptr<T> (ptr, deleter);
}

#endif

template <typename _PtrType>
struct _PtrLess {
	bool operator() (_PtrType a, _PtrType b) const {
		return *a < *b;
	}
};
template <typename _PtrType>
struct _PtrEq {
	bool operator() (_PtrType a, _PtrType b) const {
		return *a == *b;
	}
};

#endif /* SHAREDPTR_H_ */
