#ifndef __SHIPS_CORE_OPERATIONS_HPP__
#define __SHIPS_CORE_OPERATIONS_HPP__

//#include <loki/visitor.h>

namespace Sp {

template<class T1, class T2> class Operation {
public:
	typedef T1 InputType;
	typedef T1* InputPointer;
	typedef T2 OutputType;
	typedef T2* OutputPointer;

	Operation(const ExecutionModel* execution_model): execution_model_(execution_model){}
	virtual ~Operation(){
		if(execution_model_)
			delete execution_model_;
	}

	virtual OutputPointer Perform(const InputPointer src, OutputPointer dest)=0;
	virtual std::vector<OutputPointer> Perform(const std::vector<InputPointer>& srcs, std::vector<OutputPointer> dests)=0;
protected:
	ExecutionModel* execution_model_;
};

template<class T1, class T2> class MatrixOperation: public Operation<Matrix<T1, T2>,Matrix<T1, T2> >{
public:
	typedef T1 InputType;
	typedef T1* InputPointer;
	typedef T2 OutputType;
	typedef T2* OutputPointer;

	MatrixOperation(const ExecutionModel* execution_model): Super(execution_model){}
	virtual ~MatrixOperation(){}

	virtual Matrix<T1, T2>* Perform(const Matrix<T1, T2>* src, OutputPointer dest)=0;
	virtual std::vector<Matrix<T1, T2>* > Perform(const std::vector<Matrix<T1, T2>* >& srcs, std::vector<OutputPointer> dests)=0;
private:
	typedef Operation<T1, T2> Super;
};

}
#endif
