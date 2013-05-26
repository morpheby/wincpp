/*
 * serialized_io.cpp
 * wincpp
 * 
 * Created by Илья Михальцов on 2013-05-26.
 * Copyright 2013 Илья Михальцов. All rights reserved.
 */


#include "serialized_io.h" 

typedef struct tagSERIALIZABLE_NAME_T {
	size_t sz;
	char name[0];
} _SerializableName_t;

	
void serializing::_io::write_data(const void *p, size_t sz, std::ostream &output) {
	output.rdbuf()->sputn((char *) p, sz);
}

void serializing::_io::write_name(const std::string &name, std::ostream &output) {
	write_container_size(name, output);
	write_data(name.c_str(), name.size(), output);
}

void serializing::_io::read_data(void *p, size_t sz, std::istream &input) {
	input.rdbuf()->sgetn((char *) p, sz);
}

std::string serializing::_io::read_name(std::istream &input) {
	size_t sz = read_container_size(input);
	char *str = new char[sz];
	read_data(str, sz, input);
	std::string s(str, sz);
	delete[] str;
	return s;
}

size_t serializing::_io::read_container_size(std::istream &input) {
	size_t sz;
	read_data(&sz, sizeof(sz), input);
	return sz;
}
