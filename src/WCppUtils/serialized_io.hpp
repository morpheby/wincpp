/*
 * serialized_io.hpp
 * wincpp
 * 
 * Created by Илья Михальцов on 2013-05-26.
 * Copyright 2013 Илья Михальцов. All rights reserved.
 */



template<typename _Tp>
void serializing::_io::write_container(const std::basic_string<_Tp> &container, std::ostream &output) {
	write_container_size(container, output);
	write_data(container.c_str(), sizeof(_Tp)*container.size(), output);
}

template<typename _Tp>
void serializing::_io::write_container(const _Tp &container, std::ostream &output) {
	write_container_size(container, output);
	for(typename _Tp::value_type x : container)
		write_data(&x, sizeof(x), output);
}


template<typename _Tp>
void serializing::_io::read_container(std::basic_string<_Tp> &container, std::istream &input) {
	size_t sz = read_container_size(input);
	_Tp *str = new _Tp[sz];
	read_data(str, sz*sizeof(_Tp), input);
	container = std::basic_string<_Tp>(str, sz);
	delete[] str;
}

/*** DEPRECATED ***/
// template<typename _Tp>
// typename std::enable_if<!std::is_base_of<std::random_access_iterator_tag, typename _Tp::iterator>::value>::type
// serializing::_io::read_container(_Tp &container, std::istream &input) {
// 	size_t sz = read_container_size(input);
// 	for(size_t i = 0; i < sz; ++i) {
// 		typename _Tp::value_type x;
// 		read_data(&x, sizeof(x), input);
// 		container.emplace(x);
// 	}
// }
// 
// template<typename _Tp>
// typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, typename _Tp::iterator>::value>::type
// serializing::_io::read_container(_Tp &container, std::istream &input) {
// 	size_t sz = read_container_size(input);
// 	for(size_t i = 0; i < sz; ++i) {
// 		typename _Tp::value_type x;
// 		read_data(&x, sizeof(x), input);
// 		container.push_back(x);
// 	}
// }

template<typename _Tp>
void serializing::_io::write_container_size(const _Tp &container, std::ostream &output) {
	size_t sz = container.size();
	write_data(&sz, sizeof(sz), output);
}
