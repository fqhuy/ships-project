/*
 * exceptions.hpp
 *
 *  Created on: Feb 28, 2011
 *      Author: fqhuy
 */

#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

namespace Sp{
class ShipsException: public std::exception {
public:
	explicit
	ShipsException(const char *s = "ShipsException") {
		strcpy(message_,s);
	}
	virtual const char* what() const throw () {
		return message_;
	}
protected:
	char message_[256];
};

class IOExeption: public ShipsException {
public:
	explicit IOExeption(const char *s = "IOException"): ShipsException(s) {

	}
};

class FileNotFoundException: public IOExeption {
public:
	explicit
	FileNotFoundException(const char *s = "FileNotFoundException"): IOExeption(s){
	}
};

class InvalidContextException: ShipsException {
public:
	explicit
	InvalidContextException(const char *s = "InvalidContextException"): ShipsException(s) {
	}
};


class InvalidValueException: ShipsException {
public:
	explicit
	InvalidValueException(const char *s = "InvalidValueException"): ShipsException(s) {
	}
};

class InvalidDeviceException: ShipsException {
public:
	explicit
	InvalidDeviceException(const char *s = "InvalidDeviceException"): ShipsException(s) {
	}
};

class InvalidProgramException: ShipsException {
public:
	explicit
	InvalidProgramException(const char *s = "InvalidProgramException"): ShipsException(s) {
	}
};

class InvalidKernelException: ShipsException {
public:
	explicit
	InvalidKernelException(const char *s = "InvalidKernelException"): ShipsException(s) {
	}
};

class InvalidMemObjectException: ShipsException {
public:
	explicit
	InvalidMemObjectException(const char *s = "InvalidMemObjectException"): ShipsException(s) {
	}
};
}

#endif /* EXCEPTIONS_HPP_ */
