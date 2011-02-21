/*
 * iostream.cpp
 *
 *  Created on: Feb 21, 2011
 *      Author: fqhuy
 */

#include <ships/io/io.hpp>

namespace Sp {
/******************** InputStreamImpl ********************/
std::string InputStreamImpl::ReadLine(uint32_t streamsize, char delim) {
	return NULL;
}
uint8_t InputStreamImpl::ReadUInt8() {
	return this->ReadInt8();
}

int16_t InputStreamImpl::ReadInt16() {
	int16_t re;
	this->ReadInt8((int8_t*) &re, 0, 2);
	return re;
}
uint16_t InputStreamImpl::ReadUInt16() {
	return this->ReadInt16();
}

int32_t InputStreamImpl::ReadInt32() {
	int32_t re;
	this->ReadInt8((int8_t*) &re, 0, 4);
	return re;
}
uint32_t InputStreamImpl::ReadUInt32() {
	return this->ReadInt32();
}

int64_t InputStreamImpl::ReadInt64() {
	int64_t re;
	this->ReadInt8((int8_t*) &re, 0, 8);
	return re;
}
uint64_t InputStreamImpl::ReadUInt64() {
	return this->ReadInt64();
}

float InputStreamImpl::ReadFloat() {
	float re;
	this->ReadInt8((int8_t*) &re, 0, sizeof(float));
	return re;
}

double InputStreamImpl::ReadDouble() {
	double re;
	this->ReadInt8((int8_t*) &re, 0, sizeof(double));
	return re;
}

void InputStreamImpl::Seek(uint64_t pos) {
}
void InputStreamImpl::Reset() {
}
uint64_t InputStreamImpl::Skip(uint64_t num_bytes) {
}

void InputStreamImpl::Close() {
}
/******************** FileInputStream ********************/
std::string FileInputStream::ReadLine(uint32_t streamsize, char delim) {
	char buff[streamsize];
	this->file_->getline(buff, streamsize, delim);
	std::string re(buff);
	return re;
}

FileInputStream::FileInputStream(const std::string& file_name) {
	this->file_ = new std::ifstream(file_name.c_str(), std::ifstream::in);
}

int8_t FileInputStream::ReadInt8() {
	return this->file_->get();
}

int32_t FileInputStream::ReadInt8(int8_t* array, int32_t offset,
		uint32_t length) {
	array += offset;
	this->file_->get((char*) array, length);
	return length;
}

void FileInputStream::Close() {
	this->file_->close();
}
}
