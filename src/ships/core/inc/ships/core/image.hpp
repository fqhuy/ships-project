#ifndef __SHIPS_CORE_IMAGE_HPP__
#define __SHIPS_CORE_IMAGE_HPP__

namespace Sp {
class ImageMetaData {
public:
	ImageMetaData();// {	}

	virtual ~ImageMetaData() {
	}
	;
protected:

private:
};

template<class T> class Image {
public:
	typedef T ValueType;
	typedef T& Reference;
	typedef T* Pointer;
	Image();
	Image(const Image<ValueType>& img);
	virtual ~Image() {
	}

	//---------- GET ----------//

	//---------- TEG ----------//

protected:
	int channels_;
	int depth_;
	Array<T> array_;
	//SampleModel sample_model_;
	ColorModel color_model_;
	ImageMetaData meta_data_;
private:

};

template<class T> class HostImage: public Image<T> {
public:
	HostImage();
	virtual ~HostImage() {
	}
};
}
#endif
