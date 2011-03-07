/*
 * util.hpp
 *
 *  Created on: Feb 26, 2011
 *      Author: fqhuy
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_
namespace Sp {

class Random {
public:
	Random(){
		srand(time(NULL));
	}
	Random(int64_t seed): seed_(seed){}

	virtual ~Random(){}

	template<class T> T Next(T min, T max);

	void SRand(){
		srand(time(NULL));
	}
	void SetSeed(int64_t seed){
		seed_ = seed;
	}
private:
	int64_t seed_;
};

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

class ArrayUtils {
public:
	template<class T> static void copyArray(uint32_t start, uint32_t end,
			T* from, T* to) {
		for (int i = start; i < end; i++)
			to[i] = from[i];
	}

	template<class T> static void initArray(uint32_t start, uint32_t end,
			T value, T* array) {
		for (int i = start; i < end; i++)
			array[i] = value;
	}

	template<class T> static std::string ToString(T* array, const uint32_t& size){
		std::ostringstream oss;
		for(int i=0;i<size;i++)
			oss << array[i] << "\t";
		oss << "\n";
		return oss.str();
	}
};

class Timer {
public:
	static double deltaT(int iCounterID=0) ;
};
}
#endif /* UTIL_HPP_ */
