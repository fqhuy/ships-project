/*
 * matrix_operations.hpp
 *
 *  Created on: Mar 17, 2011
 *      Author: fqhuy
 */

#ifndef MATRIX_OPERATIONS_HPP_
#define MATRIX_OPERATIONS_HPP_

namespace Sp{

enum SortTypes{
	SORT_ROWS=0, SORT_COLUMNS, SORT_DIAGONALS
};

enum SortDirections{
	ASC=0, DESC
};
template<class T> class MatrixSorter: MatrixOperation<T, T> {
public:
	/*
	 * @brief constructor
	 * @param sort_type @see SortTypes
	 * @param direction
	 */
	MatrixSorter(SortTypes sort_type, SortDirections direction,  const int& start, const int& end, const ExecutionModel* execution_model): sort_type_(sort_type), direction_(direction),start_(start), end_(end), Super(execution_model){

	}
	virtual ~MatrixSorter(){}

	virtual Matrix<T, T>* Perform(const Matrix<T, T>* src, Matrix<T, T>* dest)=0;
	virtual std::vector<Matrix<T, T>* > Perform(const std::vector<Matrix<T, T>* >& srcs, std::vector<Matrix<T, T>* > dests)=0;
protected:
	SortTypes sort_type_;
	SortDirections direction_;
	int start_;
	int end_;
private:
	typedef MatrixOperation<T, T> Super;
};

template<class T> class DeviceMatrixSorter: MatrixSorter<T> {
public:
	DeviceMatrixSorter(SortTypes sort_type, const int& start, const int& end, const ExecutionModel* execution_model):Super(sort_type,start, end, execution_model){}
	virtual ~DeviceMatrixSorter(){}

	Matrix<T, T>* Perform(const Matrix<T, T>* src, Matrix<T, T>* dest);
	std::vector<Matrix<T, T>* > Perform(const std::vector<Matrix<T, T>* >& srcs, std::vector<Matrix<T, T>* > dests);
private:
	typedef MatrixSorter<T> Super;

	class Scanner{
	public:
		Scanner(cl::Context context, cl::CommandQueue queue, unsigned int num_elements){

		}
		~Scanner(){}
		void scanExclusiveLarge(
							cl::Buffer d_Dst,
							cl::Buffer d_Src,
							unsigned int batchSize,
							unsigned int arrayLength);
	};
};
}

#endif /* MATRIX_OPERATIONS_HPP_ */
