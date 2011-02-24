#ifndef __SHIPS_CORE_IMAGE_HPP__
#define __SHIPS_CORE_IMAGE_HPP__

namespace Sp {

class ImageMetadata: public Metadata {
public:
	ImageMetadata();// {	}

	virtual ~ImageMetadata() {
	}

protected:

private:
};

template<class T1, class T2> class Image {
public:
	typedef T2 ValueType2;
	typedef T1 ValueType1;
	typedef T1& Reference;
	typedef T1* Pointer;
	typedef Image<T1,T2> SelfType;
	Image();
	Image(const SelfType& img);
	virtual ~Image() {
		if(array_)
			delete array_;
		if(sample_model_)
			delete sample_model_;
		if(color_model_)
			delete color_model_;
		if(meta_data_)
			delete meta_data_;
	}

	//---------- GET ----------//

	//---------- TEG ----------//

protected:
	int channels_;
	int depth_;
	Array<ValueType1>* array_;
	SampleModel<ValueType1, ValueType2>* sample_model_;
	ColorModel* color_model_;
	ImageMetadata* meta_data_;
private:

};

template<class T1, class T2> class HostImage: public Image<T1,T2> {
public:
	HostImage();
	virtual ~HostImage() {
	}
};
}
#endif
