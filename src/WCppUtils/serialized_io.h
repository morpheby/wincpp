/*
 * serialized_io.h
 * wincpp
 * 
 * Created by Илья Михальцов on 2013-05-26.
 * Copyright 2013 Илья Михальцов. All rights reserved.
 */


#ifndef SERIALIZED_IO_H
#define SERIALIZED_IO_H

#include <iostream>
#include <stddef.h>

namespace serializing {

namespace _io {

void write_data(const void *p, size_t sz, std::ostream &output);
void write_name(const std::string &name, std::ostream &output);

template<typename _Tp>
void write_container(const std::basic_string<_Tp> &container, std::ostream &output);
template<typename _Tp>
void write_container(const _Tp &container, std::ostream &output);
template<typename _Tp>
void write_container_size(const _Tp &container, std::ostream &output);



void read_data(void *p, size_t sz, std::istream &input);
std::string read_name(std::istream &input);


template<typename _Tp>
void read_container(std::basic_string<_Tp> &container, std::istream &input);

/*** DEPRECATED ***/
// template<typename _Tp>
// typename std::enable_if<!std::is_base_of<std::random_access_iterator_tag, typename _Tp::iterator>::value>::type
// read_container(_Tp &container, std::istream &input);
// 
// template<typename _Tp>
// typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, typename _Tp::iterator>::value>::type
// read_container(_Tp &container, std::istream &input);

size_t read_container_size(std::istream &input);

} /* namespace serializing::_io */

} /* namespace serializing */

#include "serialized_io.hpp"

#endif /* SERIALIZED_IO_H */

