/*
 * utf8.h
 *
 *  Created on: 24.05.2012
 *      Author: Morphe
 */

#ifndef UTF8_H_
#define UTF8_H_

#include <string>

/// Converts wide string to utf-8
std::string utf8Str(const std::wstring& str);

/// Converts utf-8 string to wide string
std::wstring wideStr(const std::string& str);

#endif /* UTF8_H_ */
