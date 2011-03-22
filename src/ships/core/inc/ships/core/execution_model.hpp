/*
 * execution_model.hpp
 *
 *  Created on: Feb 24, 2011
 *      Author: fqhuy
 */

#ifndef EXECUTION_MODEL_HPP_
#define EXECUTION_MODEL_HPP_

namespace Sp {

class ExecutionModel {
public:
	ExecutionModel() {
	}
	virtual ~ExecutionModel() {
	}
};

class DeviceExecutionModel: ExecutionModel {
public:
	DeviceExecutionModel(
			const uint32_t& gsize_1,
			const uint32_t& gsize_2,
			const uint32_t& gsize_3,
			const uint32_t& lsize_1,
			const uint32_t& lsize_2,
			const uint32_t& lsize_3,
			std::vector<cl::Device> devices,
			cl::Context context
			) : devices_(devices), context_(context), Super(){

		if(gsize_3==lsize_3==1){
			global_ = cl::NDRange(gsize_1, gsize_2);
			local_ = cl::NDRange(lsize_1, lsize_2);
		}
		else if(gsize_3==lsize_3==gsize_2==lsize_2==1){
			global_ = cl::NDRange(gsize_1);
			local_ = cl::NDRange(lsize_1);
		} else {
			global_ = cl::NDRange(gsize_1, gsize_2, gsize_3);
			local_ = cl::NDRange(lsize_1, lsize_2, lsize_3);
		}
	}
	SHIPS_INLINE
	cl::NDRange GetGlobal(){
		return global_;
	}

	SHIPS_INLINE
	cl::NDRange GetLocal(){
		return local_;
	}

	SHIPS_INLINE
	cl::Context GetContext(){
		return context_;
	}
protected:
	cl::NDRange global_;
	cl::NDRange local_;
	std::vector<cl::Device> devices_;
	cl::Context context_;

private:
	typedef ExecutionModel Super;
};
}
#endif /* EXECUTION_MODEL_HPP_ */
