/*
 * plugin_octave_text_matrix.hpp
 *
 *  Created on: Feb 21, 2011
 *      Author: fqhuy
 */

#ifndef PLUGIN_OCTAVE_TEXT_MATRIX_HPP_
#define PLUGIN_OCTAVE_TEXT_MATRIX_HPP_

namespace Sp {
class OctaveTextMatrixReader: public MatrixReader {
public:
	int ReadAsFloatMatrix(const std::string& url,
			Matrix<float, float>*& matrix, MemoryModel<float>* memory_model =
					NULL, SampleModel<float, float>* sample_model = NULL);

	int ReadAsDoubleMatrix(const std::string& url,
			Matrix<double, double>*& matrix, MemoryModel<double>* memory_model =
					NULL, SampleModel<double, double>* sample_model = NULL);

	int ReadAsInt32Matrix(const std::string& url,
			Matrix<int32_t, int32_t>*& matrix,
			MemoryModel<int32_t>* memory_model = NULL,
			SampleModel<int32_t,int32_t>* sample_model = NULL);
	int ReadAsInt16Matrix(const std::string& url,
			Matrix<int16_t, int16_t>*& matrix,
			MemoryModel<int16_t>* memory_model = NULL,
			SampleModel<int16_t,int16_t>* sample_model = NULL);
	int ReadAsInt8Matrix(const std::string& url,
			Matrix<int8_t, int8_t>*& matrix, MemoryModel<int8_t>* memory_model =
					NULL, SampleModel<int8_t,int8_t>* sample_model = NULL);
protected:
	template<class T1, class T2> int Read(const std::string& url,Matrix<T1, T2>*& matrix, MemoryModel<T2>* memory_model =
			NULL, SampleModel<T1, T2>* sample_model = NULL);
};
class OctaveTextMatrixReaderPlugin: public MatrixReaderPlugin {
public:
	OctaveTextMatrixReaderPlugin(
			std::string vendor_name,
			std::string version,
			std::vector<std::string>& format_names,
			std::vector<std::string>& suffixes,
			std::vector<std::string>& MIME_types): Super(vendor_name, version, format_names, suffixes, MIME_types) {
	}
	MatrixReader* CreateMatrixReader();
private:
	typedef MatrixReaderPlugin Super;
};
}

#endif /* PLUGIN_OCTAVE_TEXT_MATRIX_HPP_ */
