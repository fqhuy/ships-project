/*
 * util.hpp
 *
 *  Created on: Feb 26, 2011
 *      Author: fqhuy
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

class StringUtils {
public:
	static std::string& replaceAll(std::string& context,
			const std::string& from, const std::string& to) {
		size_t lookHere = 0;
		size_t foundHere;
		while ((foundHere = context.find(from, lookHere)) != std::string::npos) {
			context.replace(foundHere, from.size(), to);
			lookHere = foundHere + to.size();
		}
		return context;
	} ///:~
};

#endif /* UTIL_HPP_ */
