/*
 * iostream.hpp
 *
 *  Created on: Feb 21, 2011
 *      Author: fqhuy
 */

#ifndef IOSTREAM_HPP_
#define IOSTREAM_HPP_

namespace Sp{

class InputStream{
public:
	virtual ~InputStream(){}

	virtual std::string ReadLine(uint32_t streamsize, char delim='\n')=0;

	virtual int8_t ReadInt8()=0;
	virtual int32_t ReadInt8(int8_t* array, int32_t offset, uint32_t length)=0;
	virtual uint8_t ReadUInt8()=0;

	virtual int16_t ReadInt16()=0;
	virtual uint16_t ReadUInt16()=0;

	virtual int32_t ReadInt32()=0;
	virtual uint32_t ReadUInt32()=0;

	virtual int64_t ReadInt64()=0;
	virtual uint64_t ReadUInt64()=0;

	virtual float ReadFloat()=0;
	virtual double ReadDouble()=0;

	virtual void Seek(uint64_t pos)=0;
	virtual void Reset()=0;
	virtual uint64_t Skip(uint64_t num_bytes)=0;
	virtual void Close()=0;
};

class OutputStream: public InputStream {

};

class InputStreamImpl: public InputStream {
public:
	virtual ~InputStreamImpl(){}

	virtual std::string ReadLine(uint32_t streamsize, char delim='\n')=0;;
	virtual int8_t ReadInt8()=0;
	virtual int32_t ReadInt8(int8_t* array, int32_t offset, uint32_t length)=0;

	virtual uint8_t ReadUInt8();

	virtual int16_t ReadInt16();
	virtual uint16_t ReadUInt16();

	virtual int32_t ReadInt32();
	virtual uint32_t ReadUInt32();

	virtual int64_t ReadInt64();
	virtual uint64_t ReadUInt64();

	virtual float ReadFloat();
	virtual double ReadDouble();

	virtual void Seek(uint64_t pos);
	virtual void Reset();
	virtual uint64_t Skip(uint64_t num_bytes);

	virtual void Close();
};

class OutputStreamImpl: public OutputStream {

};

class FileInputStream: public InputStreamImpl {
public:
	FileInputStream(const std::string& file_name);
	virtual ~FileInputStream(){
		Close();
		//delete file_;
	}
	virtual std::string ReadLine(uint32_t streamsize, char delim='\n');
	virtual int8_t ReadInt8();
	virtual int32_t ReadInt8(int8_t* array, int32_t offset, uint32_t length);
	virtual void Close();
protected:
	std::ifstream* file_;
};

}

#endif /* IOSTREAM_HPP_ */
