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
#endif

#endif /* SHAREDPTR_H_ */
