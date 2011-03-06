/*
 * exceptions.hpp
 *
 *  Created on: Feb 28, 2011
 *      Author: fqhuy
 */

#ifndef SHIPS_IO_EXCEPTIONS_HPP__
#define SHIPS_IO_EXCEPTIONS_HPP__

namespace Sp {

class IOExeption: public std::exception {
public:
	IOExeption() {
	}
	virtual const char* what() const throw () {
		return "IOException occurred.";
	}
};

}
#endif /* SHIPS_IO_EXCEPTIONS_HPP__ */
