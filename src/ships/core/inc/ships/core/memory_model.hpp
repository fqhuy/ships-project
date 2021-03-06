/*
 * memory_model.hpp
 *
 *  Created on: Feb 12, 2011
 *      Author: fqhuy
 */

#ifndef SHIPS_CORE_MODEL_HPP_
#define SHIPS_CORE_MODEL_HPP_

namespace Sp {

enum AccessModes {
	READ = 0, WRITE, READ_WRITE
};

template<class T> class MemoryModel{
public:
	typedef T ValueType;
	typedef T* Pointer;
	SHIPS_INLINE
	MemoryModel(){

	}
	SHIPS_INLINE
	MemoryModel(uint32_t num_dims, bool mapped = true,
			bool pinned = false, uint8_t alignment=1,AccessModes access_mode = READ_WRITE) :
		num_dims_(num_dims), mapped_(mapped), alignment_(alignment), access_mode_(access_mode),
				pinned_(pinned), mapped_memory_(NULL), flag_(0) {
		dims_[0] = dims_[1] = dims_[2] = 1;

		if(this->access_mode_==READ)
			flag_=flag_|CL_MEM_READ_ONLY;

		if(this->access_mode_==READ_WRITE)
			flag_=flag_|CL_MEM_READ_WRITE;

		if(this->access_mode_==WRITE)
				flag_=flag_|CL_MEM_WRITE_ONLY;

		if(this->pinned_ == true)
			flag_=flag_|CL_MEM_ALLOC_HOST_PTR;
		else{
			flag_=flag_|CL_MEM_USE_HOST_PTR;
		}

	}

	virtual ~MemoryModel() {
		if (mapped_==true && memory_() != NULL) {
			queue_.enqueueUnmapMemObject(memory_, mapped_memory_);
		}
		//mapped_memory_ = NULL;
	}

	SHIPS_INLINE
	bool IsMapped() {
		return mapped_;
	}

	SHIPS_INLINE
	cl::Memory& GetMemory(){
	  return memory_;
	}

	SHIPS_INLINE
	cl::Buffer& GetBuffer(){
	  return buffer_;
	}
	SHIPS_INLINE
	cl::Image& GetImage(){
		if(num_dims_ == 2)
			return image2d_;
		else if( num_dims_ == 3)
			return image3d_;

	  //return memory_;
	}
	SHIPS_INLINE
	cl::Image2D& GetImage2D(){
		return image2d_;
	}
	SHIPS_INLINE
	cl::Image3D& GetImage3D(){
		return image3d_;
	}
	SHIPS_INLINE
	Pointer GetMappedMemory(){
	  return mapped_memory_;
	}
	SHIPS_INLINE
	uint32_t GetNumDims() {
		return num_dims_;
	}
	SHIPS_INLINE
	cl::Context& GetContext(){
		return context_;
	}

	SHIPS_INLINE
	void SetContext(const cl::Context& context){
		context_ = context;
	}
	SHIPS_INLINE
	virtual cl::ImageFormat GetImageFormat(){
		return image_format_;
	}
	SHIPS_INLINE
	virtual void SetImageFormat(const cl::ImageFormat& image_format) {
		image_format_ = image_format;
	}

	SHIPS_INLINE
	virtual uint8_t GetAlignment(){
		return alignment_;
	}

	SHIPS_INLINE
	virtual void SetAlignment(uint8_t alignment){
		alignment_ = alignment;
	}
	//map all elements
	virtual Pointer Map();
	//map some elements
	virtual Pointer Map(int32_t offset, size_t size);
	//unmap
	virtual Pointer Map(int32_t x, int32_t y, uint32_t width, uint32_t height);
	//unmap
	virtual void UnMap();
	/*
	 * @brief clone
	 */
	virtual MemoryModel<ValueType>* Clone();
	//creating array in bytes
	virtual Pointer CreateArray(const uint32_t& num_dims, uint32_t dims[SP_MAX_NUM_DIMENSIONS]);
	virtual Pointer CreateArray(uint32_t width);
	virtual Pointer CreateArray(uint32_t width, uint32_t height);
	//virtual void* CreateArray(size_t width, size_t height, size_t depth);
protected:
	uint8_t alignment_;
	uint32_t num_dims_;
	uint32_t dims_[3];
	bool mapped_;
	AccessModes access_mode_;
	cl::Memory memory_;
	cl::Buffer buffer_;
	cl::Image2D image2d_;
	cl::Image3D image3d_;
	cl::Context context_;
	cl::CommandQueue queue_;
	Pointer mapped_memory_;
	cl::ImageFormat image_format_;

	cl_mem_flags flag_;

	uint32_t CalculateBufferSize(const uint32_t& num_dims, const uint32_t dims[]);
	bool pinned_;
private:

};

template<class T> class HostMemoryModel: public MemoryModel<T> {
public:
	typedef T ValueType;
	typedef T* Pointer;
	typedef MemoryModel<T> Super;
	SHIPS_INLINE
	HostMemoryModel(uint32_t num_dims, uint8_t alignment=1) :
		Super(num_dims, false, false, alignment) {
	}
	virtual ~HostMemoryModel() {
		delete [] this->mapped_memory_;
	}
	Pointer Map(int32_t offset, size_t size);
	Pointer Map();
	void UnMap();

	Super* Clone();
	Pointer CreateArray(uint32_t width);
	Pointer CreateArray(uint32_t width, uint32_t height);
	Pointer CreateArray(uint32_t width, uint32_t height, uint32_t depth);
private:

};

template<class T> class DeviceMemoryModel: public MemoryModel<T> {
public:
	typedef T ValueType;
	typedef T* Pointer;

	SHIPS_INLINE
	DeviceMemoryModel(uint32_t num_dims, uint8_t alignment=1,AccessModes access_mode = READ_WRITE){
		this->dims_[0] = this->dims_[1] = this->dims_[2] = 1;
		this->mapped_ = false;
		this->pinned_ = false;
		this->num_dims_ = num_dims;
		this->alignment_ = alignment;
		this->access_mode_ = access_mode;
		this->mapped_memory_ = NULL;
		this->flag_ = 0;

		if(this->access_mode_==READ)
			this->flag_=this->flag_|CL_MEM_READ_ONLY;

		if(this->access_mode_==READ_WRITE)
			this->flag_=this->flag_|CL_MEM_READ_WRITE;

		if(this->access_mode_==WRITE)
			this->flag_=this->flag_|CL_MEM_WRITE_ONLY;
	}

	virtual ~DeviceMemoryModel() {

	}
	Pointer Map(int32_t offset, size_t size);
	Pointer Map();
	//void UnMap();

	//Pointer CreateArray(uint32_t width);
	//void* CreateArray(size_t width, size_t height);
	//virtual void* CreateArray(size_t width, size_t height, size_t depth);
private:
	typedef MemoryModel<T> Super;
};

}
#endif /* SHIPS_CORE_MODEL_HPP_ */
