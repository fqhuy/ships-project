/*
 * memory_model.hpp
 *
 *  Created on: Feb 12, 2011
 *      Author: fqhuy
 */

#ifndef __SHIPS_CORE_MODEL_HPP_
#define __SHIPS_CORE_MODEL_HPP_

namespace Sp {

enum AccessModes {
	READ = 0, WRITE, READ_WRITE
};

template<class T> class MemoryModel{
public:
	typedef T ValueType;
	typedef T* Pointer;

	SHIPS_INLINE MemoryModel(uint32_t num_dims, bool mapped = true,
			bool pinned = false, AccessModes access_mode = READ_WRITE) :
		num_dims_(num_dims), mapped_(mapped), access_mode_(access_mode),
				pinned_(pinned) {
	}

	virtual ~MemoryModel() {
		if (mapped_ && memory_() != NULL) {
			queue_.enqueueUnmapMemObject(memory_, mapped_memory_);
		}
		//mapped_memory_ = NULL;
	}

	SHIPS_INLINE
	bool IsMapped() {
		return mapped_;
	}

	SHIPS_INLINE
	uint32_t GetNumDims() {
		return num_dims_;
	}

	SHIPS_INLINE
	virtual void SetImageFormat(const cl::ImageFormat& image_format) {
		image_format_ = image_format;
	}

	//map all elements
	virtual Pointer Map();
	//map some elements
	virtual Pointer Map(int32_t offset, size_t size);
	//unmap
	virtual Pointer Map(int32_t x, int32_t y, uint32_t width, uint32_t height);
	//unmap
	virtual void UnMap();
	//creating array in bytes
	virtual Pointer CreateArray(uint32_t width);
	virtual Pointer CreateArray(uint32_t width, uint32_t height);
	//virtual void* CreateArray(size_t width, size_t height, size_t depth);
protected:
	uint32_t num_dims_;
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
private:
	bool pinned_;
};

template<class T> class HostMemoryModel: public MemoryModel<T> {
public:
	typedef T ValueType;
	typedef T* Pointer;
	SHIPS_INLINE HostMemoryModel(uint32_t num_dims) :
		Super(num_dims) {
	}
	virtual ~HostMemoryModel() {
		delete this->mapped_memory_;
	}
	Pointer Map(int32_t offset, size_t size);
	Pointer Map();
	void UnMap();

	Pointer CreateArray(uint32_t width);
	//void* CreateArray(size_t width, size_t height);
	//void* CreateArray(size_t width, size_t height, size_t depth);
private:
	typedef MemoryModel<T> Super;
};

template<class T> class DeviceMemoryModel: public MemoryModel<T> {
public:
	typedef T ValueType;
	typedef T* Pointer;

	SHIPS_INLINE DeviceMemoryModel(uint32_t num_dims, bool mapped = false) :
		Super(num_dims, mapped) {
	}

	virtual ~DeviceMemoryModel() {

	}
	Pointer Map(int32_t offset, size_t size);
	Pointer Map();
	void UnMap();

	Pointer CreateArray(uint32_t width);
	//virtual void* CreateArray(size_t width, size_t height);
	//virtual void* CreateArray(size_t width, size_t height, size_t depth);
private:
	typedef MemoryModel<T> Super;
};

}
#endif /* __SHIPS_CORE_MODEL_HPP_ */
