/*
 * matrixio.hpp
 *
 *  Created on: Feb 21, 2011
 *      Author: fqhuy
 */

#ifndef MATRIXIO_HPP_
#define MATRIXIO_HPP_

namespace Sp {

class MatrixReader {
public:
	/**
	 * @param url url to the matrix (file, http url, ftp url, ldap url ... )
	 * @param matrix the pointer to the matrix to be constructed.
	 * @param memory_model the MemoryModel used to create the matrix. default is MemoryModel if NULL.
	 * @param sample_model the underlining data layout of this matrix. default is PixelInterleavedSampleModel if NULL.
	 * @return something...
	 */
	virtual int ReadAsDoubleMatrix(const std::string& url,
			Matrix<double, double>*& matrix,
			MemoryModel<double>* memory_model =NULL, 
			SampleModel<double,double>* sample_model = NULL)=0;
	virtual int ReadAsInt32Matrix(const std::string& url,
			Matrix<int32_t, int32_t>*& matrix,
			MemoryModel<int32_t>* memory_model = NULL,
			SampleModel<int32_t, int32_t>* sample_model = NULL)=0;
	virtual int ReadAsInt16Matrix(const std::string& url,
			Matrix<int16_t, int16_t>*& matrix,
			MemoryModel<int16_t>* memory_model = NULL,
			SampleModel<int16_t, int16_t>* sample_model = NULL)=0;
	virtual int ReadAsInt8Matrix(const std::string& url,
			Matrix<int8_t, int8_t>*& matrix,
			MemoryModel<int8_t>* memory_model = NULL, 
			SampleModel<int8_t,int8_t>* sample_model = NULL)=0;

	void SetInputStream(InputStream* input_stream) {
		input_stream_ = input_stream;
	}

	InputStream& GetInputStream() {
		return *input_stream_;
	}
protected:
	InputStream* input_stream_;
	uint32_t width_;
	uint32_t height_;
};

class MatrixReaderWriterPlugin: public PluginBase {
public:
	virtual ~MatrixReaderWriterPlugin() {
	}

	std::vector<std::string> GetFormatNames() {
		return format_names_;
	}
protected:
	MatrixReaderWriterPlugin(
			std::string vendor_name,
			std::string version,
			std::vector<std::string>& format_names,
			std::vector<std::string>& suffixes,
			std::vector<std::string>& MIME_types):
				vendor_name_(vendor_name),
				version_(version),
				format_names_(format_names),
				suffixes_(suffixes),
				MIME_types_(MIME_types){
	}
	std::vector<std::string> format_names_;
	std::string vendor_name_;
	std::string version_;
	std::vector<std::string> suffixes_;
	std::vector<std::string> MIME_types_;
};

class MatrixReaderPlugin: MatrixReaderWriterPlugin {
public:
	virtual ~MatrixReaderPlugin() {
	}

	virtual MatrixReader* CreateMatrixReader()=0;
protected:
	MatrixReaderPlugin(
			std::string vendor_name,
			std::string version,
			std::vector<std::string>& format_names,
			std::vector<std::string>& suffixes,
			std::vector<std::string>& MIME_types): Super(vendor_name, version, format_names, suffixes, MIME_types) {
	}
private:

	typedef MatrixReaderWriterPlugin Super;
};

}

#endif /* MATRIXIO_HPP_ */
