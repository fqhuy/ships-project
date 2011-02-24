/*
 * plugin_octave_text_matrix.cpp
 *
 *  Created on: Feb 21, 2011
 *      Author: fqhuy
 */
#include <ships/io/io.hpp>
namespace Sp {
int OctaveTextMatrixReader::ReadAsDoubleMatrix(const std::string& url, Matrix<
		double, double>*& matrix, MemoryModel<double>* memory_model,
		SampleModel<double, double>* sample_model) {
	if (memory_model == NULL)
		memory_model = new MemoryModel<double> (2, true, true, READ_WRITE);
	if (this->input_stream_ == NULL)
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

	if (sample_model == NULL)
		sample_model = new PixelInterleavedSampleModel<double> (1, 2, sizes);

	matrix = new Matrix<double, double> (sizes[0], sizes[1], memory_model,
			sample_model);

	float temp;
	for (int i = 0; i < sizes[1]; i++) {
		this->input_stream_->ReadLine(128, ' ');
		for (int j = 0; j < sizes[0] - 1; j++) {
			s = this->input_stream_->ReadLine(128, ' ');
			// TODO: don't know which argument is suitable for double 
			sscanf(s.c_str(), "%f", &temp);
			matrix->Set(temp, i, j);
		}
		s = this->input_stream_->ReadLine(128, '\n');
		sscanf(s.c_str(), "%f", &temp);
		matrix->Set(temp, i, sizes[0] - 1);
	}
	return 0;

}
int OctaveTextMatrixReader::ReadAsInt32Matrix(const std::string& url, Matrix<
		int32_t, int32_t>*& matrix, MemoryModel<int32_t>* memory_model,
		SampleModel<int32_t, int32_t>* sample_model) {
}
int OctaveTextMatrixReader::ReadAsInt16Matrix(const std::string& url, Matrix<
		int16_t, int16_t>*& matrix, MemoryModel<int16_t>* memory_model,
		SampleModel<int16_t, int16_t>* sample_model) {
}
int OctaveTextMatrixReader::ReadAsInt8Matrix(const std::string& url, Matrix<
		int8_t, int8_t>*& matrix, MemoryModel<int8_t>* memory_model,
		SampleModel<int8_t, int8_t>* sample_model) {
}
MatrixReader* OctaveTextMatrixReaderPlugin::CreateMatrixReader() {
	return new OctaveTextMatrixReader();
}
}
