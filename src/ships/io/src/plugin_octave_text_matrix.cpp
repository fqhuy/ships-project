/*
 * plugin_octave_text_matrix.cpp
 *
 *  Created on: Feb 21, 2011
 *      Author: fqhuy
 */
#include <ships/io/io.hpp>
namespace Sp {
#define INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(T1,T2) \
		template int OctaveTextMatrixReader::Read<T1,T2>(const std::string& url, Matrix<T1, T2>*& matrix,MemoryModel<T2>* memory_model, SampleModel<T1, T2>* sample_model);

INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(float,float)
;
INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(double,double)
;
INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(uint8_t,uint8_t)
;
INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(uint16_t,uint16_t)
;
INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(uint32_t,uint32_t)
;
INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(uint64_t,uint64_t)
;
INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(int8_t,int8_t)
;
INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(int16_t,int16_t)
;
INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(int32_t,int32_t)
;
INSTANTIATE_OCTAVE_TEXT_MATRIX_READER_READ(int64_t,int64_t)
;

int OctaveTextMatrixReader::ReadAsDoubleMatrix(const std::string& url, Matrix<
		double, double>*& matrix, MemoryModel<double>* memory_model,
		SampleModel<double, double>* sample_model) {
	return this->Read<double, double> (url, matrix, memory_model, sample_model);

}

int OctaveTextMatrixReader::ReadAsFloatMatrix(const std::string& url, Matrix<
		float, float>*& matrix, MemoryModel<float>* memory_model, SampleModel<
		float, float>* sample_model) {

	return this->Read<float, float> (url, matrix, memory_model, sample_model);
}

int OctaveTextMatrixReader::ReadAsInt32Matrix(const std::string& url, Matrix<
		int32_t, int32_t>*& matrix, MemoryModel<int32_t>* memory_model,
		SampleModel<int32_t, int32_t>* sample_model) {
	return this->Read<int32_t, int32_t> (url, matrix, memory_model,
			sample_model);
}
int OctaveTextMatrixReader::ReadAsInt16Matrix(const std::string& url, Matrix<
		int16_t, int16_t>*& matrix, MemoryModel<int16_t>* memory_model,
		SampleModel<int16_t, int16_t>* sample_model) {
	return this->Read<int16_t, int16_t> (url, matrix, memory_model,
			sample_model);
}
int OctaveTextMatrixReader::ReadAsInt8Matrix(const std::string& url, Matrix<
		int8_t, int8_t>*& matrix, MemoryModel<int8_t>* memory_model,
		SampleModel<int8_t, int8_t>* sample_model) {
	return this->Read<int8_t, int8_t> (url, matrix, memory_model, sample_model);
}

MatrixReader* OctaveTextMatrixReaderPlugin::CreateMatrixReader() {
	return new OctaveTextMatrixReader();
}

template<class T1, class T2> int OctaveTextMatrixReader::Read(
		const std::string& url, Matrix<T1, T2>*& matrix,
		MemoryModel<T2>* memory_model, SampleModel<T1, T2>* sample_model) {

	this->input_stream_ = new FileInputStream(url);

	//TODO: it would be better to define an empty ReadLine funtion in InputStream.

	this->input_stream_->ReadLine(1024);
	this->input_stream_->ReadLine(1024);
	this->input_stream_->ReadLine(1024);

	uint32_t sizes[2];
	std::string s = this->input_stream_->ReadLine(1024);
	sscanf(s.c_str(), "# rows: %d", &sizes[1]);
	s = this->input_stream_->ReadLine(1024);
	sscanf(s.c_str(), "# columns: %d", &sizes[0]);

	if (matrix == NULL) {

		if (memory_model == NULL) {
			memory_model = new HostMemoryModel<T1> (2);
		}

		if (sample_model == NULL)
			sample_model = new PixelInterleavedSampleModel<T1> (1, 2, sizes);

		matrix = new Matrix<T1, T2> (sizes[0], sizes[1], memory_model,
				sample_model);
	} else {
		if (sizes[1] > matrix->GetHeight())
			sizes[1] = matrix->GetHeight();

		if (sizes[0] > matrix->GetWidth())
			sizes[0] = matrix->GetWidth();
	}

	double temp;
	for (int i = 0; i < sizes[1]; i++) {
		this->input_stream_->ReadLine(128, ' ');
		for (int j = 0; j < sizes[0] - 1; j++) {
			s = this->input_stream_->ReadLine(128, ' ');
			// TODO: don't know which argument is suitable for double
			sscanf(s.c_str(), "%lf", &temp);
			matrix->Set((T1) temp, i, j);
		}
		s = this->input_stream_->ReadLine(128, '\n');
		sscanf(s.c_str(), "%lf", &temp);

		matrix->Set((T1) temp, i, sizes[0] - 1);
	}

	delete this->input_stream_;
	return 0;
}
}
